/* exampleHello.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2013.04.02
 */

#include <pv/standardField.h>

#define epicsExportSharedSymbols
#include <pv/exampleHello.h>

using namespace epics::pvData;
using namespace epics::pvDatabase;
using std::tr1::static_pointer_cast;
using std::string;

namespace epics { namespace exampleHello { 


ExampleHelloPtr ExampleHello::create(
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

    ExampleHelloPtr pvRecord(
        new ExampleHello(recordName,pvStructure));
    if(!pvRecord->init()) pvRecord.reset();
    return pvRecord;
}

ExampleHello::ExampleHello(
    string const & recordName,
    PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure)
{
}

ExampleHello::~ExampleHello()
{
}

void ExampleHello::destroy()
{
    PVRecord::destroy();
}

bool ExampleHello::init()
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

void ExampleHello::process()
{
    pvResultValue->put(string("Hello ") + pvArgumentValue->get());
    timeStamp.getCurrent();
    pvTimeStamp.set(timeStamp);
}

}}
