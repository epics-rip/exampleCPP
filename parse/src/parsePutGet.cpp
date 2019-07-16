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

int main(int argc,char *argv[])
{
    string provider("pva");
    string request("putField()getField()");
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
             cout << " -h -p provider -r request - d debug channelName args " << endl;
             cout << "default" << endl;
             cout << "-p " << provider 
                  << " -r " << request
                  << " -d " << (debug ? "true" : "false") 
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
    bool pvaSrv(((provider.find("pva")==string::npos) ? false : true));
    bool caSrv(((provider.find("ca")==string::npos) ? false : true));
    if(pvaSrv&&caSrv) {
        cerr<< "multiple providers are not allowed\n";
        return 1;
    }
    cout << "provider " << provider
         << " request " << request
         << " debug " << (debug ? "true" : "false")  << endl;

    cout << "_____put starting__\n";
    
    try {   
        if(debug) PvaClient::setDebug(true);
        int nPvs = argc - optind;
        if(nPvs<2)
        {
             throw std::runtime_error("must provide channelName and at lease one argument");
        }
        string channelName(argv[optind++]);
        vector<string> args;
        for (int n = 0; optind < argc; n++, optind++) args.push_back(argv[optind]);
        PvaClientPtr pva= PvaClient::get(provider);
        try {
             PvaClientChannelPtr channel = pva->channel(channelName,provider,2.0);
             PvaClientPutGetPtr putGet = channel->createPutGet(request);
             putGet->connect();
             PvaClientPutDataPtr putData(putGet->getPutData());
             putData->parse(args);
             putGet->putGet();
             PvaClientGetDataPtr getData = putGet->getGetData();
             cout << getData->getPVStructure() << endl;
        } catch (std::exception& e) {
                cerr << "exception " << e.what() << endl;
        }
    } catch (std::exception& e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
