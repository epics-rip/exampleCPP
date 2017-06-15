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
#include <pv/clientFactory.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

class ClientMonitor;
typedef std::tr1::shared_ptr<ClientMonitor> ClientMonitorPtr;

class ClientMonitor :
    public PvaClientChannelStateChangeRequester,
    public PvaClientMonitorRequester,
    public std::tr1::enable_shared_from_this<ClientMonitor>
{
private:
    PvaMonitorPtr pvaMonitor;
    bool isStarted;
public:
    POINTER_DEFINITIONS(ClientMonitor);

    static ClientMonitorPtr create(
        PvaClientPtr const &pvaClient,
        const string & channelName,
        const string & providerName,
        const string  & request)
    {
        ClientMonitorPtr clientMonitor(new ClientMonitor());
        clientMonitor->init(pvaClient,channelName,providerName,request);
        return clientMonitor;
    }

    ClientMonitor()
    : isStarted(false)
    {
    }
    void init(
        PvaClientPtr const &pvaClient,
        const string & channelName,
        const string & providerName,
        const string  & request)
    {
           pvaMonitor = PvaMonitor::create(pvaClient,channelName,providerName,request,
           shared_from_this(),shared_from_this());
           isStarted = true;
    }
    
    virtual void channelStateChange(PvaClientChannelPtr const & channel, bool isConnected )
    {
        cout << "channelStateChange isConnected " << (isConnected ? "true" : "false") << endl; 
    }
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
    }

    void status()
    {
        Channel::ConnectionState connectionState = pvaMonitor->getPvaClientChannel()->
             getChannel()->getConnectionState();
        cout << "connectionState " << Channel::ConnectionStateNames[connectionState] << endl;
    }

    void start()
    {
        if(pvaMonitor->getPvaClientChannel()->getChannel()->getConnectionState()
        !=Channel::ConnectionState::CONNECTED)
        {
             cout << "not connected\n";
             return;
        }
        if(isStarted)
        {
             cout << "already started\n";
             return;
        }
    
        PvaClientMonitorPtr clientMonitor = pvaMonitor->getPvaClientMonitor();
        if(!clientMonitor)
        {
             cout << "monitor no created\n";
             return;
        }
        clientMonitor->start();
        isStarted = true;
    }

    void stop()
    {
        if(pvaMonitor->getPvaClientChannel()->getChannel()->getConnectionState()
        !=Channel::ConnectionState::CONNECTED)
        {
             cout << "not connected\n";
             return;
        }
        if(!isStarted)
        {
             cout << "already stopped\n";
             return;
        }
    
        PvaClientMonitorPtr clientMonitor = pvaMonitor->getPvaClientMonitor();
        if(!clientMonitor)
        {
             cout << "monitor not created\n";
             return;
        }
        clientMonitor->stop();
        isStarted = false;
    }
};

typedef std::tr1::shared_ptr<ClientMonitor> ClientMonitorPtr;


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
    cout << "provider \"" << provider << "\""
         << " channelName " <<  channelName
         << " request " << request
         << " debug " << (debug ? "true" : "false") << endl;

    cout << "_____monitorChannel starting_______\n";
    cout << "Type help for help\n"; 
    try {
        if(debug) PvaClient::setDebug(true);
        PvaClientPtr pva = PvaClient::get(provider);
        ClientMonitorPtr clientMonitor = ClientMonitor::create(pva,channelName,provider,request);
        while(true) {
            string str;
            getline(cin,str);
            if(str.compare("help")==0){
                 cout << "Type help exit status start stop\n";
                 continue;
            }
            if(str.compare("status")==0){
                 clientMonitor->status();
                 continue;
            }
            if(str.compare("start")==0){
                 clientMonitor->start();
                 continue;
            }
            if(str.compare("stop")==0){
                 clientMonitor->stop();
                 continue;
            }
            if(str.compare("exit")==0){
                 break;
            }
        }
    } catch (std::runtime_error e) {
            cerr << "exception " << e.what() << endl;
            return 1;
    }
    return 0;
}
