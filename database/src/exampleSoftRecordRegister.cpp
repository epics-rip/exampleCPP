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
#include <pv/controlSupport.h>
#include <pv/scalarAlarmSupport.h>
#include <pv/pvDatabase.h>
#include <pv/pvStructureCopy.h>
// The following must be the last include for code database uses
#include <epicsExport.h>
#define epicsExportSharedSymbols

using namespace epics::pvData;
using namespace epics::pvDatabase;
using namespace std;

namespace epics { namespace exampleCPP { namespace database {
class ExampleSoftRecord;
typedef std::tr1::shared_ptr<ExampleSoftRecord> ExampleSoftRecordPtr;

/**
 * @brief A PVRecord that implements a hello service accessed via a channelPutGet request.
 *
 */
class epicsShareClass ExampleSoftRecord :
    public epics::pvDatabase::PVRecord
{
public:
    POINTER_DEFINITIONS(ExampleSoftRecord);
    /**
     * @brief Create an instance of ExampleSoftRecord.
     *
     * @param recordName The name of the record.
     * @return The new instance.
     */
    static ExampleSoftRecordPtr create(
        std::string const & recordName);
    /**
     *  @brief Implement hello semantics.
     */
    virtual void process();
    virtual ~ExampleSoftRecord() {}
    virtual bool init() {return true;}
private:
    ExampleSoftRecord(std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure);

};

ExampleSoftRecordPtr ExampleSoftRecord::create(
    string const & recordName)
{
    FieldCreatePtr fieldCreate = getFieldCreate();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    StandardFieldPtr standardField = getStandardField();
    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
        add("value",pvDouble) ->
        add("reset",pvBoolean) ->
        add("alarm",standardField->alarm()) ->
        add("timeStamp",standardField->timeStamp()) ->
        add("display",standardField->display()) ->
        add("control",standardField->control())->
        createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(topStructure);
    ExampleSoftRecordPtr pvRecord(
        new ExampleSoftRecord(recordName,pvStructure));
    pvRecord->initPVRecord();
    return pvRecord;
}

ExampleSoftRecord::ExampleSoftRecord(
    string const & recordName,
    PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure)
{
    PVFieldPtr pvField;
}

void ExampleSoftRecord::process()
{

}

}}}

static const iocshArg testArg0 = { "recordName", iocshArgString };
static const iocshArg *testArgs[] = {
    &testArg0};

static const iocshFuncDef exampleSoftRecordFuncDef = {"exampleSoftRecordCreate", 1,testArgs};

static void exampleSoftRecordCallFunc(const iocshArgBuf *args)
{
    char *recordName = args[0].sval;
    if(!recordName) {
        throw std::runtime_error("exampleSoftRecordCreate invalid number of arguments");
    }
    epics::exampleCPP::database::ExampleSoftRecordPtr record
        = epics::exampleCPP::database::ExampleSoftRecord::create(recordName);
    bool result = PVDatabase::getMaster()->addRecord(record);
    if(!result) cout << "recordname" << " not added" << endl;
}

static void exampleSoftRecordRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&exampleSoftRecordFuncDef, exampleSoftRecordCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(exampleSoftRecordRegister);
}
