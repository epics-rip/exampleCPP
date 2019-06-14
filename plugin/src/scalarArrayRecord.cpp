/* scalarArrayRecord.cpp */
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

#include <pv/scalarArrayRecord.h>

using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace std;

namespace epics { namespace exampleCPP { namespace plugin {


ScalarArrayRecord::~ScalarArrayRecord()
{
cout << "ScalarArrayRecord::~ScalarArrayRecord()\n";
}

ScalarArrayRecordPtr ScalarArrayRecord::create(
    std::string const & recordName,epics::pvData::ScalarType scalarType)
{
    FieldCreatePtr fieldCreate = getFieldCreate();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    StandardFieldPtr standardField = getStandardField();
    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
        addArray("value",scalarType) ->
        add("timeStamp",standardField->timeStamp()) ->
        createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(topStructure);
    ScalarArrayRecordPtr pvRecord(
        new ScalarArrayRecord(recordName,pvStructure));
    if(!pvRecord->init()) pvRecord.reset();
    return pvRecord;
}

ScalarArrayRecord::ScalarArrayRecord(
    std::string const & recordName,
    epics::pvData::PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure)
{
}

bool ScalarArrayRecord::init()
{
    initPVRecord();
    return true;
}

}}}

