// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/* helloPutGet.cpp */
/**
 * @author mrk
 * @date 2013.04.02
 */

#include <pv/standardField.h>

#define epicsExportSharedSymbols
#include <pv/helloPutGet.h>

using namespace epics::pvData;
using namespace epics::pvDatabase;
using std::tr1::static_pointer_cast;
using std::string;

namespace epics { namespace helloPutGet { 


HelloPutGetPtr HelloPutGet::create(
    string const & recordName)
{
    StandardFieldPtr standardField = getStandardField();
    FieldCreatePtr fieldCreate = getFieldCreate();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
        addNestedStructure("argument")->
            add("value",pvString)->
            endNested()->
        addNestedStructure("result") ->
            add("value",pvString) ->
            add("timeStamp",standardField->timeStamp()) ->
            endNested()->
        createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(topStructure);

    HelloPutGetPtr pvRecord(
        new HelloPutGet(recordName,pvStructure));
    if(!pvRecord->init()) pvRecord.reset();
    return pvRecord;
}

HelloPutGet::HelloPutGet(
    string const & recordName,
    PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure)
{
}

HelloPutGet::~HelloPutGet()
{
}

void HelloPutGet::destroy()
{
    PVRecord::destroy();
}

bool HelloPutGet::init()
{
    
    initPVRecord();
    PVFieldPtr pvField;
    pvArgumentValue = getPVStructure()->getSubField<PVString>("argument.value");
    if(pvArgumentValue.get()==NULL) return false;
    pvResultValue = getPVStructure()->getSubField<PVString>("result.value");
    if(pvResultValue.get()==NULL) return false;
    pvTimeStamp.attach(getPVStructure()->getSubField("result.timeStamp"));
    return true;
}

void HelloPutGet::process()
{
    pvResultValue->put(string("Hello ") + pvArgumentValue->get());
    timeStamp.getCurrent();
    pvTimeStamp.set(timeStamp);
}

}}
