/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */
#include <iocsh.h>
#include <pv/standardField.h>
#include <pv/standardPVField.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>
#include <pv/alarm.h>
#include <pv/pvAlarm.h>
#include <pv/pvDatabase.h>
#include <pv/pvStructureCopy.h>
#include <pv/pvaClient.h>
#include <pv/controlSupport.h>
#include <pv/scalarAlarmSupport.h>

#include <pv/pvAccess.h>

// The following must be the last include for code database uses
#include <epicsExport.h>
#define epicsExportSharedSymbols
#include "pvsupport/supportRecord.h"
using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;

namespace epics { namespace example { namespace support {

SupportRecordPtr SupportRecord::create(
    std::string const & recordName,std::string const &  scalarType,
    int asLevel,std::string const & asGroup)
{
    ScalarType st = epics::pvData::ScalarTypeFunc::getScalarType(scalarType);
    FieldCreatePtr fieldCreate = getFieldCreate();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    StandardFieldPtr standardField = getStandardField();
    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
        add("value",st) ->
        add("reset",pvBoolean) ->
        add("alarm",standardField->alarm()) ->
        add("timeStamp",standardField->timeStamp()) ->
        add("display",standardField->display()) ->
        add("control",ControlSupport::controlField(st)) ->
        add("scalarAlarm",ScalarAlarmSupport::scalarAlarmField()) ->
        createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(topStructure);
    SupportRecordPtr pvRecord(
        new SupportRecord(recordName,pvStructure,asLevel,asGroup));
    if(!pvRecord->init()) pvRecord.reset();
    return pvRecord;
}

SupportRecord::SupportRecord(
    std::string const & recordName,
    epics::pvData::PVStructurePtr const & pvStructure,
    int asLevel,std::string const & asGroup)
: PVRecord(recordName,pvStructure,asLevel,asGroup)
{
}

bool SupportRecord::init()
{
    initPVRecord();
    PVRecordPtr pvRecord = shared_from_this();
    PVStructurePtr pvStructure(getPVStructure());
    controlSupport = ControlSupport::create(pvRecord);
    bool result = controlSupport->init(
       pvStructure->getSubField("value"),pvStructure->getSubField("control"));
    if(!result) return false;
    scalarAlarmSupport = ScalarAlarmSupport::create(pvRecord);
    result = scalarAlarmSupport->init(
       pvStructure->getSubField("value"),
       pvStructure->getSubField<PVStructure>("alarm"),
       pvStructure->getSubField("scalarAlarm"));
    if(!result) return false;
    pvReset = getPVStructure()->getSubField<PVBoolean>("reset");
    return true;
}

void SupportRecord::process()
{
    bool wasChanged = false;
    if(pvReset->get()==true) {
        pvReset->put(false);
        controlSupport->reset();
        scalarAlarmSupport->reset();
    } else {
        if(controlSupport->process()) wasChanged = true;;
        if(scalarAlarmSupport->process()) wasChanged = true;
    }
    if(wasChanged) PVRecord::process();
}
}}}

static const iocshArg arg0 = { "recordName", iocshArgString };
static const iocshArg arg1 = { "scalarType", iocshArgString };
static const iocshArg arg2 = { "asLevel", iocshArgInt };
static const iocshArg arg3 = { "asGroup", iocshArgString };
static const iocshArg *args[] = {&arg0,&arg1,&arg2,&arg3};

static const iocshFuncDef supportRecordFuncDef = {"supportRecord", 4,args};

static void supportRecordCallFunc(const iocshArgBuf *args)
{
    char *sval = args[0].sval;
    if(!sval) {
        throw std::runtime_error("supportRecord recordName not specified");
    }
    string recordName = string(sval);
    sval = args[1].sval;
    if(!sval) {
        throw std::runtime_error("supportRecord scalarType not specified");
    }
    string scalarType = string(sval);
    int asLevel = args[2].ival;
    string asGroup("DEFAULT");
    sval = args[3].sval;
    if(sval) {
        asGroup = string(sval);
    }
    epics::example::support::SupportRecordPtr record
        = epics::example::support::SupportRecord::create(recordName,scalarType,asLevel,asGroup);
    epics::pvDatabase::PVDatabasePtr master = epics::pvDatabase::PVDatabase::getMaster();
    bool result =  master->addRecord(record);
    if(!result) cout << "recordname " << recordName << " not added" << endl;
}

static void supportRecord(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&supportRecordFuncDef, supportRecordCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(supportRecord);
}
