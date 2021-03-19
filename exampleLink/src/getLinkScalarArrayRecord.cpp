/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2021.03.15
 */
#include <pv/standardField.h>
#include <pv/standardPVField.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>
#include <pv/alarm.h>
#include <pv/pvAlarm.h>
#include <pv/pvDatabase.h>
#include <pv/pvaClient.h>

#define epicsExportSharedSymbols
#include "pv/getLinkScalarArrayRecord.h"

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::pvaClient;
using std::tr1::static_pointer_cast;
using std::tr1::dynamic_pointer_cast;
using std::cout;
using std::endl;
using std::string;
using std::vector;

namespace epics { namespace exampleCPP { namespace exampleLink {

static FieldCreatePtr fieldCreate = getFieldCreate();
static StandardFieldPtr standardField = getStandardField();
static PVDataCreatePtr pvDataCreate = getPVDataCreate();

GetLinkScalarArrayRecordPtr GetLinkScalarArrayRecord::create(std::string const & recordName)
{
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
    std::size_t len = pvScalarArray->getLength();
    shared_vector<const string> value(len);
    pvScalarArray->getAs(value);
    pvValue->putFrom(value);
    alarm.setMessage("success");
    alarm.setSeverity(noAlarm);
    pvAlarm.set(alarm);
    PVRecord::process();   
}

}}}
