/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2021.04.20
 */
#ifndef HELLORPCRECORD_H
#define HELLORPCRECORD_H

#include <pv/pvData.h>
#include <pv/pvDatabase.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>

#include <shareLib.h>


namespace epics { namespace example { namespace helloRPC {

class HelloRPCRecord;
typedef std::tr1::shared_ptr<HelloRPCRecord> HelloRPCRecordPtr;

/**
 * @brief A PVRecord that implements a hello service accessed via a channelRPC request.
 *
 */
class epicsShareClass HelloRPCRecord :
    public epics::pvDatabase::PVRecord
{
public:
    POINTER_DEFINITIONS(HelloRPCRecord);
    /**
     * @brief Create an instance of ExampleHelloRecord.
     *
     * @param recordName The name of the record.
     * @return The new instance.
     */
    static  HelloRPCRecordPtr create(std::string const & recordName);

     /**
      * @brief Get the ExampleRPC service.
      */
    virtual epics::pvAccess::RPCServiceAsync::shared_pointer getService(
        epics::pvData::PVStructurePtr const & pvRequest);
    
    virtual ~HelloRPCRecord() {}
    virtual bool init() {return true;}
private :
    HelloRPCRecord(
        std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvTop);
};

}}}

#endif  /* HELLORPCRECORD_H */
