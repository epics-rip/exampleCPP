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
#include <pv/pvaClientMultiChannel.h>
#include <pv/convert.h>

using std::tr1::static_pointer_cast;
using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

class MyMonitor;
typedef std::tr1::shared_ptr<MyMonitor> MyMonitorPtr;

class MyMonitor :
    public std::tr1::enable_shared_from_this<MyMonitor>
{
private:
    PvaClientMultiChannelPtr multiChannel;
    PvaClientNTMultiMonitorPtr multiMonitor;
    epics::pvData::shared_vector<const std::string> const channelNames;
    string request;
    bool valueOnly;

    MyMonitor(
       shared_vector<const string> const &channelNames,
       string const & request,
       bool valueOnly
     )
    : 
      channelNames(channelNames),
      request(request),
      valueOnly(valueOnly)
    {}

    void init(PvaClientPtr const &pva,string const & provider)
    {
        multiChannel = PvaClientMultiChannel::create(pva,channelNames,provider);
        Status status = multiChannel->connect();     
        if(!status.isSuccess()) {
             cout << "Did not connect: ";
             shared_vector<epics::pvData::boolean> isConnected = multiChannel->getIsConnected();
             size_t num = channelNames.size();
             for(size_t i=0; i<num; ++i) {
                 if(!isConnected[i]) cout << channelNames[i] << " ";
             }
             cout << endl;
        }
        multiMonitor = multiChannel->createNTMonitor(request);
        multiMonitor->connect();
    }
public:
    static MyMonitorPtr create(
        PvaClientPtr const &pva,
        string const & provider,
        shared_vector<const string> const &channelNames,
        string const & request,
        bool valueOnly
        )
    {
        
        MyMonitorPtr clientMonitor(MyMonitorPtr(new MyMonitor(channelNames,request,valueOnly)));
        clientMonitor->init(pva,provider);
        return clientMonitor;
    }
    void poll()
    {
        if(!multiMonitor->poll(valueOnly)) {
           cout << "no new data\n";
           return;
        }
        PvaClientNTMultiDataPtr multiData = multiMonitor->getData();
        PVStructurePtr pvStructure = multiData->getNTMultiChannel()->getPVStructure();
        PVUnionArrayPtr pvUnionArray = static_pointer_cast<PVUnionArray>(
             pvStructure->getSubField("value"));
        shared_vector<const PVUnionPtr> values = pvUnionArray->view();
#ifndef pvaClientMultiChannel_changes
        for(size_t ind=0; ind < values.size(); ++ind)
        {
            PVUnionPtr pvUnion = values[ind];
            if(pvUnion) {
                PVFieldPtr pvField = pvUnion->get();
                cout << channelNames[ind] << " = " << pvField << "\n";
            } else  {
                cout << channelNames[ind] << " no monitor event\n";
            }
        }
#else
        shared_vector<epics::pvData::boolean> changeFlags(multiData->getChannelChangeFlags());
        shared_vector<epics::pvData::boolean> isConnected(multiChannel->getIsConnected());
        for(size_t ind=0; ind < values.size(); ++ind)
        {
            if(changeFlags[ind]) {
                PVUnionPtr pvUnion = values[ind];
                PVFieldPtr pvField = pvUnion->get();
                cout << channelNames[ind] << " = " << pvField << "\n";
            } else  {
                string connected("false");
                if(isConnected[ind]) connected = "true";
                cout << channelNames[ind] << " no monitor event; isConnected=" << connected << "\n";
            }
        }
#endif        
    }         
};


int main(int argc,char *argv[])
{
    string provider("pva");
    shared_vector<string> channelNames;
    channelNames.push_back("PVRbyte");
    channelNames.push_back("PVRshortArray");
    channelNames.push_back("PVRint");
    channelNames.push_back("PVRlongArray");
    channelNames.push_back("PVRubyte");
    channelNames.push_back("PVRushortArray");
    channelNames.push_back("PVRdouble");
    channelNames.push_back("PVRdoubleArray");
    string request("value,alarm,timeStamp");
    bool debug(false);
    bool valueOnly(false);
    int opt;
    while((opt = getopt(argc, argv, "hp:r:v:d:")) != -1) {
        switch(opt) {
            case 'p':
                provider = optarg;
                break;
            case 'r':
                request = optarg;
                break;
            case 'h':
             cout << " -h -p provider -r request -v valueOnly - d debug channelNames " << endl;
             cout << "default" << endl;
             cout << "-p " << provider 
                  << " -r " << request
                  << " -v " << (valueOnly ? "true" : "false")
                  << " -d " << (debug ? "true" : "false")
                  << " " <<  channelNames
                  << endl;           
                return 0;
           case 'v' :
               if(string(optarg)=="true") valueOnly = true;
               break;
            case 'd' :
               if(string(optarg)=="true") debug = true;
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
    cout << "_____ntMultiMonitor starting_______\n";
    if(debug) PvaClient::setDebug(true);
    try {
        int nPvs = argc - optind;       /* Remaining arg list are PV names */      
        if (nPvs!=0)
        {
            channelNames.clear();
            while(optind < argc) {
                channelNames.push_back(argv[optind]);
                optind++;
            }
        }
        cout << " channelNames " <<  channelNames << endl;
        PvaClientPtr pva= PvaClient::get(provider);
        shared_vector<const string> names(freeze(channelNames));
        MyMonitorPtr clientMonitor(MyMonitor::create(pva,provider,names,request,valueOnly));
        while(true) {
            cout << "Type exit or enter to poll\n";
            string str;
            getline(cin,str);
            if(str.compare("exit")==0) break;
            clientMonitor->poll();
        }
        cout << "_____ntMultiMonitor done_______\n";
    } catch (std::exception& e) {
        cout << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
