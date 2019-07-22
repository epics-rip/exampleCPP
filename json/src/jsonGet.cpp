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
    bool debug(false);
    bool multiline(false);
    int opt;
    while((opt = getopt(argc, argv, "hp:r:m:d:")) != -1) {
        switch(opt) {
            case 'h':
             cout << " -h -p provider -r request -m multiline - d debug channelName " << endl;
             cout << "default" << endl;
             cout << "-p " << provider 
                  << " -r " << request
                  << " -m " << (multiline ? "true" : "false") 
                  << " -d " << (debug ? "true" : "false") 
                  << endl;           
                return 0;
            case 'p':
                provider = optarg;
                break;
            case 'r':
                request = optarg;
                break;
            case 'm' :
               argString =  optarg;
               if(argString=="true") multiline = true;
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
        if(nPvs<1)
        {
             throw std::runtime_error("must provide channelName");
        }
        string channelName(argv[optind++]);
        cout << "_____parsePut"
             << " channel" << channelName
             << " provider " << provider
             << " request " << request
             << " multiline " << (multiline ? "true" : "false")
             << " debug " << (debug ? "true" : "false")
             << "\n";
        PvaClientPtr pva(PvaClient::get(provider));
        PvaClientGetDataPtr pvData =
            pva->channel(channelName,provider,2.0)->get(request)->getData();
        std::ostringstream os;
        pvData->streamJSON(os,true,multiline);
        cout << os.str() << "\n";
    } catch (std::exception& e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
