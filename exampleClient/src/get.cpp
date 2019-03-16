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


using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

class ClientGet;
typedef std::tr1::shared_ptr<ClientGet> ClientGetPtr;

class ClientGet :
    public PvaClientChannelStateChangeRequester,
    public PvaClientGetRequester,
    public std::tr1::enable_shared_from_this<ClientGet>
{
private:
    string channelName;
    string provider;
    string request;
    bool channelConnected;
    bool getConnected;

    PvaClientChannelPtr pvaClientChannel;
    PvaClientGetPtr pvaClientGet;

    void init(PvaClientPtr const &pvaClient)
    {
        pvaClientChannel = pvaClient->createChannel(channelName,provider);
        pvaClientChannel->setStateChangeRequester(shared_from_this());
        pvaClientChannel->issueConnect();
    }

public:
    POINTER_DEFINITIONS(ClientGet);
    ClientGet(
        const string &channelName,
        const string &provider,
        const string &request)
    : channelName(channelName),
      provider(provider),
      request(request),
      channelConnected(false),
      getConnected(false)
    {
    }
    
    static ClientGetPtr create(
        PvaClientPtr const &pvaClient,
        const string & channelName,
        const string & provider,
        const string  & request)
    {
       ClientGetPtr client(ClientGetPtr(
             new ClientGet(channelName,provider,request)));
        client->init(pvaClient);
        return client;
    }

    virtual void channelStateChange(PvaClientChannelPtr const & channel, bool isConnected)
    {
        channelConnected = isConnected;
        if(isConnected) {
            if(!pvaClientGet) {
                pvaClientGet = pvaClientChannel->createGet(request);
                pvaClientGet->setRequester(shared_from_this());
                pvaClientGet->issueConnect();
            }
        }
    }

    virtual void channelGetConnect(
        const epics::pvData::Status& status,
        PvaClientGetPtr const & clientGet)
    {
         getConnected = true;
         cout << "channelGetConnect " << channelName << " status " << status << endl;
    }

    virtual void getDone(
        const epics::pvData::Status& status,
        PvaClientGetPtr const & clientGet)
    {
        cout << "channelGetDone " << channelName << " status " << status << endl;
    }

    void get()
    {
        
        if(!channelConnected) {
            cout << channelName << " channel not connected\n";
            return;
        }
        if(!getConnected) {
            cout << channelName << " channelGet not connected\n";
            return;
        }
        pvaClientGet->get();
        PvaClientGetDataPtr data = pvaClientGet->getData();
        BitSetPtr bitSet =  data->getChangedBitSet();
        if(bitSet->cardinality()>0) {
             cout << "changed " << channelName << endl;
             data->showChanged(cout);
             cout << "bitSet " << *bitSet << endl;
        }
    }
   
};


int main(int argc,char *argv[])
{
    string provider("pva");
    string channelName("PVRdouble");
    string request("value,alarm,timeStamp");
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

    cout << "_____get starting__\n";
    
    try {   
        if(debug) PvaClient::setDebug(true);
        vector<string> channelNames;
        vector<ClientGetPtr> clientGets;
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
            clientGets.push_back(ClientGet::create(pva,channelNames[i],provider,request));
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
                    clientGets[i]->get();
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
