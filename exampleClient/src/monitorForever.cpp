/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <epicsThread.h>

#include <iostream>

#include <pv/pvaClient.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

class  ChannelStateChangeRequester :
     public PvaClientChannelStateChangeRequester
{
    bool connected;
public:
    POINTER_DEFINITIONS(ChannelStateChangeRequester);
    ChannelStateChangeRequester()
    : connected(false)
     {}
    virtual void channelStateChange(PvaClientChannelPtr const & channel, bool isConnected )
    {
        cout << "channelStateChange state " << (isConnected ? "true" : "false") << endl; 
        connected = isConnected;
    }
    bool isConnected() {return connected;}
};


class ClientMonitorRequester :
   public PvaClientMonitorRequester
{
    bool unlistenCalled;
public:
    POINTER_DEFINITIONS(ClientMonitorRequester);
    ClientMonitorRequester()
    : unlistenCalled(false)
    {}
    virtual void event(PvaClientMonitorPtr const & monitor)
    {
        while(monitor->poll()) {
            PvaClientMonitorDataPtr monitorData = monitor->getData();
            cout << "monitor " << endl;
            cout << "changed\n";
            monitorData->showChanged(cout);
            cout << "overrun\n";
            monitorData->showOverrun(cout);
            monitor->releaseEvent();
        }
    }
    virtual void unlisten()
    {
         cout << "ClientMonitorRequester::unlisten\n";
         unlistenCalled = true;
    }
    bool isUnlisten() {return unlistenCalled;}
};

int main(int argc,char *argv[])
{
    string provider("pva");
    string channelName("PVRdouble");
    string request("value,alarm,timeStamp");
    bool debug(false);
    if(argc==2 && string(argv[1])==string("-help")) {
        cout << "provider channelName request debug" << endl;
        cout << "default" << endl;
        cout << provider << " " <<  channelName
             << " " << '"' << request << '"'
             << " " << (debug ? "true" : "false")
             << endl;
        return 0;
    }
    if(argc>1) provider = argv[1];
    if(argc>2) channelName = argv[2];
    if(argc>3) request = argv[3];
    if(argc>4) {
        string value(argv[4]);
        if(value=="true") debug = true;
    }
    bool pvaSrv(((provider.find("pva")==string::npos) ? false : true));
    bool caSrv(((provider.find("ca")==string::npos) ? false : true));
    if(pvaSrv&&caSrv) {
        cerr<< "multiple providers are not allowed\n";
        return 1;
    }
    cout << "provider \"" << provider << "\""
         << " pvaSrv " << (pvaSrv ? "true" : "false")
         << " caSrv " << (caSrv ? "true" : "false")
         << " channelName " <<  channelName
         << " request " << request
         << " debug " << (debug ? "true" : "false") << endl;

    cout << "_____monitorForever starting_______\n";
    PvaClientPtr pva = PvaClient::get(provider);
    try {
        if(debug) PvaClient::setDebug(true);
        PvaClientChannelPtr channel;
        ChannelStateChangeRequester::shared_pointer stateChangeRequester(
            new ChannelStateChangeRequester());
        ClientMonitorRequester::shared_pointer monitorRequester(new ClientMonitorRequester());
        PvaClientMonitorPtr monitor;
        while(true) {
            if(!channel) {
                 channel = pva->createChannel(channelName,provider);
                 channel->setStateChangeRequester(stateChangeRequester);
                 channel->issueConnect();
            }
            if(stateChangeRequester->isConnected()) {
                if(!monitor) {
                      monitor = channel->monitor(request,monitorRequester);
                      epicsThreadSleep(1.0);
                      continue;
                }
            } else {
                cout <<"channel not connected\n";
            }
            cout << "Type exit to stop: \n";
            string str;
            getline(cin,str);
            if(monitorRequester->isUnlisten()) {
                cout << "exiting because unlisten was called\n";
                break;
            }
            if(str.compare("exit")==0){
                 break;
            }
            if(str.compare("disconnect")==0){
                 channel.reset();
                 monitor.reset();
            }
        }
    } catch (std::runtime_error e) {
            cerr << "exception " << e.what() << endl;
            return 1;
    }
cout << "before epicsThreadSleep\n";
pva.reset();
epicsThreadSleep(1.0);
cout << "after epicsThreadSleep\n";
    return 0;
}
