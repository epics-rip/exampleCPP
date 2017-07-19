/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */
#include <iostream>

#include <pv/pvaClient.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

class ClientGet;
typedef std::tr1::shared_ptr<ClientGet> ClientGetPtr;

class ClientGet :
    public PvaClientChannelStateChangeRequester
{
private:
    string request;
    PvaClientChannelPtr pvaClientChannel;
    PvaClientGetPtr pvaClientGet;
public:
    POINTER_DEFINITIONS(ClientGet);
    ClientGet(
        const string &request,
        const PvaClientChannelPtr & pvaClientChannel)
    : request(request),
      pvaClientChannel(pvaClientChannel)
    {
    }
    
    static ClientGetPtr create(
        PvaClientPtr const &pvaClient,
        const string & channelName,
        const string & providerName,
        const string  & request)
    {
        PvaClientChannelPtr pvaClientChannel = pvaClient->createChannel(channelName,providerName);
        ClientGetPtr clientGet(new ClientGet(request,pvaClientChannel));
        pvaClientChannel->setStateChangeRequester(clientGet);
        pvaClientChannel->issueConnect();
        return clientGet;
    }

    virtual void channelStateChange(PvaClientChannelPtr const & channel, bool isConnected)
    {
        if(isConnected&&!pvaClientGet)
        {
           pvaClientGet  = pvaClientChannel->createGet(request);
           pvaClientGet->issueConnect();
        }
    }

    PvaClientGetPtr getPvaClientGet()
    {
        return  pvaClientGet;
    }   
};


int main(int argc,char *argv[])
{
    string providerName("pva");
    string channelName("PVRdouble");
    string request("value,alarm,timeStamp");
    bool debug(false);
    if(argc==2 && string(argv[1])==string("-help")) {
        cout << "providerName channelName request debug" << endl;
        cout << "default" << endl;
        cout << providerName << " " <<  channelName
             << " " << '"' << request << '"'
             << " " << (debug ? "true" : "false") << endl;
        return 0;
    }
    if(argc>1) providerName = argv[1];
    if(argc>2) channelName = argv[2];
    if(argc>3) request = argv[3];
    if(argc>4) {
        string value(argv[4]);
        if(value=="true") debug = true;
    }
    bool pvaSrv(((providerName.find("pva")==string::npos) ? false : true));
    bool caSrv(((providerName.find("ca")==string::npos) ? false : true));
    if(pvaSrv&&caSrv) {
        cerr<< "multiple providerNames are not allowed\n";
        return 1;
    }
    cout << "providerName " << providerName
         << " pvaSrv " << (pvaSrv ? "true" : "false")
         << " caSrv " << (caSrv ? "true" : "false")
         << " channelName " <<  channelName
         << " request " << request
         << " debug " << (debug ? "true" : "false") << endl;

    cout << "_____getNoBlock starting__\n";
    
    try {   
        if(debug) PvaClient::setDebug(true);
        PvaClientPtr pva= PvaClient::get(providerName);
        ClientGetPtr clientGet(ClientGet::create(pva,channelName,providerName,request));
        while(true) {
            int c = std::cin.peek();  // peek character
            if ( c == EOF ) continue;
            cout << "Type exit to stop: \n";
            string str;
            getline(cin,str);
            if(str.compare("exit")==0) break;
            PvaClientGetPtr pvaClientGet = clientGet->getPvaClientGet();
            if(!pvaClientGet) {
                cout << "not connected\n";
            } else {
                pvaClientGet->get();
                PvaClientGetDataPtr data = pvaClientGet->getData();
                BitSetPtr bitSet =  data->getChangedBitSet();
                if(bitSet->cardinality()>0) {
                    cout << "changed\n";
                    data->showChanged(cout);
                    cout << "bitSet " << *bitSet << endl;
                }
            }
            
        }
    } catch (std::runtime_error e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
