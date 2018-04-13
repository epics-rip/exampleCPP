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

class ClientPutGet;
typedef std::tr1::shared_ptr<ClientPutGet> ClientPutGetPtr;

class ClientPutGet :
    public PvaClientChannelStateChangeRequester,
    public PvaClientPutGetRequester,
    public std::tr1::enable_shared_from_this<ClientPutGet>
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
    POINTER_DEFINITIONS(ClientPutGet);
    ClientPutGet(
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
    
    static ClientPutGetPtr create(
        PvaClientPtr const &pvaClient,
        const string & channelName,
        const string & providerName,
        const string  & request)
    {
        ClientPutGetPtr client(ClientPutGetPtr(
             new ClientPutGet(channelName,providerName,request)));
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
        PvaClientPutGetPtr const & clientPutGet)
    {
        putGetConnected = true;
        cout << "channelPutGetConnect " << channelName << " status " << status << endl;
    }

    
    virtual void putGetDone(
        const epics::pvData::Status& status,
        PvaClientPutGetPtr const & clientPutGet)
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
            cout << channelName << " channelPutGet not connected\n";
            return;
        }
        PvaClientPutDataPtr putData = pvaClientPutGet->getPutData();
        PVStructurePtr pvStructure = putData->getPVStructure();
        PVScalarPtr pvScalar(pvStructure->getSubField<PVScalar>("value"));
        PVScalarArrayPtr pvScalarArray(pvStructure->getSubField<PVScalarArray>("value"));
        while(true) {
            if(pvScalar) break;
            if(pvScalarArray) break;
            PVFieldPtr pvField(pvStructure->getPVFields()[0]);
            pvScalar = std::tr1::dynamic_pointer_cast<PVScalar>(pvField);
            if(pvScalar) break;
            pvScalarArray = std::tr1::dynamic_pointer_cast<PVScalarArray>(pvField);
            if(pvScalarArray) break;
            pvStructure = std::tr1::dynamic_pointer_cast<PVStructure>(pvField);
            if(!pvStructure) {
               cout << channelName << " did not find a pvScalar field\n";
               return;
            }
        }
        ConvertPtr convert = getConvert();
        if(pvScalar) {
            convert->fromString(pvScalar,value);
        } else {
            vector<string> values;
            size_t pos = 0;
            size_t n = 1;
            while(true)
            {
                size_t offset = value.find(" ",pos);
                if(offset==string::npos) {
                    values.push_back(value.substr(pos));
                    break;
                }
                values.push_back(value.substr(pos,offset-pos));
                pos = offset+1;
                n++;    
            }
            convert->fromStringArray(pvScalarArray,0,n,values,0);        
        }
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
    string channelName("PVRhelloPutGet");
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
         << " debug " << (debug ? "true" : "false") << endl;

    cout << "_____put starting__\n";
    
    try {   
        if(debug) PvaClient::setDebug(true);
        vector<string> channelNames;
        vector<ClientPutGetPtr> ClientPutGets;
        int nPvs = argc - optind;       /* Remaining arg list are PV names */
        if (nPvs==0)
        {
            channelNames.push_back(channelName);
            nPvs = 1;
        } else {
            for (int n = 0; optind < argc; n++, optind++) channelNames.push_back(argv[optind]);
        }
        PvaClientPtr pva= PvaClient::get(provider);
        for(int i=0; i<nPvs; ++i) {
            ClientPutGets.push_back(ClientPutGet::create(pva,channelNames[i],provider,request));
        }
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
                 for(int i=0; i<nPvs; ++i) {
                    try {
                         ClientPutGets[i]->putGet(str);
                    } catch (std::runtime_error e) {
                       cerr << "exception " << e.what() << endl;
                    }
                 }
                 continue;
            }
            if(str.compare("getPut")==0) {
                 for(int i=0; i<nPvs; ++i) {
                    try {
                         ClientPutGets[i]->getPut();
                    } catch (std::runtime_error e) {
                       cerr << "exception " << e.what() << endl;
                    }
                 }
                 continue;
            }
            if(str.compare("getGet")==0) {
                 for(int i=0; i<nPvs; ++i) {
                    try {
                         ClientPutGets[i]->getGet();
                    } catch (std::runtime_error e) {
                       cerr << "exception " << e.what() << endl;
                    }
                 }
                 continue;
            }
        }
    } catch (std::runtime_error e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
