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
#include <pv/clientFactory.h>
#include <pv/ntscalarArray.h>

#include <pv/exampleLinkRecord.h>

using namespace std;
using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::nt;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::exampleCPP::exampleLink;

int main(int argc,char *argv[])
{
    string provider("pva");
    if(argc==2 && string(argv[1])==string("-help")) {
        cout << "provider" << endl;
        cout << "default" << endl;
        cout << provider;
        return 0;
    }
    if(argc>1) provider = argv[1];
    if(provider=="pva") ClientFactory::start();
    PVDatabasePtr master = PVDatabase::getMaster();
    NTScalarArrayBuilderPtr ntScalarArrayBuilder = NTScalarArray::createBuilder();
    PVStructurePtr pvStructure = ntScalarArrayBuilder->
        value(pvDouble)->
        addAlarm()->
        addTimeStamp()->
        createPVStructure();
    string recordName("doubleArray");
    master->addRecord(PVRecord::create(recordName,pvStructure));
    ExampleLinkRecordPtr pvRecord(
        ExampleLinkRecord::create(
           "exampleLink",provider,recordName));
    master->addRecord(pvRecord);

    ChannelProviderLocalPtr channelProvider = getChannelProviderLocal();
    
    ServerContext::shared_pointer ctx =
        startPVAServer(PVACCESS_ALL_PROVIDERS,0,true,true);
    cout << "exampleLink\n";
    string str;
    while(true) {
        cout << "Type exit to stop: \n";
        getline(cin,str);
        if(str.compare("exit")==0) break;

    }
    ctx->destroy();
epicsThreadSleep(5.0);  // should not be necessary
    return 0;
}
