/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2019.04
 */

/* Author: Marty Kraimer */
#include <iostream>
#include <string>
#include <epicsGetopt.h>
#include <pv/pvaClient.h>
#include <pv/convert.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

static void showHelp(const string & provider,const string & request,const string & channelName,bool debug)
{
    cout << " -h -p provider -r request - d debug channelName value" << endl;
    cout << "default" << endl;
    cout << "-p " << provider 
    << " -r " << request
    << " -d " << (debug ? "true" : "false") 
    << " " <<  channelName
    << endl;            
}

int main(int argc,char *argv[])
{
    string provider("pva");
    string channelName("<null>");
    string request("value");
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
                showHelp(provider,request,channelName,debug);
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
        int nPvs = argc - optind;
    if(nPvs<2)
    {
        cout << "must enter channelName and value\n";
        showHelp(provider,request,channelName,debug);
        return 1;
    }
    channelName = argv[optind];
    if(debug) PvaClient::setDebug(true);
    PvaClientPtr pva= PvaClient::get(provider);
    try {
        double value = atof(argv[optind+1]);
        PvaClientChannelPtr channel = pva->channel(channelName,provider,2.0);
        PvaClientPutPtr clientPut = channel->put(request);
        PvaClientPutDataPtr putData = clientPut->getData();
        putData->putDouble(value); clientPut->put();
        clientPut->get();
        double result = clientPut->getData()->getDouble();
        cout << "result " << result << "\n";
        pva->channel(channelName,provider,2.0)->putDouble(value + 1,request);
        cout << "result short way " << pva->channel(channelName,provider,2.0)->getDouble(request) << "\n";
    } catch (std::exception& e) {
         cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
