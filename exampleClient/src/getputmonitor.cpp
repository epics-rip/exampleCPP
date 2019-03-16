/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <iostream>
#include <sstream>
#include <epicsGetopt.h>
#include <epicsGuard.h>
#include <pv/pvaClient.h>
#include <epicsThread.h>
#include <pv/event.h>
#include <pv/timeStamp.h>
#include <pv/convert.h>


using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

typedef epicsGuard<epicsMutex> Guard;
typedef epicsGuardRelease<epicsMutex> UnGuard;

class ClientGetPutMonitor;
typedef std::tr1::shared_ptr<ClientGetPutMonitor> ClientGetPutMonitorPtr;

class ClientGetPutMonitor :
    public PvaClientChannelStateChangeRequester,
    public PvaClientGetRequester,
    public PvaClientPutRequester,
    public PvaClientMonitorRequester,
    public std::tr1::enable_shared_from_this<ClientGetPutMonitor>
{
private:
    string channelName;
    string provider;
    string request;
    string putrequest;
    
    bool channelConnected;
    bool getConnected;
    bool putConnected;
    bool monitorConnected;
    bool isStarted;

    PvaClientChannelPtr pvaClientChannel;
    PvaClientGetPtr pvaClientGet;
    PvaClientPutPtr pvaClientPut;
    PvaClientMonitorPtr pvaClientMonitor;
    Event waitForCallback;  

    void init(PvaClientPtr const &pvaClient)
    {
        pvaClientChannel = pvaClient->createChannel(channelName,provider);
        pvaClientChannel->setStateChangeRequester(shared_from_this());
        pvaClientChannel->issueConnect();
    }
public:
    POINTER_DEFINITIONS(ClientGetPutMonitor);
    ClientGetPutMonitor(
        const string &channelName,
        const string &provider,
        const string &request,
        const string &putrequest)
    : channelName(channelName),
      provider(provider),
      request(request),
      putrequest(putrequest),
      channelConnected(false),
      getConnected(false),
      putConnected(false),
      monitorConnected(false),
      isStarted(false)
    {
    }
    ~ClientGetPutMonitor() {cout<< "~ClientGetPutMonitor() "<< channelName << "\n";}
    
    static ClientGetPutMonitorPtr create(
        PvaClientPtr const &pvaClient,
        const string & channelName,
        const string & provider,
        const string  & request,
        bool block)
    {
       string putrequest("field(value)");
       if(block) putrequest = "record[block=true]field(value)";
       ClientGetPutMonitorPtr client(ClientGetPutMonitorPtr(
             new ClientGetPutMonitor(channelName,provider,request,putrequest)));
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
            if(!pvaClientPut) {
                pvaClientPut = pvaClientChannel->createPut(request);
                pvaClientPut->setRequester(shared_from_this());
                pvaClientPut->issueConnect();
            }
            if(!pvaClientMonitor) {
                pvaClientMonitor = pvaClientChannel->createMonitor(request);
                pvaClientMonitor->setRequester(shared_from_this());
                pvaClientMonitor->issueConnect();
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
        try {
            pvaClientGet->get();
            PvaClientGetDataPtr data = pvaClientGet->getData();
            cout << "get " << channelName << "\n";
            BitSetPtr bitSet =  data->getChangedBitSet();
            if(bitSet->cardinality()>0) {
                cout << "changed " << channelName << endl;
                data->showChanged(cout);
                cout << "bitSet " << *bitSet << endl;
            }
        } catch (std::exception& e) {
            cerr << "exception " << e.what() << endl;
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
         waitForCallback.signal();
    }

    
    virtual void getDone(
        const epics::pvData::Status& status,
        PvaClientPutPtr const & clientPut)
    {
         cout << "getDone " << channelName << " status " << status << endl;
          if(status.isOK()) {
             cout << pvaClientPut->getData()->getPVStructure() << endl;
         } else {
             cout << "getDone " << channelName << " status " << status << endl;
         }
    }


    void put(const string & value)
    {
        cout << "put " << channelName << " value " << value << endl;
        if(!channelConnected) {
            cout << channelName << " channel not connected\n";
            return;
        }
        if(!putConnected) {
            cout << channelName << " channelPut not connected\n";
            return;
        }
        PvaClientPutDataPtr putData = pvaClientPut->getData();
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
            pvScalarArray->setLength(n);
            convert->fromStringArray(pvScalarArray,0,n,values,0);        
        }
        pvaClientPut->issuePut();
        waitForCallback.wait(); 
    }

    void getPut()
    {
        if(!channelConnected) {
            cout << channelName << " channel not connected\n";
            return;
        }
        if(!putConnected) {
            cout << channelName << " channelPut not connected\n";
            return;
        }
        pvaClientPut->issueGet();
    }

    virtual void monitorConnect(epics::pvData::Status const & status,
        PvaClientMonitorPtr const & monitor, epics::pvData::StructureConstPtr const & structure)
    {
        if(!status.isOK()) return;
        monitorConnected = true;
        if(isStarted) return;
        isStarted = true;
        pvaClientMonitor->start();
    }
    
    virtual void event(PvaClientMonitorPtr const & monitor)
    {
        while(monitor->poll()) {
            PvaClientMonitorDataPtr monitorData = monitor->getData();
            cout << "event " << "channelName " << channelName << "\n";
            monitorData->showChanged(cout);
            monitor->releaseEvent();
        }
    }

     void stop()
    {
         if(isStarted) {
             isStarted = false;
             pvaClientMonitor->stop();
         }
    }

    void start()
    {
         if(!channelConnected || !monitorConnected)
         {
              cout << "notconnected\n";
         }
         isStarted = true;
         pvaClientMonitor->start();
    }
};

int main(int argc,char *argv[])
{
    string argString("");
    string provider("pva");
    string channelName("PVRdouble");
    string request("value,alarm,timeStamp");
    bool debug(false);
    bool block(true);
    int opt;
    while((opt = getopt(argc, argv, "hp:r:d:s:o:b:")) != -1) {
        switch(opt) {
            case 'p':
                provider = optarg;
                break;
            case 'r':
                request = optarg;
                break;
            case 'h':
             cout << " -h -p provider -r request - d debug channelNames \n";
             cout << "default" << endl;
             cout << "-p " << provider 
                  << " -r " << request
                  << " -d " << (debug ? "true" : "false")
                  << " -b " << (block ? "true" : "false")
                  << " " <<  channelName
                  << endl;           
                return 0;
            case 'd' :
               argString = optarg;
               if(argString=="true") debug = true;
               break;
            case 'b' :
               argString = optarg;
               if(argString=="true") block = true;
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
         << " debug " << (debug ? "true" : "false")
         << " block " << (block ? "true" : "false")
         << endl;

    cout << "_____getputmonitorrate starting__\n";
    PvaClientPtr pva(PvaClient::get(provider));
    try {  
        if(debug) PvaClient::setDebug(true);
        vector<string> channelNames;
        vector<ClientGetPutMonitorPtr> clientGetPutMonitors;
        int nPvs = argc - optind;       /* Remaining arg list are PV names */
        if (nPvs==0)
        {
            channelNames.push_back(channelName);
            nPvs = 1;
        } else {
            for (int n = 0; optind < argc; n++, optind++) channelNames.push_back(argv[optind]);
        }
        for(int i=0; i<nPvs; ++i) {
            clientGetPutMonitors.push_back(
                ClientGetPutMonitor::create(pva,channelNames[i],provider,request,block));
        }
        while(true) {
            cout << "enter one of: exit get put start stop\n";
            string str;
            getline(cin,str);
            if(str.compare("get")==0){
                 for(size_t i=0; i< clientGetPutMonitors.size() ; ++i) clientGetPutMonitors[i]->get();
                 continue;
            }
            if(str.compare("put")==0){
                 cout << "enter value\n";
                 getline(cin,str);
                 for(size_t i=0; i< clientGetPutMonitors.size() ; ++i) clientGetPutMonitors[i]->put(str);
                 continue;
            }
            if(str.compare("start")==0){
                 for(size_t i=0; i< clientGetPutMonitors.size() ; ++i) clientGetPutMonitors[i]->start();
                 continue;
            }
            if(str.compare("stop")==0){
                 for(size_t i=0; i< clientGetPutMonitors.size() ; ++i) clientGetPutMonitors[i]->stop();
                 continue;
            }
            if(str.compare("exit")==0) break;
            cout << "illegal command\n";
        }
    } catch (std::exception& e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
