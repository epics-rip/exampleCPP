/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */
#include <iostream>
#include <epicsGetopt.h>
#include <pv/pvaClient.h>
#include <pv/convert.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

class ClientAddRecord;
typedef std::tr1::shared_ptr<ClientAddRecord> ClientAddRecordPtr;

class ClientAddRecord :
    public PvaClientChannelStateChangeRequester,
    public PvaClientPutGetRequester,
    public std::tr1::enable_shared_from_this<ClientAddRecord>
{
private:
    string channelName;
    string providerName;
    string request;
    bool channelConnected;
    bool putGetConnected;

    PvaClientChannelPtr pvaClientChannel;
    PvaClientPutGetPtr pvaClientPutGet;

    void init(PvaClientPtr const &pvaClient)
    {
        pvaClientChannel = pvaClient->createChannel(channelName,providerName);
        pvaClientChannel->setStateChangeRequester(shared_from_this());
        pvaClientChannel->issueConnect();
    }
public:
    POINTER_DEFINITIONS(ClientAddRecord);
    ClientAddRecord(
        const string &channelName,
        const string &providerName,
        const string &request)
    : channelName(channelName),
      providerName(providerName),
      request(request),
      channelConnected(false),
      putGetConnected(false)
    {
    }
    
    static ClientAddRecordPtr create(
        PvaClientPtr const &pvaClient,
        const string & channelName,
        const string & providerName,
        const string  & request)
    {
        ClientAddRecordPtr client(ClientAddRecordPtr(
             new ClientAddRecord(channelName,providerName,request)));
        client->init(pvaClient);
        return client;
    }

    virtual void channelStateChange(PvaClientChannelPtr const & channel, bool isConnected)
    {
        channelConnected = isConnected;
        if(isConnected) {
            if(!pvaClientPutGet) {
                pvaClientPutGet = pvaClientChannel->createPutGet(request);
                pvaClientPutGet->setRequester(shared_from_this());
                pvaClientPutGet->issueConnect();
            }
        }
    }

    virtual void channelPutGetConnect(
        const epics::pvData::Status& status,
        PvaClientPutGetPtr const & clientAddRecord)
    {
        putGetConnected = true;
        cout << "channelPutGetConnect " << channelName << " status " << status << endl;
    }

    
    virtual void putGetDone(
        const epics::pvData::Status& status,
        PvaClientPutGetPtr const & clientAddRecord)
    {
         if(status.isOK()) {
             cout << pvaClientPutGet->getGetData()->getPVStructure() << endl;
         } else {
             cout << "putGetDone " << channelName << " status " << status << endl;
         }
    }
    virtual void getGetDone(
        const epics::pvData::Status& status,
        PvaClientPutGetPtr const & clientPutGet)
    {
         if(status.isOK()) {
             cout << pvaClientPutGet->getGetData()->getPVStructure() << endl;
         } else {
             cout << "getGetDone " << channelName << " status " << status << endl;
         }
    }
    
    virtual void getPutDone(
        const epics::pvData::Status& status,
        PvaClientPutGetPtr const & clientPutGet)
    {
         if(status.isOK()) {
             cout << pvaClientPutGet->getPutData()->getPVStructure() << endl;
         } else {
             cout << "getPutDone " << channelName << " status " << status << endl;
         }
    }

    void putGet(const string & value)
    {
        if(!channelConnected) {
            cout << channelName << " channel not connected\n";
            return;
        }
        if(!putGetConnected) {
            cout << channelName << " channelAddRecord not connected\n";
            return;
        }
        PvaClientPutDataPtr putData = pvaClientPutGet->getPutData();
        PVStructurePtr pvStructure = putData->getPVStructure();
        PVStringPtr pvName = pvStructure->getSubField<PVString>("argument.recordName");
        if(!pvName) {
             cout << "argument.recordName not found\n";
        }
        pvName->put(value);
        PVUnionPtr pvUnion = pvStructure->getSubField<PVUnion>("argument.union");
        if(!pvUnion) {
             cout << "argument.union not found\n";
        }
        FieldCreatePtr fieldCreate = getFieldCreate();
        PVDataCreatePtr pvDataCreate = getPVDataCreate();
        StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
            add("timeStamp",getStandardField()->timeStamp()) ->
            addArray("value",pvDouble) ->
            createStructure();
        PVStructurePtr pvStruct = pvDataCreate->createPVStructure(topStructure);
        pvUnion->set(pvStruct);
        putData->getChangedBitSet()->set(pvUnion->getFieldOffset());
        pvaClientPutGet->putGet();
    }

    void getPut()
    {
        if(!channelConnected) {
            cout << channelName << " channel not connected\n";
            return;
        }
        if(!putGetConnected) {
            cout << channelName << " channelPutGet not connected\n";
            return;
        }
        pvaClientPutGet->issueGetPut();
    }

    void getGet()
    {
        if(!channelConnected) {
            cout << channelName << " channel not connected\n";
            return;
        }
        if(!putGetConnected) {
            cout << channelName << " channelPutGet not connected\n";
            return;
        }
        pvaClientPutGet->issueGetGet();
    }

};


int main(int argc,char *argv[])
{
    string provider("pva");
    string channelName("PVRaddRecord");
    string request("putField(argument)getField(result)");
    string debugString;
    bool debug(false);
    int opt;
    while((opt = getopt(argc, argv, "hp:r:d:")) != -1) {
        switch(opt) {
            case 'p':
                provider = optarg;
                break;
            case 'r':
                request = optarg;
                break;
            case 'h':
             cout << " -h -p provider -r request - d debug channelNames " << endl;
             cout << "default" << endl;
             cout << "-p " << provider 
                  << " -r " << request
                  << " -d " << (debug ? "true" : "false") 
                  << " " <<  channelName
                  << endl;           
                return 0;
            case 'd' :
               debugString =  optarg;
               if(debugString=="true") debug = true;
               break;
            default:
                std::cerr<<"Unknown argument: "<<opt<<"\n";
                return -1;
        }
    }
    bool pvaSrv(((provider.find("pva")==string::npos) ? false : true));
    bool caSrv(((provider.find("ca")==string::npos) ? false : true));
    if(pvaSrv&&caSrv) {
        cerr<< "multiple providers are not allowed\n";
        return 1;
    }
    cout << "provider " << provider
         << " channelName " <<  channelName
         << " request " << request
         << " debug " << (debug ? "true" : "false")  << endl;

    cout << "_____addRecord starting__\n";
    
    try {   
        if(debug) PvaClient::setDebug(true);
        int nPvs = argc - optind;       /* Remaining arg list are PV names */
        if (nPvs>0)
        {
            channelName = argv[optind];
        }
        PvaClientPtr pva= PvaClient::get(provider);
        ClientAddRecordPtr clientAddRecord(ClientAddRecord::create(pva,channelName,provider,request));
        while(true) {
            cout << "enter one of: exit putGet getPut getGet\n";
            int c = std::cin.peek();  // peek character
            if ( c == EOF ) continue;
            string str;
            getline(cin,str);
            if(str.compare("exit")==0) break;
            if(str.compare("putGet")==0) {
                 cout << "enter value\n";
                 getline(cin,str);
                 try {
                     clientAddRecord->putGet(str);
                } catch (std::exception& e) {
                     cerr << "exception " << e.what() << endl;
                }
                continue;
            }
            if(str.compare("getPut")==0) {
                try {
                    clientAddRecord->getPut();
                } catch (std::exception& e) {
                    cerr << "exception " << e.what() << endl;
                }
                continue;
            }
            if(str.compare("getGet")==0) {
                 try {
                    clientAddRecord->getGet();
                } catch (std::exception& e) {
                    cerr << "exception " << e.what() << endl;
                }
                 continue;
            }
        }
    } catch (std::exception& e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
