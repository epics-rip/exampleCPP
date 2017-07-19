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

class ClientMonitor;
typedef std::tr1::shared_ptr<ClientMonitor> ClientMonitorPtr;

class ClientMonitor :
    public PvaClientMonitorRequester,
    public std::tr1::enable_shared_from_this<ClientMonitor>
{
private:
    PvaClientMonitorPtr pvaClientMonitor;
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
    {
    }
    void init(
        PvaClientPtr const &pvaClient,
        const string & channelName,
        const string & providerName,
        const string  & request)
    {
           pvaClientMonitor = PvaClientMonitor::create(pvaClient,channelName,providerName,request,
           PvaClientChannelStateChangeRequesterPtr(),shared_from_this());
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
    PvaClientMonitorPtr getPvaClientMonitor() {
        return pvaClientMonitor;
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

    cout << "_____monitorNoBlock starting_______\n";
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
            if(str.compare("start")==0){
                 clientMonitor->getPvaClientMonitor()->start(request);
                 continue;
            }
            if(str.compare("stop")==0){
                 clientMonitor->getPvaClientMonitor()->stop();
                 continue;
            }
            if(str.compare("exit")==0){
                 break;
            }
            bool isConnected = clientMonitor->
                getPvaClientMonitor()->getPvaClientChannel()->getChannel()->isConnected();
            cout << "isConnected " << (isConnected ? "true" : "false") << endl;
        }
    } catch (std::runtime_error e) {
            cerr << "exception " << e.what() << endl;
            return 1;
    }
    return 0;
}
