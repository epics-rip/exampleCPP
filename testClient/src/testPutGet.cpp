/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author Sinisa Veseli and Marty Kraimer
 */

#include <iostream>
#include <epicsGetopt.h>
#include <unistd.h>
#include <pv/pvaClient.h>
#include <pv/convert.h>

using std::tr1::static_pointer_cast;
using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

static PVDataCreatePtr pvDataCreate = getPVDataCreate();
static ConvertPtr convert = getConvert();

static void example(
     PvaClientPtr const &pvaClient,
     string provider,
     shared_vector<const string> const &channelNames)
{
    size_t num = channelNames.size();
    shared_vector<PvaClientChannelPtr> pvaClientChannels(num);
    shared_vector<PvaClientPutPtr> pvaClientPuts(num);
    shared_vector<PvaClientGetPtr> pvaClientGets(num);
    for(size_t i=0; i<num; i++) {
        pvaClientChannels[i] = pvaClient->createChannel(channelNames[i],provider);
        pvaClientChannels[i]->issueConnect();
    }
    for(size_t i=0; i<num; i++) {
        Status status = pvaClientChannels[i]->waitConnect(5.0);
        if(!status.isOK()) {
            cout << "channel=" << channelNames[i] << " connect failed " << status <<"\n";
            return;
        }
        pvaClientPuts[i] = pvaClientChannels[i]->createPut("value");
        pvaClientPuts[i]->issueConnect();
        pvaClientGets[i] = pvaClientChannels[i]->createGet("value");
        pvaClientGets[i]->issueConnect();
    }
    for(size_t i=0; i<num; i++) {
        Status status = pvaClientPuts[i]->waitConnect();
        if(!status.isOK()) {
            cout << "channel=" << channelNames[i] << " put connect failed " << status <<"\n";
            return;
        }
        status = pvaClientGets[i]->waitConnect();
        if(!status.isOK()) {
            cout << "channel=" << channelNames[i] << " get connect failed " << status <<"\n";
            return;
        }
    }
    int successPutCount = 0;
    int failedPutCount = 0;
    for(int value = 0; value< 10000; value+= 1) {
        string strValue = to_string(value);
        for(size_t i=0; i<num; i++) {
            PvaClientPutDataPtr putData = pvaClientPuts[i]->getData();
            PVStructurePtr pvStructure = putData->getPVStructure();
            PVScalarPtr pvScalar(pvStructure->getSubField<PVScalar>("value"));
            convert->fromString(pvScalar,strValue);
            pvaClientPuts[i]->issuePut();
        }
        for(size_t i=0; i<num; i++) {
            Status status = pvaClientPuts[i]->waitPut();
            if(!status.isOK()) {
                cout << "channel=" << channelNames[i] << " put failed " << status <<"\n";
                return;
            }
        }
        for(size_t i=0; i<num; i++) {
            pvaClientGets[i]->issueGet();
        }
        bool correctData = true;
        for(size_t i=0; i<num ; ++i) {
            Status status = pvaClientGets[i]->waitGet();
            if(!status.isOK()) {
                cout << "channel=" << channelNames[i] << " get failed " << status <<"\n";
                return;
            }
            PvaClientGetDataPtr data = pvaClientGets[i]->getData();
            PVScalarPtr pvScalar = data->getPVStructure()->getSubField<PVScalar>("value");
            string getValue = convert->toString(pvScalar);
            if(strValue!=getValue){
                 cout << channelNames[i] << " expected=" << strValue << " got=" << getValue << "\n";
                 correctData = false;
            }  
        }
        if(correctData) {
            successPutCount++;
        }
        else {
            failedPutCount++;
        }
    }
    cout << "SUCCESS PUT COUNT: " << successPutCount << endl;
    cout << "FAILED  PUT COUNT: " << failedPutCount << endl;
}

int main(int argc,char *argv[])
{
    string provider("pva");
    shared_vector<string> channelNames;
    channelNames.push_back("PVRshort");
    channelNames.push_back("PVRint");
    channelNames.push_back("PVRlong");
    channelNames.push_back("PVRdouble");
    int opt;
    while((opt = getopt(argc, argv, "hp:")) != -1) {
        switch(opt) {
            case 'p':
                provider = optarg;
                break;
            case 'h':
             cout << " -h -p provider channelNames " << endl;
             cout << "default" << endl;
             cout << "-p " << provider 
                  << " " <<  channelNames
                  << endl;           
                return 0;
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
    cout << "_____testNTMultiPutGet starting_______\n";
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
        example(pva,provider,names);
        cout << "_____testNTMultiPutGet done_______\n";
    } catch (std::exception& e) {
        cout << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
