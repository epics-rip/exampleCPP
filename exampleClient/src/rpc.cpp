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
#include <epicsThread.h>
#include <pv/pvData.h>
#include <pv/ntscalar.h>
#include <pv/pvAccess.h>
#include <pv/rpcClient.h>
#include <pv/pvaClient.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::nt;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

class ClientRPCRequester :
   public PvaClientRPCRequester
{
private:
     bool requestDoneCalled;
public:
    POINTER_DEFINITIONS(ClientRPCRequester);

    ClientRPCRequester() : requestDoneCalled(false){}
    virtual void requestDone(
        const Status& status,
        PvaClientRPCPtr const & pvaClientRPC,
        PVStructurePtr const & pvResponse)
   {
       if(!status.isOK()) {
          cout << "status " << status.getMessage() << "\n";
       }
       cout << "response\n" << pvResponse << endl;
       requestDoneCalled = true;
   }
   void waitResponse()
   {
       while(true)
       {
            if(requestDoneCalled) {
                requestDoneCalled = false;
                return;
            }
            epicsThreadSleep(.1);
       }
   }
};


static void exampleSimple(PvaClientPtr const &pva,string const & channelName)
{
    cout << "_____exampleSimple___\n";
    NTScalarBuilderPtr ntScalarBuilder = NTScalar::createBuilder();
    PVStructurePtr pvRequest = ntScalarBuilder->
        value(pvString)->
        createPVStructure(); 
    PVStringPtr pvArgument = pvRequest->getSubField<PVString>("value");
    pvArgument->put("SimpleWorld");
    try {
        cout << "send " << pvArgument->get() << endl;
        PVStructurePtr pvResult = pva->channel(channelName)->rpc(pvRequest);
        cout << "result\n" << pvResult << endl;
    } catch (epics::pvAccess::RPCRequestException & ex){
        std::cerr << "Acquisition of greeting was not successful. RPCException:" << "\n";
        std::cerr << ex.what() << std::endl;
    } catch (std::exception& e) {
        // Catch any other exceptions so we always call ClientFactory::stop().
        std::cerr << "Acquisition of greeting was not successful. Unexpected exception." << "\n";
        std::cerr << e.what() << "\n";
    }
}

static void exampleMore(PvaClientPtr const &pva,string const & channelName)
{
    cout << "_____exampleMore___\n";
    NTScalarBuilderPtr ntScalarBuilder = NTScalar::createBuilder();
    PVStructurePtr pvRequest = ntScalarBuilder->
        value(pvString)->
        createPVStructure(); 
    PVStringPtr pvArgument = pvRequest->getSubField<PVString>("value");
    pvArgument->put("MoreWorld");
    try {
        PvaClientRPCPtr rpc(pva->channel(channelName)->createRPC());    
        cout << "send " << pvArgument->get() << endl;
        PVStructurePtr pvResult = rpc->request(pvRequest);
        cout << "result\n" << pvResult << endl;
    } catch (epics::pvAccess::RPCRequestException & ex){
        std::cerr << "Acquisition of greeting was not successful. RPCException:" << "\n";
        std::cerr << ex.what() << std::endl;
    } catch (std::exception& e) {
        // Catch any other exceptions so we always call ClientFactory::stop().
        std::cerr << "Acquisition of greeting was not successful. Unexpected exception." << "\n";
        std::cerr << e.what() << "\n";
    }
}

static void exampleEvenMore(PvaClientPtr const &pva,string const & channelName)
{
    cout << "_____exampleEvenMore___\n";
    NTScalarBuilderPtr ntScalarBuilder = NTScalar::createBuilder();
    PVStructurePtr pvRequest = ntScalarBuilder->
        value(pvString)->
        createPVStructure(); 
    PVStringPtr pvArgument = pvRequest->getSubField<PVString>("value");
    PvaClientChannelPtr pvaChannel = pva->createChannel(channelName);
    pvaChannel->issueConnect();
    Status status = pvaChannel->waitConnect(2.0);
    if(!status.isOK()) {cout << " connect failed\n"; return;}
    ClientRPCRequester::shared_pointer requester(new ClientRPCRequester());
    PvaClientRPCPtr rpc(pvaChannel->createRPC());
    rpc->issueConnect();
    status = rpc->waitConnect();
    if(!status.isOK()) {cout << " rpc connect failed\n"; return;}
    cout << "example channeRPC more\n";
    pvArgument->put("EvenMoreWorld");
    cout << "send " << pvArgument->get() << endl;
    rpc->request(pvRequest,requester);
    requester->waitResponse();
}


int main(int argc,char *argv[])
{
    string channelName("PVRhelloRPC");
    string debugString;
    bool debug(false);
    int opt;
    while((opt = getopt(argc, argv, "hd:")) != -1) {
        switch(opt) {
            case 'h':
             cout << " -h- d debug channelName " << endl;
             cout << "default" << endl;
             cout << " -d " << (debug ? "true" : "false") 
                  << " " <<  channelName
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
    cout << " channelName " <<  channelName
         << " debug " << (debug ? "true" : "false") << endl;
    cout << "_____rpc starting__\n";
    try {
        if(debug) PvaClient::setDebug(true);
        int nPvs = argc - optind;
        if(nPvs>0) channelName = argv[optind];
        PvaClientPtr pva= PvaClient::get("pva");
        exampleSimple(pva,channelName);
        exampleMore(pva,channelName);
        exampleEvenMore(pva,channelName);
        cout << "_____rpc done_______\n";
    } catch (std::exception& e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
