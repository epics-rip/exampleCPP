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

class GetLinkScalarArrayRecord;
typedef std::tr1::shared_ptr<GetLinkScalarArrayRecord> GetLinkScalarArrayRecordPtr;

class epicsShareClass GetLinkScalarArrayRecord :
    public epics::pvDatabase::PVRecord
{
private:
    GetLinkScalarArrayRecord(
        std::string const & recordName,
        PVStructurePtr const & pvStructure);    
    PVStringArrayPtr pvValue;      
    PVStringPtr pvLink;
    PVStringPtr pvAccessMethod;
    PVStructurePtr pvAlarmField;
    PVAlarm pvAlarm;
    Alarm alarm;
    epics::pvDatabase::PVRecordWPtr linkRecord;
    epics::pvaClient::PvaClientChannelPtr linkChannel;
    epics::pvaClient::PvaClientGetPtr clientGet;
    PVBooleanPtr pvReconnect;
    void clientProcess();
    void databaseProcess();
public:
    POINTER_DEFINITIONS(GetLinkScalarArrayRecord);
    static GetLinkScalarArrayRecordPtr create(std::string const & recordName);
    virtual ~GetLinkScalarArrayRecord() {}
    virtual void process();
    virtual bool init();

};

GetLinkScalarArrayRecordPtr GetLinkScalarArrayRecord::create(std::string const & recordName)
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
    GetLinkScalarArrayRecordPtr pvRecord(
        new GetLinkScalarArrayRecord(recordName,pvStructure)); 
    if(!pvRecord->init()) pvRecord.reset();   
    return pvRecord;
}

GetLinkScalarArrayRecord::GetLinkScalarArrayRecord(
    string const & recordName,
    PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure)
{
}

bool GetLinkScalarArrayRecord::init()
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

void GetLinkScalarArrayRecord::process()
{
   bool reconnect = (pvReconnect->get() ? true : false);
   if(reconnect) {
       linkRecord = PVRecordWPtr();
       clientGet = PvaClientGetPtr();
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

void GetLinkScalarArrayRecord::clientProcess()
{
    PvaClientPtr pva = PvaClient::get("pva");
    if(!linkChannel) {
        try {
            linkChannel = pva->channel(pvLink->get());
            clientGet = linkChannel->createGet();
            clientGet->connect();
        } catch(std::exception& ex) {
            clientGet = PvaClientGetPtr();
            linkChannel = PvaClientChannelPtr();
            Status status = Status(Status::STATUSTYPE_FATAL, ex.what());
            alarm.setMessage(status.getMessage());
            alarm.setSeverity(invalidAlarm);
            pvAlarm.set(alarm);
            PVRecord::process();
            return;
        }       
    }
    clientGet->get();
    PVStructurePtr pvStructure = clientGet->getData()->getPVStructure();
    PVScalarArrayPtr pvScalarArray(pvStructure->getSubField<PVScalarArray>("value"));
    if(!pvScalarArray) {
        alarm.setMessage(string("record ")
           + pvLink->get()
           + string(" value is not a scalar array"));
        alarm.setSeverity(invalidAlarm);
        pvAlarm.set(alarm);
        PVRecord::process();
        return;    
    }
    std::size_t len = pvScalarArray->getLength();
    shared_vector<const string> value(len);
    pvScalarArray->getAs(value);
    pvValue->putFrom(value);
    alarm.setMessage("success");
    alarm.setSeverity(noAlarm);
    pvAlarm.set(alarm);
    PVRecord::process();   
}

void GetLinkScalarArrayRecord::databaseProcess()
{
    PVDatabasePtr pvDatabase = PVDatabase::getMaster();
    {
        PVRecordPtr pvRecord(linkRecord.lock());
        if(!pvRecord) {  
            PVRecordPtr pvRecord(pvDatabase->findRecord(pvLink->get()));
            if(pvRecord) linkRecord= PVRecordWPtr(pvRecord);
        }    
    }
    PVRecordPtr pvRecord(linkRecord.lock());
    if(!pvRecord) {
        linkRecord = PVRecordWPtr();
        alarm.setMessage(string("record ") + pvLink->get() + string(" does not exist"));
        alarm.setSeverity(invalidAlarm);
        pvAlarm.set(alarm);
        PVRecord::process();
        return;
    }
    PVScalarArrayPtr pvScalarArray(pvRecord->getPVStructure()->getSubField<PVScalarArray>("value"));
    if(!pvScalarArray) {
        alarm.setMessage(string("record ")
           + pvLink->get()
           + string(" argument.value is not a scalar array field"));
        alarm.setSeverity(invalidAlarm);
        pvAlarm.set(alarm);
        PVRecord::process();
        return;    
    }
    std::size_t len = pvScalarArray->getLength();
    shared_vector<const string> value(len);
    pvScalarArray->getAs(value);
    pvValue->putFrom(value);
    alarm.setMessage("success");
    alarm.setSeverity(noAlarm);
    pvAlarm.set(alarm);
    PVRecord::process();   
}

static const iocshArg arg0 = { "recordName", iocshArgString };
static const iocshArg *args[] = {&arg0};

static const iocshFuncDef getLinkScalarArrayFuncDef = {
    "getLinkScalarArray", 1, args};
static void getLinkScalarArrayCallFunc(const iocshArgBuf *args)
{
    string getLinkScalarArrayRecord("getLinkScalarArray");
    char *sval = args[0].sval;
    if(sval) getLinkScalarArrayRecord = string(sval);
    PVDatabasePtr master = PVDatabase::getMaster();
    bool result(false);
    GetLinkScalarArrayRecordPtr record = GetLinkScalarArrayRecord::create(getLinkScalarArrayRecord);
    if(record) 
        result = master->addRecord(record);
    if(!result) cout << "recordname" << " not added" << endl;
}

static void getLinkScalarArrayRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&getLinkScalarArrayFuncDef, getLinkScalarArrayCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(getLinkScalarArrayRegister);
} 
