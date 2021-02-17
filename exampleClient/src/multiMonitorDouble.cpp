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

class MultiMonitorDouble;
typedef std::tr1::shared_ptr<MultiMonitorDouble> MultiMonitorDoublePtr;

class MultiMonitorDouble :
    public std::tr1::enable_shared_from_this<MultiMonitorDouble>
{
private:
    PvaClientMultiChannelPtr multiChannel;
    PvaClientMultiMonitorDoublePtr multiMonitorDouble;
    shared_vector<const std::string> const channelNames;

    MultiMonitorDouble(
       shared_vector<const string> const &channelNames
     )
    : 
      channelNames(channelNames)
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
        multiMonitorDouble = multiChannel->createMonitor();
        multiMonitorDouble->connect();
    }
public:     
    static MultiMonitorDoublePtr create(
        PvaClientPtr const &pva,
        string const & provider,
        shared_vector<const string> const &channelNames
    )
    {
        
        MultiMonitorDoublePtr clientMonitorDouble(MultiMonitorDoublePtr(new MultiMonitorDouble(channelNames)));
        clientMonitorDouble->init(pva,provider);
        return clientMonitorDouble;
    }
    void poll()
    {
        bool result = multiMonitorDouble->waitEvent(.1);
        while(result) {
            epics::pvData::shared_vector<double> data(multiMonitorDouble->get());
            cout << "value=" << data << "\n";
            result = multiMonitorDouble->poll();
        }
    }
};


int main(int argc,char *argv[])
{
    string provider("pva");
    shared_vector<string> channelNames;
    channelNames.push_back("PVRbyte");
    channelNames.push_back("PVRshort");
    channelNames.push_back("PVRint");
    channelNames.push_back("PVRlong");
    channelNames.push_back("PVRubyte");
    channelNames.push_back("PVRushort");
    channelNames.push_back("PVRuint");
    channelNames.push_back("PVRulong");
    channelNames.push_back("PVRfloat");
    channelNames.push_back("PVRdouble");
    string debugString;
    bool debug(false);
    int opt;
    while((opt = getopt(argc, argv, "hp:d:")) != -1) {
        switch(opt) {
            case 'h':
             cout << " -h -p provider -d debug  channelNames " << endl;
             cout << "default" << endl;
             cout << " -d " 
                  << " " <<  channelNames
                  << endl;           
                return 0;
            case 'p':
                provider = optarg;
                break;  
            case 'd' :
               debugString =  optarg;
               if(debugString=="true") debug = true;
               break;
            default:
                std::cerr<<"Unknown argument: "<<opt<<"\n";
                return -1;
        }
    }
    cout << "_____multiMonitorDouble starting_______\n";
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
        PvaClientPtr pva= PvaClient::get("pva");
        shared_vector<const string> names(freeze(channelNames));
        MultiMonitorDoublePtr clientMonitorDouble(MultiMonitorDouble::create(pva,provider,names));
        while(true) {
            cout << "Type exit or enter to poll\n";
            string str;
            getline(cin,str);
            if(str.compare("exit")==0) break;
            clientMonitorDouble->poll();
        }
        cout << "_____multiMonitorDouble done_______\n";
    } catch (std::exception& e) {
        cout << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
