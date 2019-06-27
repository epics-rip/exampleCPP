/* supportRecord.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2019.06.01
 */
#include <pv/pvDatabase.h>
#include <pv/convert.h>
#include <pv/standardField.h>
#include <pv/controlSupport.h>
#include <pv/scalarAlarmSupport.h>
#include <pv/channelProviderLocal.h>

#include "pv/supportRecord.h"

using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace std;

namespace epics { namespace exampleCPP { namespace support {


SupportRecord::~SupportRecord()
{
cout << "SupportRecord::~SupportRecord()\n";
}

SupportRecordPtr SupportRecord::create(
    std::string const & recordName,epics::pvData::ScalarType scalarType)
{
    FieldCreatePtr fieldCreate = getFieldCreate();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    StandardFieldPtr standardField = getStandardField();
    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
        add("value",scalarType) ->
        add("reset",pvBoolean) ->
        add("alarm",standardField->alarm()) ->
        add("timeStamp",standardField->timeStamp()) ->
        add("display",standardField->display()) ->
        add("control",ControlSupport::controlField(scalarType)) ->
        add("scalarAlarm",ScalarAlarmSupport::scalarAlarmField()) ->
        createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(topStructure);
    SupportRecordPtr pvRecord(
        new SupportRecord(recordName,pvStructure));
    if(!pvRecord->init()) pvRecord.reset();
    return pvRecord;
}

SupportRecord::SupportRecord(
    std::string const & recordName,
    epics::pvData::PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure)
{
}

bool SupportRecord::init()
{
    initPVRecord();
    PVRecordPtr pvRecord = shared_from_this();
    PVStructurePtr pvStructure(getPVStructure());
    controlSupport = ControlSupport::create(pvRecord);
    bool result = controlSupport->init(
       pvStructure->getSubField("value"),pvStructure->getSubField("control"));
    if(!result) return false;
    scalarAlarmSupport = ScalarAlarmSupport::create(pvRecord);
    result = scalarAlarmSupport->init(
       pvStructure->getSubField("value"),
       pvStructure->getSubField<PVStructure>("alarm"),
       pvStructure->getSubField("scalarAlarm"));
    if(!result) return false;
    pvReset = getPVStructure()->getSubField<PVBoolean>("reset");
    return true;
}

void SupportRecord::process()
{
    bool wasChanged = false;
    if(pvReset->get()==true) {
        pvReset->put(false);
        controlSupport->reset();
        scalarAlarmSupport->reset();
    } else {
        if(controlSupport->process()) wasChanged = true;;
        if(scalarAlarmSupport->process()) wasChanged = true;
    }
    if(wasChanged) PVRecord::process();
}


}}}

