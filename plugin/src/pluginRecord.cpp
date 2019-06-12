/* pluginRecord.cpp */
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

#define epicsExportSharedSymbols

#include <pv/pluginRecord.h>

using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace std;

namespace epics { namespace exampleCPP { namespace plugin {


PluginRecord::~PluginRecord()
{
cout << "PluginRecord::~PluginRecord()\n";
}

PluginRecordPtr PluginRecord::create(
    std::string const & recordName,epics::pvData::ScalarType scalarType)
{
    FieldCreatePtr fieldCreate = getFieldCreate();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    StandardFieldPtr standardField = getStandardField();
    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
        add("value",scalarType) ->
        add("alarm",standardField->alarm()) ->
        add("timeStamp",standardField->timeStamp()) ->
        add("display",standardField->display()) ->
        add("control",ControlSupport::controlField(scalarType)) ->
        add("scalarAlarm",ScalarAlarmSupport::scalarAlarmField()) ->
        createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(topStructure);
    PluginRecordPtr pvRecord(
        new PluginRecord(recordName,pvStructure));
    if(!pvRecord->init()) pvRecord.reset();
    return pvRecord;
}

PluginRecord::PluginRecord(
    std::string const & recordName,
    epics::pvData::PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure)
{
}

bool PluginRecord::init()
{
    initPVRecord();
    PVRecordPtr pvRecord = shared_from_this();
    PVStructurePtr pvStructure(getPVStructure());
    pvValue = pvStructure->getSubField<PVScalar>("value");
    ScalarConstPtr s = static_pointer_cast<const Scalar>(pvValue->getField());
    epics::pvData::ScalarType scalarType = s->getScalarType();
    isSigned = false;
    switch(scalarType) {
    case pvByte:
    case pvShort:
    case pvInt:
    case pvLong:
        isSigned = true; break;
    case pvUByte:
    case pvUShort:
    case pvUInt:
    case pvULong:
        isSigned = false; break;
    case pvFloat:
    case pvDouble:
         isSigned = true; break;
    default:
         throw std::logic_error("illegal scalar type");
    }
    if(!pvValue) return false;
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
    return true;
}

void PluginRecord::process()
{
    PVRecord::process();
    ConvertPtr convert = getConvert();
    double value = convert->toDouble(pvValue);
    value = value +1;
    convert->fromDouble(pvValue,value);
    controlSupport->process();
    if(convert->toDouble(pvValue)!=value) {
        double value = -100;
        if(!isSigned) value = 0;
        convert->fromDouble(pvValue,value);
    }
    controlSupport->process();
    scalarAlarmSupport->process();
}


}}}

