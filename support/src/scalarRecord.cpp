/* scalarRecord.cpp */
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
#include <pv/channelProviderLocal.h>

#define epicsExportSharedSymbols
#include "pv/scalarRecord.h"

using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace std;

namespace epics { namespace exampleCPP { namespace support {


ScalarRecord::~ScalarRecord()
{
cout << "ScalarRecord::~ScalarRecord()\n";
}

ScalarRecordPtr ScalarRecord::create(
    string const & recordName,
    epics::pvData::ScalarType scalarType,
    double minValue,
    double maxValue,
    double stepSize)
{
    FieldCreatePtr fieldCreate = getFieldCreate();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    StandardFieldPtr standardField = getStandardField();
    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
        add("value",scalarType) ->
        add("timeStamp",standardField->timeStamp()) ->
        createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(topStructure);
    ScalarRecordPtr pvRecord(
        new ScalarRecord(recordName,pvStructure,minValue,maxValue,stepSize));
    if(!pvRecord->init()) pvRecord.reset();
    return pvRecord;
}

ScalarRecord::ScalarRecord(
    std::string const & recordName,
    epics::pvData::PVStructurePtr const & pvStructure,
    double minValue,
    double maxValue,
    double stepSize)
: PVRecord(recordName,pvStructure),
  minValue(minValue),
  maxValue(maxValue),
  stepSize(stepSize),
  stepPositive(true)
{
}

bool ScalarRecord::init()
{
    initPVRecord();
    pvValue = getPVStructure()->getSubField<PVScalar>("value");
    if(!pvValue) return false;
    return true;
}

void ScalarRecord::process()
{
    ConvertPtr convert = getConvert();
    double value = convert->toDouble(pvValue);
    if(stepPositive) {
        value = value + stepSize;
        if(value>maxValue) {
           stepPositive = false;
           value = maxValue;
        }
    } else {
        value = value - stepSize;
        if(value<minValue) {
             stepPositive = true;
             value = minValue;
        }
    }
    convert->fromDouble(pvValue,value);
    PVRecord::process();
}

}}}

