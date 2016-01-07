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

#include <pv/exampleLink.h>

using namespace std;
using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::nt;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;

int main(int argc,char *argv[])
{
    string provider("pva");
    if(argc>1) provider = argv[1];
    PVDatabasePtr master = PVDatabase::getMaster();
    NTScalarArrayBuilderPtr ntScalarArrayBuilder = NTScalarArray::createBuilder();
    PVStructurePtr pvStructure = ntScalarArrayBuilder->
        value(pvDouble)->
        addAlarm()->
        addTimeStamp()->
        createPVStructure();
    master->addRecord(PVRecord::create("doubleArray",pvStructure));
    ChannelProviderLocalPtr channelProvider = getChannelProviderLocal();
    if(provider=="pva") ClientFactory::start();
    ServerContext::shared_pointer ctx =
        startPVAServer(PVACCESS_ALL_PROVIDERS,0,true,true);
    ExampleLinkPtr pvRecord(
        ExampleLink::create(
           "exampleLink",provider,"doubleArray")); 
    master->addRecord(pvRecord);
    cout << "exampleLink\n";
    string str;
    while(true) {
        cout << "Type exit to stop: \n";
        getline(cin,str);
        if(str.compare("exit")==0) break;

    }
    ctx->destroy();
    epicsThreadSleep(3.0);
//    channelProvider->destroy();
//    if(provider=="pva") ClientFactory::stop();
//    epicsThreadSleep(3.0);
    return 0;
}
