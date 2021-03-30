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
#include <pv/convert.h>
// The following must be the last include for code exampleLink uses
#include <epicsExport.h>
#define epicsExportSharedSymbols

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::pvaClient;
using namespace std;

class ScalarRecord;
typedef std::tr1::shared_ptr<ScalarRecord> ScalarRecordPtr;
class epicsShareClass ScalarRecord :
    public epics::pvDatabase::PVRecord
{
public:
    POINTER_DEFINITIONS(ScalarRecord);
    static ScalarRecordPtr create(
        std::string const & recordName,
        epics::pvData::ScalarType scalarType,
        double minValue,
        double maxValue,
        double stepSize);
    virtual bool init();
    virtual void process();
    ~ScalarRecord();
private:
    ScalarRecord(
        std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure,
        double minValue,
        double maxValue,
        double stepSize);
    epics::pvData::PVScalarPtr pvValue;
    double minValue;
    double maxValue;
    double stepSize;
    bool stepPositive;
};

ScalarRecord::~ScalarRecord()
{
cout << "ScalarRecord::~ScalarRecord()\n";
}

ScalarRecordPtr ScalarRecord::create(
    string const & recordName,
    epics::pvData::ScalarType scalarType,
    double minValue,
    double maxValue,
    double stepSize)
{
    FieldCreatePtr fieldCreate = getFieldCreate();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    StandardFieldPtr standardField = getStandardField();
    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
        add("value",scalarType) ->
        add("timeStamp",standardField->timeStamp()) ->
        createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(topStructure);
    ScalarRecordPtr pvRecord(
        new ScalarRecord(recordName,pvStructure,minValue,maxValue,stepSize));
    if(!pvRecord->init()) pvRecord.reset();
    return pvRecord;
}

ScalarRecord::ScalarRecord(
    std::string const & recordName,
    epics::pvData::PVStructurePtr const & pvStructure,
    double minValue,
    double maxValue,
    double stepSize)
: PVRecord(recordName,pvStructure),
  minValue(minValue),
  maxValue(maxValue),
  stepSize(stepSize),
  stepPositive(true)
{
}

bool ScalarRecord::init()
{
    initPVRecord();
    pvValue = getPVStructure()->getSubField<PVScalar>("value");
    if(!pvValue) return false;
    return true;
}

void ScalarRecord::process()
{
    ConvertPtr convert = getConvert();
    double value = convert->toDouble(pvValue);
    if(stepPositive) {
        value = value + stepSize;
        if(value>maxValue) {
           stepPositive = false;
           value = maxValue;
        }
    } else {
        value = value - stepSize;
        if(value<minValue) {
             stepPositive = true;
             value = minValue;
        }
    }
    convert->fromDouble(pvValue,value);
    PVRecord::process();
}

static const iocshArg testArg0 = { "recordName", iocshArgString };
static const iocshArg testArg1 = { "scalarType", iocshArgString };
static const iocshArg testArg2 = { "minValue", iocshArgDouble };
static const iocshArg testArg3 = { "maxValue", iocshArgDouble };
static const iocshArg testArg4 = { "stepSize", iocshArgDouble };
static const iocshArg *testArgs[] = {
    &testArg0,&testArg1,&testArg2,&testArg3,&testArg4};

static const iocshFuncDef scalarRecordFuncDef = {"scalarRecordCreate", 5,testArgs};

static void scalarRecordCallFunc(const iocshArgBuf *args)
{
    cerr << "DEPRECATED\n";
    char *recordName = args[0].sval;
    if(!recordName) {
        throw std::runtime_error("scalarRecordCreate invalid number of arguments");
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
        } else if(val.compare("pvString")==0) {
            scalarType = epics::pvData::pvString;
        } else if(val.compare("pvBoolean")==0) {
            scalarType = epics::pvData::pvBoolean;
        } else {
             cout << val << " is not a scalar type\n";
             return; 
        }
    }
    double minValue = args[2].dval;
    double maxValue = args[3].dval;
    double stepSize = args[4].dval;
    ScalarRecordPtr record = ScalarRecord::create(
        recordName,scalarType,minValue,maxValue,stepSize);
    bool result = PVDatabase::getMaster()->addRecord(record);
    if(!result) cout << "recordname" << " not added" << endl;
}

static void scalarRecordRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&scalarRecordFuncDef, scalarRecordCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(scalarRecordRegister);
}
