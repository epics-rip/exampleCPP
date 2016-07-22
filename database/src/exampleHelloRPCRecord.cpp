// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/* exampleHelloRPCRecord.cpp */

/**
 * @author mrk
 * @date 2013.04.02
 */

#include <epicsThread.h>
#include <pv/standardField.h>
#include <sstream>
#define epicsExportSharedSymbols
#include <pv/exampleHelloRPCRecord.h>

using namespace epics::pvData;
using namespace epics::pvDatabase;
using namespace epics::pvAccess;
using std::tr1::static_pointer_cast;
using std::tr1::dynamic_pointer_cast;
using namespace std;

namespace epics { namespace exampleCPP { namespace database {

PVStructurePtr ExampleHelloRPCRecordService::request(PVStructurePtr const & pvArgument)
{
    PVStringPtr pvFrom = pvArgument->getSubField<PVString>("value");
    if(!pvFrom) {
        stringstream ss;
        ss << " expected string subfield named value. got\n" << pvArgument;
        throw RPCRequestException(
                 Status::STATUSTYPE_ERROR,ss.str());
    }
    pvRecord->put(pvFrom);
    return pvRecord->pvResult;
}

ExampleHelloRPCRecordPtr  ExampleHelloRPCRecord::create(string const & recordName)
{
    FieldCreatePtr fieldCreate = getFieldCreate();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
        add("value",pvString)->
        createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(topStructure);
    ExampleHelloRPCRecordPtr pvRecord(
        new ExampleHelloRPCRecord(recordName,pvStructure));
    if(!pvRecord->init()) pvRecord.reset();
    return pvRecord;
}

ExampleHelloRPCRecord::ExampleHelloRPCRecord(
    string const & recordName,
    PVStructurePtr const & pvResult)
: PVRecord(recordName,pvResult),
  pvResult(pvResult)
{
}

bool ExampleHelloRPCRecord::init()
{
    initPVRecord();
    service = ExampleHelloRPCRecordService::create(
        std::tr1::dynamic_pointer_cast<ExampleHelloRPCRecord>(
            shared_from_this()));
    return true;
}


Service::shared_pointer ExampleHelloRPCRecord::getService(PVStructurePtr const & pvRequest)
{
     return service;
}

void ExampleHelloRPCRecord::put(PVStringPtr const &pvFrom)
{
    lock();
    beginGroupPut();
    PVStringPtr pvTo = pvResult->getSubField<PVString>("value");
    pvTo->put("Hello " + pvFrom->get());
    process();
    epicsThreadSleep(1.0);  // simulate service that takes time
    endGroupPut();
    unlock();
    
}

}}}
