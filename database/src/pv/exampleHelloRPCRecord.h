// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/* exampleHelloRPCRecord.h */
/**
 * @author mrk
 * @date 2013.04.02
 */
#ifndef EXAMPLEHELLORPCRECORD_H
#define EXAMPLEHELLORPCRECORD_H

#ifdef epicsExportSharedSymbols
#   define exampleHelloRPCRecordEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <pv/pvData.h>
#include <pv/pvDatabase.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>

#ifdef exampleHelloRPCRecordEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef exampleHelloRPCRecordEpicsExportSharedSymbols
#endif

#include <shareLib.h>


namespace epics { namespace exampleCPP { namespace database {


class ExampleHelloRPCRecord;
typedef std::tr1::shared_ptr<ExampleHelloRPCRecord> ExampleHelloRPCRecordPtr;

class ExampleHelloRPCRecordService;
typedef std::tr1::shared_ptr<ExampleHelloRPCRecordService> ExampleHelloRPCRecordServicePtr;

class epicsShareClass ExampleHelloRPCRecordService:
    public virtual epics::pvAccess::RPCService
{
public:
    POINTER_DEFINITIONS(ExampleHelloRPCRecordService);

    static ExampleHelloRPCRecordServicePtr create(ExampleHelloRPCRecordPtr const & pvRecord)
    {
        return ExampleHelloRPCRecordServicePtr(new ExampleHelloRPCRecordService(pvRecord));
    } 
    ~ExampleHelloRPCRecordService() {};

    epics::pvData::PVStructurePtr request(
        epics::pvData::PVStructurePtr const & args
    );
private:
    ExampleHelloRPCRecordService(ExampleHelloRPCRecordPtr const & pvRecord)
    : pvRecord(pvRecord) {}

    ExampleHelloRPCRecordPtr pvRecord;
};

class epicsShareClass ExampleHelloRPCRecord :
    public epics::pvDatabase::PVRecord
{
public:
    POINTER_DEFINITIONS(ExampleHelloRPCRecord);
    static  ExampleHelloRPCRecordPtr create(std::string const & recordName);
    virtual ~ExampleHelloRPCRecord() {}
    virtual void destroy() {PVRecord::destroy();}
    virtual bool init();
    virtual void process() {PVRecord::process();}
    virtual epics::pvAccess::Service::shared_pointer getService(
        epics::pvData::PVStructurePtr const & pvRequest);
    void put(epics::pvData::PVStringPtr const & pvFrom);
private :
    ExampleHelloRPCRecord(
        std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvResult);

    epics::pvData::PVStructurePtr pvResult;
    epics::pvAccess::Service::shared_pointer service;
    friend class ExampleHelloRPCRecordService;
};

}}}

#endif  /* EXAMPLEHELLORPCRECORD_H */
