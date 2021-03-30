/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2021.03.26
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
// The following must be the last include for code exampleLink uses
#include <epicsExport.h>
#define epicsExportSharedSymbols

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::pvaClient;
using namespace std;

class ScalarArrayRecord;
typedef std::tr1::shared_ptr<ScalarArrayRecord> ScalarArrayRecordPtr;
class epicsShareClass ScalarArrayRecord :
    public epics::pvDatabase::PVRecord
{
public:
    POINTER_DEFINITIONS(ScalarArrayRecord);
    static ScalarArrayRecordPtr create(
        std::string const & recordName,
        epics::pvData::ScalarType scalarType);
    ~ScalarArrayRecord();
private:
    ScalarArrayRecord(
        std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure);
};

ScalarArrayRecord::~ScalarArrayRecord()
{
cout << "ScalarArrayRecord::~ScalarArrayRecord()\n";
}

ScalarArrayRecordPtr ScalarArrayRecord::create(
    string const & recordName,
    epics::pvData::ScalarType scalarType)
{
    FieldCreatePtr fieldCreate = getFieldCreate();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    StandardFieldPtr standardField = getStandardField();
    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
        addArray("value",scalarType) ->
        add("timeStamp",standardField->timeStamp()) ->
        createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(topStructure);
    ScalarArrayRecordPtr pvRecord(
        new ScalarArrayRecord(recordName,pvStructure));
    pvRecord->init();
    return pvRecord;
}

ScalarArrayRecord::ScalarArrayRecord(
    std::string const & recordName,
    epics::pvData::PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure)
{
}

static const iocshArg testArg0 = { "recordName", iocshArgString };
static const iocshArg testArg1 = { "scalarType", iocshArgString };
static const iocshArg *testArgs[] = {
    &testArg0,&testArg1};

static const iocshFuncDef scalarArrayRecordFuncDef = {"scalarArrayRecordCreate", 2,testArgs};

static void scalarArrayRecordCallFunc(const iocshArgBuf *args)
{
    cerr << "DEPRECATED\n";
    char *recordName = args[0].sval;
    if(!recordName) {
        throw std::runtime_error("scalarArrayRecordCreate invalid number of arguments");
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
    ScalarArrayRecordPtr record = ScalarArrayRecord::create(
        recordName,scalarType);
    bool result = PVDatabase::getMaster()->addRecord(record);
    if(!result) cout << "recordname" << " not added" << endl;
}

static void scalarArrayRecordRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&scalarArrayRecordFuncDef, scalarArrayRecordCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(scalarArrayRecordRegister);
}
