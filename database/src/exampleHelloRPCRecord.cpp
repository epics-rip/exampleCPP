/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.04.02
 */

#include <epicsThread.h>
#include <pv/standardField.h>
#include <pv/ntscalar.h>
#include <sstream>
#include <epicsExport.h>
#include <pv/exampleHelloRPCRecord.h>

using namespace epics::pvData;
using namespace epics::nt;
using namespace epics::pvDatabase;
using namespace epics::pvAccess;
using std::tr1::static_pointer_cast;
using std::tr1::dynamic_pointer_cast;
using namespace std;

namespace epics { namespace exampleCPP { namespace database {

class HelloService;
typedef std::tr1::shared_ptr<HelloService> HelloServicePtr;

class epicsShareClass HelloService :
    public virtual RPCServiceAsync
{
public:
    POINTER_DEFINITIONS(HelloService);

    static HelloService::shared_pointer create(const epics::pvDatabase::PVRecordPtr & pvRecord)
    {
        return HelloServicePtr(new HelloService(pvRecord));
    }
    ~HelloService() {};
 
    void request(
        PVStructurePtr const & args,
        RPCResponseCallback::shared_pointer const & callback
    );
    
private:
    HelloService(const epics::pvDatabase::PVRecordPtr & pvRecord)
    : pvRecord(pvRecord)
    {
    }

    epics::pvDatabase::PVRecordWPtr pvRecord;
};

void HelloService::request(
    PVStructure::shared_pointer const & pvArgument,
    RPCResponseCallback::shared_pointer const & callback
)
{
    try {
         PVStringPtr pvFrom = pvArgument->getSubField<PVString>("value");
         if(!pvFrom) {
            stringstream ss;
            ss << " expected string subfield named value. got\n" << pvArgument;
            Status status(Status::STATUSTYPE_ERROR,ss.str());
            callback->requestDone(status,PVStructurePtr());
            return;
        }
        epics::pvDatabase::PVRecordPtr record(pvRecord.lock());
        if(!record) {
            Status status(Status::STATUSTYPE_ERROR,"pvRecord was destroyed");
            callback->requestDone(status,PVStructurePtr());
            return;
        }
        epicsThreadSleep(2.0);  // simulate service that takes time
        record->lock();
        record->beginGroupPut();
        PVStringPtr pvTo = record->getPVStructure()->getSubField<PVString>("value");
        pvTo->put("Hello " + pvFrom->get());
        record->process();
        record->endGroupPut();
        NTScalarBuilderPtr ntScalarBuilder = NTScalar::createBuilder();
        PVStructurePtr pvResult = ntScalarBuilder->
            value(pvString)->
            createPVStructure(); 
        PVStringPtr pvValue(pvResult->getSubField<PVString>("value"));
        pvValue->put(pvTo->get());
        record->unlock();
        callback->requestDone(Status(), pvResult);
    }
    catch (std::exception& e) {
        throw epics::pvAccess::RPCRequestException(
            Status::STATUSTYPE_ERROR,e.what());
    }
}

ExampleHelloRPCRecordPtr  ExampleHelloRPCRecord::create(string const & recordName)
{
    NTScalarBuilderPtr ntScalarBuilder = NTScalar::createBuilder();
    PVStructurePtr pvTop = ntScalarBuilder->
        value(pvString)->
        addTimeStamp()->
        createPVStructure();
    ExampleHelloRPCRecordPtr pvRecord(
        new ExampleHelloRPCRecord(recordName,pvTop));
    pvRecord->initPVRecord();
    return pvRecord;
}

ExampleHelloRPCRecord::ExampleHelloRPCRecord(
    string const & recordName,
    PVStructurePtr const & pvTop)
: PVRecord(recordName,pvTop)
{
}

RPCServiceAsync::shared_pointer ExampleHelloRPCRecord::getService(PVStructurePtr const & pvRequest)
{
     return HelloService::create(shared_from_this());
}

}}}
