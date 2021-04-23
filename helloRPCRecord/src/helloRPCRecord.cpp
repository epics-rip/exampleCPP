/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2021.04.20
 */
#include <iocsh.h>
#include <epicsThread.h>
#include <pv/pvDatabase.h>
#include <pv/pvStructureCopy.h>
#include <pv/timeStamp.h>
#include <pv/pvAlarm.h>
#include <pv/standardField.h>
#include <pv/pvAccess.h>
#include <pv/ntscalar.h>
#include <pv/channelProviderLocal.h>

// The following must be the last include for code database uses
#include <epicsExport.h>
#define epicsExportSharedSymbols
#include "helloRPC/helloRPCRecord.h"

using namespace epics::pvData;
using namespace epics::nt;
using namespace epics::pvDatabase;
using namespace epics::pvAccess;
using std::tr1::static_pointer_cast;
using namespace std;


namespace epics { namespace example { namespace helloRPC { 

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

HelloRPCRecordPtr  HelloRPCRecord::create(string const & recordName)
{
    NTScalarBuilderPtr ntScalarBuilder = NTScalar::createBuilder();
    PVStructurePtr pvTop = ntScalarBuilder->
        value(pvString)->
        addTimeStamp()->
        createPVStructure();
    HelloRPCRecordPtr pvRecord(
        new HelloRPCRecord(recordName,pvTop));
    pvRecord->initPVRecord();
    return pvRecord;
}

HelloRPCRecord::HelloRPCRecord(
    string const & recordName,
    PVStructurePtr const & pvTop)
: PVRecord(recordName,pvTop)
{
}

RPCServiceAsync::shared_pointer HelloRPCRecord::getService(PVStructurePtr const & pvRequest)
{
     return HelloService::create(shared_from_this());
}


}}}

static const iocshArg testArg0 = { "recordName", iocshArgString };
static const iocshArg *testArgs[] = {
    &testArg0};

static const iocshFuncDef helloRPCFuncDef = {
    "helloRPCRecord", 1, testArgs};
static void helloRPCCallFunc(const iocshArgBuf *args)
{
    char *recordName = args[0].sval;
    if(!recordName) {
        throw std::runtime_error("helloRPCRecord invalid number of arguments");
    }
    epics::example::helloRPC::HelloRPCRecordPtr record
        = epics::example::helloRPC::HelloRPCRecord::create(recordName);
    bool result = PVDatabase::getMaster()->addRecord(record);
    if(!result) std::cout << string(recordName) << " not added" << "\n";
}

static void helloRPCRecord(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&helloRPCFuncDef, helloRPCCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(helloRPCRecord);
}

