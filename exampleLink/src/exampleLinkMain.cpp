// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/*exampleLinkMain.cpp */
/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <memory>
#include <vector>
#include <iostream>

#include <pv/channelProviderLocal.h>
#include <pv/serverContext.h>
#include <pv/pvaClient.h>
#include <pv/serverContext.h>
#include <pv/ntscalarArray.h>

#include <pv/exampleLinkRecord.h>

using namespace std;
using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::nt;
using namespace epics::pvaClient;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::exampleCPP::exampleLink;

int main(int argc,char *argv[])
{
    string provider("pva");
    string exampleLinkRecordName("exampleLink");
    string linkedRecordName("doubleArray");
    bool generateLinkedRecord(true);
    if(argc==2 && string(argv[1])==string("-help")) {
        cout << "provider exampleLinkRecordName linkedRecordName generateLinkedRecord" << endl;
        cout << "default" << endl;
        cout << provider << " " << exampleLinkRecordName << " " << linkedRecordName << " true" << endl;
        return 0;
    }
    if(argc>1) provider = argv[1];
    if(argc>2) exampleLinkRecordName = argv[2];
    if(argc>3) linkedRecordName = argv[3];
    if(argc>4) {
        string val = argv[4];
        if(val=="false") generateLinkedRecord = false;
    }
    PvaClientPtr pva= PvaClient::create();
    PVDatabasePtr master = PVDatabase::getMaster();
// If this is not done before 
//  ExampleLinkRecordPtr pvRecord(ExampleLinkRecord::create(exampleLinkRecordName,provider,recordName));
//  master->addRecord(pvRecord);
// and provider is local then failure.
    ChannelProviderLocalPtr channelProvider = getChannelProviderLocal();
    ServerContext::shared_pointer ctx =
    startPVAServer(PVACCESS_ALL_PROVIDERS,0,true,true);
    if(generateLinkedRecord) {
        NTScalarArrayBuilderPtr ntScalarArrayBuilder = NTScalarArray::createBuilder();
        PVStructurePtr pvStructure = ntScalarArrayBuilder->
            value(pvDouble)->
            addAlarm()->
            addTimeStamp()->
            createPVStructure();
        master->addRecord(PVRecord::create(linkedRecordName,pvStructure));
    }
    ExampleLinkRecordPtr pvRecord(
        ExampleLinkRecord::create(
           exampleLinkRecordName,provider,linkedRecordName));

    master->addRecord(pvRecord);

//    ChannelProviderLocalPtr channelProvider = getChannelProviderLocal();
//    ServerContext::shared_pointer ctx =
//        startPVAServer(PVACCESS_ALL_PROVIDERS,0,true,true);
    cout << "exampleLink\n";
    string str;
    while(true) {
        cout << "Type exit to stop: \n";
        getline(cin,str);
        if(str.compare("exit")==0) break;

    }
    ctx->destroy();
    pva->destroy();
//epicsThreadSleep(5.0);  // should not be necessary
    return 0;
}
