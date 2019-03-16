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

static PVDataCreatePtr pvDataCreate = getPVDataCreate();
static ConvertPtr convert = getConvert();

class ClientPut;
typedef std::tr1::shared_ptr<ClientPut> ClientPutPtr;

class ClientPut :
    public PvaClientChannelStateChangeRequester,
    public PvaClientPutRequester,
    public std::tr1::enable_shared_from_this<ClientPut>
{
private:
    string channelName;
    string providerName;
    string request;
    bool channelConnected;
    bool putConnected;

    PvaClientChannelPtr pvaClientChannel;
    PvaClientPutPtr pvaClientPut;

    void init(PvaClientPtr const &pvaClient)
    {
        pvaClientChannel = pvaClient->createChannel(channelName,providerName);
        pvaClientChannel->setStateChangeRequester(shared_from_this());
        pvaClientChannel->issueConnect();
    }
    vector<string> split(string const & blankSeparatedList)
    {
        string::size_type numValues = 1;
        string::size_type index=0;
        while(true) {
            string::size_type pos = blankSeparatedList.find(' ',index);
            if(pos==string::npos) break;
            numValues++;
            index = pos +1;
	}
        vector<string> valueList(numValues,"");
        index=0;
        for(size_t i=0; i<numValues; i++) {
            size_t pos = blankSeparatedList.find(' ',index);
            string value = blankSeparatedList.substr(index,pos-index);
            valueList[i] = value;
            index = pos +1;
        }
        return valueList;
    }
public:
    POINTER_DEFINITIONS(ClientPut);
    ClientPut(
        const string &channelName,
        const string &providerName,
        const string &request)
    : channelName(channelName),
      providerName(providerName),
      request(request),
      channelConnected(false),
      putConnected(false)
    {
    }
    
    static ClientPutPtr create(
        PvaClientPtr const &pvaClient,
        const string & channelName,
        const string & providerName,
        const string  & request)
    {
        ClientPutPtr client(ClientPutPtr(
             new ClientPut(channelName,providerName,request)));
        client->init(pvaClient);
        return client;
    }

    virtual void channelStateChange(PvaClientChannelPtr const & channel, bool isConnected)
    {
        channelConnected = isConnected;
        if(isConnected) {
            if(!pvaClientPut) {
                pvaClientPut = pvaClientChannel->createPut(request);
                pvaClientPut->setRequester(shared_from_this());
                pvaClientPut->issueConnect();
            }
        }
    }

    virtual void channelPutConnect(
        const epics::pvData::Status& status,
        PvaClientPutPtr const & clientPut)
    {
        putConnected = true;
        cout << "channelPutConnect " << channelName << " status " << status << endl;
    }

    
    virtual void putDone(
        const epics::pvData::Status& status,
        PvaClientPutPtr const & clientPut)
    {
         cout << "putDone " << channelName << " status " << status << endl;
    }

    void put(const string & value)
    {
        if(!channelConnected) {
            cout << channelName << " channel not connected\n";
            return;
        }
        if(!putConnected) {
            cout << channelName << " channelPut not connected\n";
            return;
        }
        
        cout << "value " << value << endl;
        PvaClientPutDataPtr putData = pvaClientPut->getData();
        PVFieldPtr pvField = putData->getPVStructure()->getPVFields()[0];
        if(!pvField) {
             throw std::runtime_error("no value field");
        }
        if(pvField->getField()->getType()!=union_) {
             throw std::runtime_error("value is not a PVUnion");
        }
        PVUnionPtr pvUnion = std::tr1::static_pointer_cast<PVUnion>(pvField);
        UnionConstPtr u = pvUnion->getUnion();
        vector<string> items = split(value);
        int nitems = items.size();
        bool isArray = (nitems==1) ? false : true;
        if(isArray) {
            if(u->isVariant()) {
               PVStringArrayPtr pvStringArray = 
                    pvDataCreate->createPVScalarArray<PVStringArray>();
               convert->fromStringArray(pvStringArray,0,nitems,items,0);
               pvUnion->set(pvStringArray);
           } else {
               PVStringArrayPtr pvStringArray = 
                    pvUnion->select<PVStringArray>("stringArray");
               convert->fromStringArray(pvStringArray,0,nitems,items,0);
           }
        } else {
            if(u->isVariant()) {
                PVStringPtr pvString = pvDataCreate->createPVScalar<PVString>();
                pvString->put(value);
                pvUnion->set(pvString);
            } else {
                PVStringPtr pvString = pvUnion->select<PVString>("string");
                pvString->put(value);
            }
        }
        putData->getChangedBitSet()->set(pvUnion->getFieldOffset());
        pvaClientPut->put();
    }
};



int main(int argc,char *argv[])
{
    string provider("pva");
    string channelName("PVRrestrictedUnion");
    string request("value");
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

    cout << "_____putUnion starting__\n";
    try {   
        if(debug) PvaClient::setDebug(true);
        vector<string> channelNames;
        vector<ClientPutPtr> ClientPuts;
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
            ClientPuts.push_back(ClientPut::create(pva,channelNames[i],provider,request));
        }
        while(true) {
            cout << "Type exit to stop: \n";
            int c = std::cin.peek();  // peek character
            if ( c == EOF ) continue;
            string str;
            getline(cin,str);
            if(str.compare("exit")==0) break;
            for(int i=0; i<nPvs; ++i) {
                try {
                    ClientPuts[i]->put(str);
                } catch (std::exception& e) {
                   cerr << "exception " << e.what() << endl;
                }
            }
        }
    } catch (std::exception& e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
