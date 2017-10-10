/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.04.02
 */
#ifndef EXAMPLEHELLORPCRECORD_H
#define EXAMPLEHELLORPCRECORD_H

#include <pv/pvData.h>
#include <pv/pvDatabase.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>

#include <shareLib.h>


namespace epics { namespace exampleCPP { namespace database {

class ExampleHelloRPCRecord;
typedef std::tr1::shared_ptr<ExampleHelloRPCRecord> ExampleHelloRPCRecordPtr;

/**
 * @brief A PVRecord that implements a hello service accessed via a channelRPC request.
 *
 */
class epicsShareClass ExampleHelloRPCRecord :
    public epics::pvDatabase::PVRecord
{
public:
    POINTER_DEFINITIONS(ExampleHelloRPCRecord);
    /**
     * @brief Create an instance of ExampleHelloRecord.
     *
     * @param recordName The name of the record.
     * @return The new instance.
     */
    static  ExampleHelloRPCRecordPtr create(std::string const & recordName);

     /**
      * @brief Get the ExampleRPC service.
      */
    virtual epics::pvAccess::RPCServiceAsync::shared_pointer getService(
        epics::pvData::PVStructurePtr const & pvRequest);
    
    virtual ~ExampleHelloRPCRecord() {}
    virtual bool init() {return true;}
private :
    ExampleHelloRPCRecord(
        std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvTop);
};

}}}

#endif  /* EXAMPLEHELLORPCRECORD_H */
