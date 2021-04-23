/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.04.02
 */
#include <iocsh.h>
#include <pv/pvDatabase.h>
#include <pv/pvStructureCopy.h>
#include <pv/timeStamp.h>
#include <pv/pvAlarm.h>
#include <pv/standardField.h>
#include <pv/channelProviderLocal.h>

// The following must be the last include for code database uses
#include <epicsExport.h>
#define epicsExportSharedSymbols
#include "helloPutGet/helloPutGetRecord.h"

using namespace epics::pvData;
using namespace epics::pvDatabase;
using std::string;

namespace epics { namespace example { namespace helloPutGet { 


HelloPutGetRecordPtr HelloPutGetRecord::create(
    string const & recordName)
{
    StandardFieldPtr standardField = getStandardField();
    FieldCreatePtr fieldCreate = getFieldCreate();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
        add("timeStamp",standardField->timeStamp()) ->
        addNestedStructure("argument")->
            add("value",pvString)->
            endNested()->
        addNestedStructure("result") ->
            add("value",pvString) ->    
            endNested()->
        createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(topStructure);

    HelloPutGetRecordPtr pvRecord(
        new HelloPutGetRecord(recordName,pvStructure));
    pvRecord->initPvt();
    return pvRecord;
}

HelloPutGetRecord::HelloPutGetRecord(
    string const & recordName,
    PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure)
{
}


void HelloPutGetRecord::initPvt()
{
    initPVRecord();
    pvArgumentValue = getPVStructure()->getSubField<PVString>("argument.value");
    pvResultValue = getPVStructure()->getSubField<PVString>("result.value");
}


void HelloPutGetRecord::process()
{
    pvResultValue->put("Hello " + pvArgumentValue->get());
    PVRecord::process();
}

}}}

static const iocshArg testArg0 = { "recordName", iocshArgString };
static const iocshArg *testArgs[] = {
    &testArg0};

static const iocshFuncDef helloPutGetFuncDef = {
    "helloPutGetRecord", 1, testArgs};
static void helloPutGetCallFunc(const iocshArgBuf *args)
{
    char *recordName = args[0].sval;
    if(!recordName) {
        throw std::runtime_error("helloPutGetRecord invalid number of arguments");
    }
    epics::example::helloPutGet::HelloPutGetRecordPtr record
        = epics::example::helloPutGet::HelloPutGetRecord::create(recordName);
    bool result = PVDatabase::getMaster()->addRecord(record);
    if(!result) std::cout << string(recordName) << " not added" << "\n";
}

static void helloPutGetRecord(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&helloPutGetFuncDef, helloPutGetCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(helloPutGetRecord);
}

