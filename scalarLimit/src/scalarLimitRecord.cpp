/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2021.05.05
 */
#include <iocsh.h>
#include <pv/standardField.h>
#include <pv/standardPVField.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>
#include <pv/alarm.h>
#include <pv/pvAlarm.h>
#include <pv/convert.h>
#include <pv/pvDatabase.h>

// The following must be the last include for code scalarLimit implements
#include <epicsExport.h>
#define epicsExportSharedSymbols
#include "scalarLimit/scalarLimitRecord.h"
using namespace epics::pvData;
using namespace epics::pvDatabase;
using namespace std;

namespace epics { namespace scalarLimit {

class epicsShareClass ScalarLimitRecord :
     public epics::pvDatabase::PVRecord
{
private:
friend class ScalarLimitRecordCreate;
    ScalarLimitRecord(
      std::string const & recordName,epics::pvData::PVStructurePtr const & pvStructure,
      int asLevel,std::string const & asGroup);
   double desiredValue;
   double currentValue;
public:
    virtual ~ScalarLimitRecord() {}
    virtual void process();
    virtual bool init(){
        initPVRecord();
        return true;
    }
}; 

ScalarLimitRecord::ScalarLimitRecord(
    std::string const & recordName,epics::pvData::PVStructurePtr const & pvStructure,
    int asLevel,std::string const & asGroup)
: PVRecord(recordName,pvStructure,asLevel,asGroup)
{}
void ScalarLimitRecord::process()
{
    ConvertPtr convert = getConvert();
    PVStructurePtr top = this->getPVStructure();
    PVScalarPtr value = top->getSubField<PVScalar>("value");
    PVStructurePtr timeStamp = top->getSubField<PVStructure>("timeStamp");
    PVStructurePtr alarm = top->getSubField<PVStructure>("alarm");
    PVStructurePtr control = top->getSubField<PVStructure>("control");
    PVScalarPtr limitLow  = control->getSubField<PVScalar>("limitLow");
    PVScalarPtr  limitHigh = control->getSubField<PVScalar>("limitHigh");
    PVScalarPtr  maxStep = control->getSubField<PVScalar>("maxStep");
    double low = convert->toDouble(limitLow);
    double high = convert->toDouble(limitHigh);
    if(low>=high) return;
    
    double valNow = convert->toDouble(value);
    if((valNow!=this->desiredValue) && (valNow!=this->currentValue)) {
        double low = convert->toDouble(limitLow);
        double high = convert->toDouble(limitHigh);
        if(valNow<low) valNow = low;
        if(valNow>high) valNow = high;
        this->desiredValue = valNow;
    } else if(this->desiredValue==this->currentValue) {
        return;
    }
    convert->fromDouble(value,this->currentValue);
    bool move = !(this->desiredValue==this->currentValue);
    if(move) {
        double step = convert->toDouble(maxStep);
        double diff = this->desiredValue - this->currentValue;
        if(diff>0.0) {
            if(diff<step) step = diff;
        } else {
            if(-step<diff) {
                step = diff;            
            } else {
                step = -step;
            }
        }
        double val = convert->toDouble(value) + step;
        double low = convert->toDouble(limitLow);
        double high = convert->toDouble(limitHigh);
        if(val<low) val = low;
        if(val>high) val = high;
        convert->fromDouble(value,val);
        this->currentValue = val;
    }
    PVStructurePtr alarmLimit = top->getSubField<PVStructure>("alarmLimit");
    PVScalarPtr lowAlarmLimit  = alarmLimit->getSubField<PVScalar>("lowAlarmLimit");
    PVScalarPtr lowWarningLimit  = alarmLimit->getSubField<PVScalar>("lowWarningLimit");
    PVScalarPtr  highAlarmLimit = alarmLimit->getSubField<PVScalar>("highAlarmLimit");
    PVScalarPtr  highWarningLimit = alarmLimit->getSubField<PVScalar>("highWarningLimit");
    double lowAlarm = convert->toDouble(lowAlarmLimit);
    double lowWarning = convert->toDouble(lowWarningLimit);
    double highAlarm = convert->toDouble(highAlarmLimit);
    double highWarning = convert->toDouble(highWarningLimit);
    if(lowAlarm>=lowWarning) return;
    if(highAlarm<=highWarning) return;
    if(lowWarning>=highWarning) return;
    double val = convert->toDouble(value);
    PVIntPtr pvSeverity = top->getSubField<PVInt>("alarm.severity");
    int severity = pvSeverity->get();
    PVStringPtr pvMessage = top->getSubField<PVString>("alarm.message");
    if(val<=lowAlarm) {
        if(severity!=2) {
            pvSeverity->put(2);
            pvMessage->put("major low alarm");
        }
    } else if(val<=lowWarning) {
        if(severity!=1) {
            pvSeverity->put(1);
            pvMessage->put("minor low alarm");
        }
    } else if(val>=highAlarm) {
        if(severity!=2) {
            pvSeverity->put(2);
            pvMessage->put("major high alarm");
        }
    } else if(val>=highWarning) {
        if(severity!=1) {
            pvSeverity->put(1);
            pvMessage->put("minor high alarm");
        }
    } else {
        if(severity!=0) {
            pvSeverity->put(0);
            pvMessage->put("no alarm");
        }
    }
    PVRecord::process();
}

void ScalarLimitRecordCreate::create(
    std::string const & recordName,std::string const &  scalarType,
    int asLevel,std::string const & asGroup)
{
    ConvertPtr convert = getConvert();
    ScalarType st = epics::pvData::ScalarTypeFunc::getScalarType(scalarType);
    FieldCreatePtr fieldCreate = getFieldCreate();
    StandardFieldPtr standardField = getStandardField();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    StructureConstPtr top = fieldCreate->createFieldBuilder()->
        add("value",st) ->
        add("timeStamp",standardField->timeStamp()) ->
        add("alarm",standardField->alarm()) ->
        addNestedStructure("control") ->
            add("limitLow",st) ->
            add("limitHigh",st) ->
            add("maxStep",st) ->
        endNested()->
        addNestedStructure("alarmLimit") ->
            add("lowAlarmLimit",st) ->
            add("lowWarningLimit",st) ->
            add("highAlarmLimit",st) ->
            add("highWarningLimit",st) ->
        endNested()->
        createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);
    PVScalarPtr pvValue = pvStructure->getSubField<PVScalar>("value");
    convert->fromDouble(pvValue,100.0);
    PVScalarPtr pvlimitHigh = pvStructure->getSubField<PVScalar>("control.limitHigh");
    convert->fromDouble(pvlimitHigh,250.0);
    PVScalarPtr pvmaxStep = pvStructure->getSubField<PVScalar>("control.maxStep");
    convert->fromDouble(pvmaxStep,10.0);
    
    PVScalarPtr lowAlarmLimit = pvStructure->getSubField<PVScalar>("alarmLimit.lowAlarmLimit");
    convert->fromDouble(lowAlarmLimit,20.0);
    PVScalarPtr lowWarningLimit = pvStructure->getSubField<PVScalar>("alarmLimit.lowWarningLimit");
    convert->fromDouble(lowWarningLimit,50.0);
    PVScalarPtr highAlarmLimit = pvStructure->getSubField<PVScalar>("alarmLimit.highAlarmLimit");
    convert->fromDouble(highAlarmLimit,230.0);
    PVScalarPtr highWarningLimit = pvStructure->getSubField<PVScalar>("alarmLimit.highWarningLimit");
    convert->fromDouble(highWarningLimit,200.0);
    
    ScalarLimitRecordPtr pvRecord =
        ScalarLimitRecordPtr(
            new ScalarLimitRecord(recordName,pvStructure,asLevel,asGroup));
    pvRecord->init();
    pvRecord->desiredValue = 100.0;
    pvRecord->currentValue = 100.0;
    PVDatabasePtr master = PVDatabase::getMaster();
    if(!master->addRecord(pvRecord)) {
        cerr << recordName << " not added to master\n";
    }
}

}}

static const iocshArg arg0 = { "recordName", iocshArgString };
static const iocshArg arg1 = { "scalarType", iocshArgString };
static const iocshArg arg2 = { "asLevel", iocshArgInt };
static const iocshArg arg3 = { "asGroup", iocshArgString };
static const iocshArg *args[] = {&arg0,&arg1,&arg2,&arg3};

static const iocshFuncDef scalarLimitFuncDef = {"scalarLimitRecord", 4,args};

static void scalarLimitCallFunc(const iocshArgBuf *args)
{
    char *sval = args[0].sval;
    if(!sval) {
        throw std::runtime_error("scalarLimitRecord recordName not specified");
    }
    string recordName = string(sval);
    sval = args[1].sval;
    if(!sval) {
        throw std::runtime_error("scalarLimitRecord scalarType not specified");
    }
    string scalarType = string(sval);
    int asLevel = args[2].ival;
    string asGroup("DEFAULT");
    sval = args[3].sval;
    if(sval) {
        asGroup = string(sval);
    }
    epics::scalarLimit::ScalarLimitRecordCreate::create(recordName,scalarType,asLevel,asGroup); 
}

static void scalarLimitRecord(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&scalarLimitFuncDef, scalarLimitCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(scalarLimitRecord);
}
