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
    string request("");
    string argString;
    bool zeroarray(true);
    bool debug(false);
    int opt;
    while((opt = getopt(argc, argv, "hp:r:z:d:")) != -1) {
        switch(opt) {
            case 'h':
             cout << " -h -p provider -r request  -z zeroarray - d debug channelName args " << endl;
             cout << "default" << endl;
             cout << "-p " << provider 
                  << " -r " << request
                  << " -z " << (zeroarray ? "true" : "false")
                  << " -d " << (debug ? "true" : "false") 
                  << endl;           
                return 0;
            case 'p':
                provider = optarg;
                break;
            case 'r':
                request = optarg;
                break;
            case 'z' :
               argString =  optarg;
               if(argString=="false") zeroarray = false;
               break;
            case 'd' :
               argString =  optarg;
               if(argString=="true") debug = true;
               break;
            default:
                std::cerr<<"Unknown argument: "<<opt<<"\n";
                return -1;
        }
    }
    try {   
        if(debug) PvaClient::setDebug(true);
        int nPvs = argc - optind;
        if(nPvs<2)
        {
             throw std::runtime_error("must provide channelName and at lease one argument");
        }
        string channelName(argv[optind++]);
        cout << "_____parsePut"
             << " channel " << channelName
             << " provider " << provider
             << " request " << request
             << " zeroarray " << (zeroarray ? "true" : "false")
             << " debug " << (debug ? "true" : "false")
             << "\n";
        vector<string> args;
        for (int n = 0; optind < argc; n++, optind++) args.push_back(argv[optind]);
        PvaClientPtr pva= PvaClient::get(provider);
        PvaClientChannelPtr channel = pva->channel(channelName,provider,2.0);
        PvaClientPutPtr put = channel->put(request);
        PvaClientPutDataPtr putData(put->getData());
        if(zeroarray) putData->zeroArrayLength();
        putData->getChangedBitSet()->clear();
        putData->parse(args);
        put->put();
    } catch (std::exception& e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
