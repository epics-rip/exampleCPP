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
#include <pv/convert.h>
// The following must be the last include for code exampleLink uses
#include <epicsExport.h>
#define epicsExportSharedSymbols

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::pvaClient;
using namespace std;

class GetLinkScalarRecord;
typedef std::tr1::shared_ptr<GetLinkScalarRecord> GetLinkScalarRecordPtr;

class epicsShareClass GetLinkScalarRecord :
    public epics::pvDatabase::PVRecord
{
private:
    GetLinkScalarRecord(
        std::string const & recordName,
        PVStructurePtr const & pvStructure);    
    PVStringPtr pvValue;      
    PVStringPtr pvLink;
    PVStringPtr pvAccessMethod;
    PVStructurePtr pvAlarmField;
    PVAlarm pvAlarm;
    Alarm alarm;
    epics::pvDatabase::PVRecordPtr linkRecord;
    epics::pvaClient::PvaClientChannelPtr linkChannel;
    epics::pvaClient::PvaClientGetPtr clientGet;
    PVBooleanPtr pvReconnect;
    void clientProcess();
    void databaseProcess();
public:
    POINTER_DEFINITIONS(GetLinkScalarRecord);
    static GetLinkScalarRecordPtr create(std::string const & recordName);
    virtual ~GetLinkScalarRecord() {}
    virtual void process();
    virtual bool init();
};

GetLinkScalarRecordPtr GetLinkScalarRecord::create(std::string const & recordName)
{
    FieldCreatePtr fieldCreate = getFieldCreate();
    StandardFieldPtr standardField = getStandardField();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    StructureConstPtr top = fieldCreate->createFieldBuilder()->
        add("value",pvString) ->
        add("linkRecord",pvString) ->
        add("accessMethod",pvString) ->
        add("timeStamp",standardField->timeStamp()) ->
        add("alarm",standardField->alarm()) ->
        add("reconnect",epics::pvData::pvBoolean) ->
        createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);
    GetLinkScalarRecordPtr pvRecord(
        new GetLinkScalarRecord(recordName,pvStructure)); 
    if(!pvRecord->init()) pvRecord.reset();   
    return pvRecord;
}

GetLinkScalarRecord::GetLinkScalarRecord(
    string const & recordName,
    PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure)
{
}

bool GetLinkScalarRecord::init()
{
    initPVRecord();
    PVStructurePtr pvStructure = getPVRecordStructure()->getPVStructure();
    pvValue = pvStructure->getSubField<PVString>("value");
    pvLink = pvStructure->getSubField<PVString>("linkRecord");
    pvLink->put("double");
    pvReconnect = pvStructure->getSubField<PVBoolean>("reconnect");
    pvReconnect->put(false);
    pvAccessMethod = pvStructure->getSubField<PVString>("accessMethod");
    pvAccessMethod->put(std::string("database"));
    pvAlarmField = pvStructure->getSubField<PVStructure>("alarm");
    pvAlarm.attach(pvAlarmField);
    return true;
}

void GetLinkScalarRecord::process()
{
   bool reconnect = (pvReconnect->get() ? true : false);
   if(reconnect) {
       linkRecord = PVRecordPtr();
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

void GetLinkScalarRecord::clientProcess()
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
    PVScalarPtr pvScalar(pvStructure->getSubField<PVScalar>("value"));
    if(!pvScalar) {
        alarm.setMessage(string("record ")
           + pvLink->get()
           + string(" value is not a scalar field"));
        alarm.setSeverity(invalidAlarm);
        pvAlarm.set(alarm);
        PVRecord::process();
        return;    
    }
    ConvertPtr convert = getConvert();
    try {
        pvValue->put(convert->toString(pvScalar));
    } catch(std::exception& ex) {
        Status status = Status(Status::STATUSTYPE_FATAL, ex.what());
        alarm.setMessage(status.getMessage());
        alarm.setSeverity(invalidAlarm);
        pvAlarm.set(alarm);
        PVRecord::process();
        return;    
    }
    alarm.setMessage("success");
    alarm.setSeverity(noAlarm);
    pvAlarm.set(alarm);
    PVRecord::process();
}

void GetLinkScalarRecord::databaseProcess()
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
    PVScalarPtr pvScalar(linkRecord->getPVStructure()->getSubField<PVScalar>("value"));
    if(!pvScalar) {
        alarm.setMessage(string("record ")
           + pvLink->get()
           + string(" argument.value is not a scalar array field"));
        alarm.setSeverity(invalidAlarm);
        pvAlarm.set(alarm);
        PVRecord::process();
        return;    
    }
    ConvertPtr convert = getConvert();
    try {
        pvValue->put(convert->toString(pvScalar));
    } catch(std::exception& ex) {
        Status status = Status(Status::STATUSTYPE_FATAL, ex.what());
        alarm.setMessage(status.getMessage());
        alarm.setSeverity(invalidAlarm);
        pvAlarm.set(alarm);
        PVRecord::process();
        return;    
    }
    alarm.setMessage("success");
    alarm.setSeverity(noAlarm);
    pvAlarm.set(alarm);
    PVRecord::process();
}

static const iocshArg arg0 = { "recordName", iocshArgString };
static const iocshArg *args[] = {&arg0};

static const iocshFuncDef getLinkScalarFuncDef = {
    "getLinkScalar", 1, args};
static void getLinkScalarCallFunc(const iocshArgBuf *args)
{
    string getLinkScalarRecord("getLinkScalar");
    char *sval = args[0].sval;
    if(sval) getLinkScalarRecord = string(sval);
    PVDatabasePtr master = PVDatabase::getMaster();
    bool result(false);
    GetLinkScalarRecordPtr record = GetLinkScalarRecord::create(getLinkScalarRecord);
    if(record) 
        result = master->addRecord(record);
    if(!result) cout << "recordname" << " not added" << endl;
}

static void getLinkScalarRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&getLinkScalarFuncDef, getLinkScalarCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(getLinkScalarRegister);
} 
