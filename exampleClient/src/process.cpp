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

class ClientProcess;
typedef std::tr1::shared_ptr<ClientProcess> ClientProcessPtr;

class ClientProcess :
    public PvaClientChannelStateChangeRequester,
    public PvaClientProcessRequester,
    public std::tr1::enable_shared_from_this<ClientProcess>
{
private:
    string channelName;
    string provider;
    bool channelConnected;
    bool processConnected;

    PvaClientChannelPtr pvaClientChannel;
    PvaClientProcessPtr pvaClientProcess;

    void init(PvaClientPtr const &pvaClient)
    {
        pvaClientChannel = pvaClient->createChannel(channelName,provider);
        pvaClientChannel->setStateChangeRequester(shared_from_this());
        pvaClientChannel->issueConnect();
    }
public:
    POINTER_DEFINITIONS(ClientProcess);
    ClientProcess(
        const string &channelName,
        const string &provider)
    : channelName(channelName),
      provider(provider),
      channelConnected(false),
      processConnected(false)
    {
    }
    
    static ClientProcessPtr create(
        PvaClientPtr const &pvaClient,
        const string & channelName,
        const string &provider)
    {
        ClientProcessPtr client(ClientProcessPtr(
             new ClientProcess(channelName,provider)));
        client->init(pvaClient);
        return client;
    }

    virtual void channelStateChange(PvaClientChannelPtr const & channel, bool isConnected)
    {
        channelConnected = isConnected;
        if(isConnected) {
            if(!pvaClientProcess) {
                pvaClientProcess = pvaClientChannel->createProcess();
                pvaClientProcess->setRequester(shared_from_this());
                pvaClientProcess->issueConnect();
            }
        }
    }

    virtual void channelProcessConnect(
        const epics::pvData::Status& status,
        PvaClientProcessPtr const & clientProcess)
    {
        processConnected = true;
        cout << "channelProcessConnect " << channelName << " status " << status << endl;
    }

    
    virtual void processDone(
        const epics::pvData::Status& status,
        PvaClientProcessPtr const & clientProcess)
    {
         cout << "processDone " << channelName << " status " << status << endl;
    }

    void process()
    {
        if(!channelConnected) {
            cout << channelName << " channel not connected\n";
            return;
        }
        if(!processConnected) {
            cout << channelName << " channelProcess not connected\n";
            return;
        }
        pvaClientProcess->process();
    }
};


int main(int argc,char *argv[])
{
    string channelName("PVRdouble");
    string debugString;
    bool debug(false);
    int opt;
    while((opt = getopt(argc, argv, "hd:")) != -1) {
        switch(opt) {
            case 'h':
             cout << " -h - d debug channelNames " << endl;
             cout << "default" << endl;
             cout << " -d " << (debug ? "true" : "false") 
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
    cout << " channelName " <<  channelName
         << " debug " << (debug ? "true" : "false")  << endl;

    cout << "_____process starting__\n";
    try {   
        if(debug) PvaClient::setDebug(true);
        vector<string> channelNames;
        vector<ClientProcessPtr> ClientProcesss;
        int nPvs = argc - optind;       /* Remaining arg list are PV names */
        if (nPvs==0)
        {
            channelNames.push_back(channelName);
            nPvs = 1;
        } else {
            for (int n = 0; optind < argc; n++, optind++) channelNames.push_back(argv[optind]);
        }
        PvaClientPtr pva= PvaClient::get("pva");
        for(int i=0; i<nPvs; ++i) {
            ClientProcesss.push_back(ClientProcess::create(pva,channelNames[i],"pva"));
        }
        while(true) {
            cout << "exit or enter\n";
            string str;
            getline(cin,str);
            if(str.compare("exit")==0) break;
            for(int i=0; i<nPvs; ++i) {
                try {
                     ClientProcesss[i]->process();
                } catch (std::exception& e) {
                   cerr << "exception " << e.what() << endl;
                }
            }
            continue;
        }
    } catch (std::exception& e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
