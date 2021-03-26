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

class PutLinkScalarArrayRecord;
typedef std::tr1::shared_ptr<PutLinkScalarArrayRecord> PutLinkScalarArrayRecordPtr;

class epicsShareClass PutLinkScalarArrayRecord :
    public epics::pvDatabase::PVRecord
{
private:
    PutLinkScalarArrayRecord(
        std::string const & recordName,
        PVStructurePtr const & pvStructure);    
    PVStringArrayPtr pvValue;      
    PVStringPtr pvLink;
    PVStringPtr pvAccessMethod;
    PVStructurePtr pvAlarmField;
    PVAlarm pvAlarm;
    Alarm alarm;
    epics::pvDatabase::PVRecordPtr linkRecord;
    epics::pvaClient::PvaClientChannelPtr linkChannel;
    epics::pvaClient::PvaClientPutPtr clientPut;
    PVBooleanPtr pvReconnect;
    void clientProcess();
    void databaseProcess();
public:
    POINTER_DEFINITIONS(PutLinkScalarArrayRecord);
    static PutLinkScalarArrayRecordPtr create(std::string const & recordName);
    virtual ~PutLinkScalarArrayRecord() {}
    virtual void process();
    virtual bool init();

};

PutLinkScalarArrayRecordPtr PutLinkScalarArrayRecord::create(std::string const & recordName)
{
    FieldCreatePtr fieldCreate = getFieldCreate();
    StandardFieldPtr standardField = getStandardField();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    StructureConstPtr top = fieldCreate->createFieldBuilder()->
        addArray("value",pvString) ->
        add("linkRecord",pvString) ->
        add("accessMethod",pvString) ->
        add("timeStamp",standardField->timeStamp()) ->
        add("alarm",standardField->alarm()) ->
        add("alarm",standardField->alarm()) ->
        add("reconnect",epics::pvData::pvBoolean) ->
        createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);
    PutLinkScalarArrayRecordPtr pvRecord(
        new PutLinkScalarArrayRecord(recordName,pvStructure)); 
    if(!pvRecord->init()) pvRecord.reset();   
    return pvRecord;
}

PutLinkScalarArrayRecord::PutLinkScalarArrayRecord(
    string const & recordName,
    PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure)
{
}

bool PutLinkScalarArrayRecord::init()
{
    initPVRecord();
    PVStructurePtr pvStructure = getPVRecordStructure()->getPVStructure();
    pvValue = pvStructure->getSubField<PVStringArray>("value");
    pvLink = pvStructure->getSubField<PVString>("linkRecord");
    pvLink->put("doubleArray");
    pvReconnect = pvStructure->getSubField<PVBoolean>("reconnect");
    pvReconnect->put(false);
    pvAccessMethod = pvStructure->getSubField<PVString>("accessMethod");
    pvAccessMethod->put(std::string("database"));
    pvAlarmField = pvStructure->getSubField<PVStructure>("alarm");
    pvAlarm.attach(pvAlarmField);
    return true;
}

void PutLinkScalarArrayRecord::process()
{
   bool reconnect = (pvReconnect->get() ? true : false);
   if(reconnect) {
       linkRecord = PVRecordPtr();
       clientPut = PvaClientPutPtr();
       linkChannel = PvaClientChannelPtr();
       pvReconnect->put(false);
       alarm.setMessage("reconnecting");
       alarm.setSeverity(minorAlarm);
       pvAlarm.set(alarm);
       PVRecord::process();
       return;
   }
   std::string accessMethod = pvAccessMethod->get();
   if(accessMethod.compare("client")==0) {
        clientProcess();
        return;
   }
   if(accessMethod.compare("database")==0) {
        databaseProcess();
        return;
   }
   alarm.setMessage("illegal accessMethod: must be client or database");
   alarm.setSeverity(invalidAlarm);
   pvAlarm.set(alarm);
   PVRecord::process();
}

void PutLinkScalarArrayRecord::clientProcess()
{
    PvaClientPtr pva = PvaClient::get("pva");
    if(!linkChannel) {
        try {
            linkChannel = pva->channel(pvLink->get());
            clientPut = linkChannel->createPut();
            clientPut->connect();
        } catch(std::exception& ex) {
            clientPut = PvaClientPutPtr();
            linkChannel = PvaClientChannelPtr();
            Status status = Status(Status::STATUSTYPE_FATAL, ex.what());
            alarm.setMessage(status.getMessage());
            alarm.setSeverity(invalidAlarm);
            pvAlarm.set(alarm);
            PVRecord::process();
            return;
        }       
    }
    PvaClientPutDataPtr putData = clientPut->getData();
    PVScalarArrayPtr pvScalarArray(putData->getPVStructure()->getSubField<PVScalarArray>("value"));
    if(!pvScalarArray) {
        alarm.setMessage(string("record ")
           + pvLink->get()
           + string("value is not a scalar array field"));
        alarm.setSeverity(invalidAlarm);
        pvAlarm.set(alarm);
        PVRecord::process();
        return;    
    }
    size_t len = pvValue->getLength();
    shared_vector<const string> value(len);
    pvValue->getAs(value);
    pvScalarArray->putFrom(value);
    clientPut->put();
    alarm.setMessage("success");
    alarm.setSeverity(noAlarm);
    pvAlarm.set(alarm);
    PVRecord::process();
}

void PutLinkScalarArrayRecord::databaseProcess()
{
    PVDatabasePtr pvDatabase = PVDatabase::getMaster();
    if(!linkRecord) linkRecord = pvDatabase->findRecord(pvLink->get());
    if(!linkRecord) {
        alarm.setMessage(string("record ") + pvLink->get() + string(" does not exist"));
        alarm.setSeverity(invalidAlarm);
        pvAlarm.set(alarm);
        PVRecord::process();
        return;
    }
    PVScalarArrayPtr pvScalarArray(linkRecord->getPVStructure()->getSubField<PVScalarArray>("value"));
    if(!pvScalarArray) {
        alarm.setMessage(string("record ")
           + pvLink->get()
           + string(" argument.value is not a scalar array field"));
        alarm.setSeverity(invalidAlarm);
        pvAlarm.set(alarm);
        PVRecord::process();
        return;    
    }
    size_t len = pvValue->getLength();
    shared_vector<const string> value(len);
    pvValue->getAs(value);
    alarm.setMessage("success");
    alarm.setSeverity(noAlarm);
    pvAlarm.set(alarm);
    try {
        epicsGuard <PVRecord> guard(*linkRecord);
        linkRecord->beginGroupPut();
        pvScalarArray->putFrom(value);
        linkRecord->process();
        linkRecord->endGroupPut();
    } catch(std::exception& ex) {
        Status status = Status(Status::STATUSTYPE_FATAL, ex.what());
        alarm.setMessage(status.getMessage());
        alarm.setSeverity(invalidAlarm);
        pvAlarm.set(alarm);
    }
    PVRecord::process();
}

static const iocshArg arg0 = { "recordName", iocshArgString };

static const iocshArg *args[] = {&arg0};

static const iocshFuncDef putLinkScalarArrayFuncDef = {
    "putLinkScalarArray", 1, args};
static void putLinkScalarArrayCallFunc(const iocshArgBuf *args)
{
    string putLinkScalarArrayRecord("putLinkScalarArray");
    char *sval = args[0].sval;
    if(sval) putLinkScalarArrayRecord = string(sval);
    PVDatabasePtr master = PVDatabase::getMaster();
    bool result(false);
    PutLinkScalarArrayRecordPtr record = PutLinkScalarArrayRecord::create(putLinkScalarArrayRecord);
    if(record) 
        result = master->addRecord(record);
    if(!result) cout << "recordname" << " not added" << endl;
}

static void putLinkScalarArrayRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&putLinkScalarArrayFuncDef, putLinkScalarArrayCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(putLinkScalarArrayRegister);
} 
