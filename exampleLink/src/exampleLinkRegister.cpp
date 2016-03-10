// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/*exampleLinkRegister.cpp */

/**
 * @author mrk
 * @date 2013.07.24
 */


/* Author: Marty Kraimer */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <memory>
#include <iostream>

#include <cantProceed.h>
#include <epicsStdio.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <epicsThread.h>
#include <iocsh.h>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/standardField.h>
#include <pv/standardPVField.h>
#include <pv/pvAccess.h>
#include <pv/ntscalarArray.h>
#include <pv/pvDatabase.h>

#include <epicsExport.h>
#include <pv/exampleLinkRecord.h>

using namespace epics::pvData;
using namespace epics::nt;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::exampleCPP::exampleLink;
using std::cout;
using std::endl;
using std::string;

static StandardPVFieldPtr standardPVField = getStandardPVField();

static const iocshArg testArg0 = { "recordName", iocshArgString };
static const iocshArg testArg1 = { "providerName", iocshArgString };
static const iocshArg testArg2 = { "channelName", iocshArgString };
static const iocshArg *testArgs[] = {
    &testArg0,&testArg1,&testArg2};

static const iocshFuncDef exampleLinkFuncDef = {
    "exampleLinkCreateRecord", 3, testArgs};
static void exampleLinkCallFunc(const iocshArgBuf *args)
{
    char *recordName = args[0].sval;
    char *providerName = args[1].sval;
    char *channelName = args[2].sval;
    if(!recordName || !providerName || !channelName) {
        throw std::runtime_error("exampleLinkCreateRecord invalid number of arguments");
    }
cout << "exampleLinkCreateRecord recordName " << recordName << " providerName " << providerName << " channelName " << channelName << endl;
    PVDatabasePtr master = PVDatabase::getMaster();
    PVRecordPtr pvRecord;
    bool result(false);
    NTScalarArrayBuilderPtr ntScalarArrayBuilder = NTScalarArray::createBuilder();
    PVStructurePtr pvStructure = ntScalarArrayBuilder->
        value(pvDouble)->
        addAlarm()->
        addTimeStamp()->
        createPVStructure();
    result = master->addRecord(PVRecord::create(channelName,pvStructure));
    if(!result) cout<< "record " << recordName << " not added" << endl;

    ExampleLinkRecordPtr record = ExampleLinkRecord::create(recordName,providerName,channelName);
    if(record) 
        result = master->addRecord(record);
    if(!result) cout << "recordname" << " not added" << endl;
}

static void exampleLinkRegister(void)
{
    static int firstTime = 1;
cout << "exampleLinkRegister firstTime " << firstTime << endl;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&exampleLinkFuncDef, exampleLinkCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(exampleLinkRegister);
}
