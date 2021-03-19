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
#include "pv/putLinkScalarRecord.h"

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

PutLinkScalarRecordPtr PutLinkScalarRecord::create(std::string const & recordName)
{
    StructureConstPtr top = fieldCreate->createFieldBuilder()->
        add("value",pvString) ->
        add("linkRecord",pvString) ->
        add("accessMethod",pvString) ->
        add("timeStamp",standardField->timeStamp()) ->
        add("alarm",standardField->alarm()) ->
        add("reconnect",epics::pvData::pvBoolean) ->
        createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);
    PutLinkScalarRecordPtr pvRecord(
        new PutLinkScalarRecord(recordName,pvStructure)); 
    if(!pvRecord->init()) pvRecord.reset();   
    return pvRecord;
}

PutLinkScalarRecord::PutLinkScalarRecord(
    string const & recordName,
    PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure)
{
}

bool PutLinkScalarRecord::init()
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

void PutLinkScalarRecord::process()
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

void PutLinkScalarRecord::clientProcess()
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
    PVScalarPtr pvScalar(putData->getPVStructure()->getSubField<PVScalar>("value"));
    if(!pvScalar) {
        alarm.setMessage(string("record ")
           + pvLink->get()
           + string("value is not a scalar array field"));
        alarm.setSeverity(invalidAlarm);
        pvAlarm.set(alarm);
        PVRecord::process();
        return;    
    }
    try {
        pvScalar->putFrom(pvValue->get());
        clientPut->put();      
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

void PutLinkScalarRecord::databaseProcess()
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
    alarm.setMessage("success");
    alarm.setSeverity(noAlarm);
    pvAlarm.set(alarm);
    epicsGuard <PVRecord> guard(*linkRecord);
    linkRecord->beginGroupPut();
    try {
        pvScalar->putFrom(pvValue->get());
    } catch(std::exception& ex) {
        Status status = Status(Status::STATUSTYPE_FATAL, ex.what());
        alarm.setMessage(status.getMessage());
        alarm.setSeverity(invalidAlarm);
        pvAlarm.set(alarm);
    }
    linkRecord->process();
    linkRecord->endGroupPut();
    PVRecord::process();
}

}}}
