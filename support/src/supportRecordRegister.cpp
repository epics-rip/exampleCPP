/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.07.24
 */
#include <iocsh.h>
#include <pv/standardField.h>
#include <pv/standardPVField.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>
#include <pv/alarm.h>
#include <pv/pvAlarm.h>
#include <pv/pvDatabase.h>
#include <pv/pvaClient.h>
#include <pv/controlSupport.h>
#include <pv/scalarAlarmSupport.h>
// The following must be the last include for code exampleLink uses
#include <epicsExport.h>
#define epicsExportSharedSymbols

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::pvaClient;
using namespace std;

class SupportRecord;
typedef std::tr1::shared_ptr<SupportRecord> SupportRecordPtr;
class epicsShareClass SupportRecord :
    public epics::pvDatabase::PVRecord
{
public:
    POINTER_DEFINITIONS(SupportRecord);
    static SupportRecordPtr create(
        std::string const & recordName,epics::pvData::ScalarType scalarType);
    virtual bool init();
    virtual void process();
    ~SupportRecord();
private:
    SupportRecord(
        std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure);
    epics::pvDatabase::ControlSupportPtr controlSupport;
    epics::pvDatabase::ScalarAlarmSupportPtr scalarAlarmSupport;
    epics::pvData::PVBooleanPtr pvReset;
};

SupportRecord::~SupportRecord()
{
cout << "SupportRecord::~SupportRecord()\n";
}

SupportRecordPtr SupportRecord::create(
    std::string const & recordName,epics::pvData::ScalarType scalarType)
{
    FieldCreatePtr fieldCreate = getFieldCreate();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    StandardFieldPtr standardField = getStandardField();
    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
        add("value",scalarType) ->
        add("reset",pvBoolean) ->
        add("alarm",standardField->alarm()) ->
        add("timeStamp",standardField->timeStamp()) ->
        add("display",standardField->display()) ->
        add("control",ControlSupport::controlField(scalarType)) ->
        add("scalarAlarm",ScalarAlarmSupport::scalarAlarmField()) ->
        createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(topStructure);
    SupportRecordPtr pvRecord(
        new SupportRecord(recordName,pvStructure));
    if(!pvRecord->init()) pvRecord.reset();
    return pvRecord;
}

SupportRecord::SupportRecord(
    std::string const & recordName,
    epics::pvData::PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure)
{
}

bool SupportRecord::init()
{
    initPVRecord();
    PVRecordPtr pvRecord = shared_from_this();
    PVStructurePtr pvStructure(getPVStructure());
    PVStructurePtr pvControl = pvStructure->getSubField<PVStructure>("control");
    pvControl->getSubField<PVDouble>("limitHigh")->put(100.0);
    pvControl->getSubField<PVDouble>("minStep")->put(1.0);
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

static const iocshArg testArg0 = { "recordName", iocshArgString };
static const iocshArg testArg1 = { "scalarType", iocshArgString };
static const iocshArg *testArgs[] = {
    &testArg0,&testArg1};

static const iocshFuncDef supportRecordFuncDef = {"supportRecordCreate", 2,testArgs};

static void supportRecordCallFunc(const iocshArgBuf *args)
{
    char *recordName = args[0].sval;
    if(!recordName) {
        throw std::runtime_error("supportRecordCreate invalid number of arguments");
    }
    char *stype = args[1].sval;
    epics::pvData::ScalarType scalarType  = epics::pvData::pvDouble;
    if(stype) {
        string val(stype);
        if(val.compare("pvByte")==0) {
            scalarType = epics::pvData::pvByte;
        } else if(val.compare("pvShort")==0) {
            scalarType = epics::pvData::pvShort;
        } else if(val.compare("pvInt")==0) {
            scalarType = epics::pvData::pvInt;
        } else if(val.compare("pvLong")==0) {
            scalarType = epics::pvData::pvLong;
        } else if(val.compare("pvFloat")==0) {
            scalarType = epics::pvData::pvFloat;
        } else if(val.compare("pvDouble")==0) {
            scalarType = epics::pvData::pvDouble;
        } else if(val.compare("pvUByte")==0) {
            scalarType = epics::pvData::pvUByte;
        } else if(val.compare("pvUShort")==0) {
            scalarType = epics::pvData::pvUShort;
        } else if(val.compare("pvUInt")==0) {
            scalarType = epics::pvData::pvUInt;
        } else if(val.compare("pvULong")==0) {
            scalarType = epics::pvData::pvULong;
        } else {
             cout << val << " is not a support scalar type\n";
             return; 
        }
    }
    SupportRecordPtr record = SupportRecord::create(recordName,scalarType);
    bool result = PVDatabase::getMaster()->addRecord(record);
    if(!result) cout << "recordname" << " not added" << endl;
}

static void supportRecordRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&supportRecordFuncDef, supportRecordCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(supportRecordRegister);
}
