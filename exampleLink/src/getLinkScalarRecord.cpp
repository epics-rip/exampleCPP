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
#include <pv/convert.h>

#define epicsExportSharedSymbols
#include "pv/getLinkScalarRecord.h"

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

GetLinkScalarRecordPtr GetLinkScalarRecord::create(std::string const & recordName)
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

}}}
