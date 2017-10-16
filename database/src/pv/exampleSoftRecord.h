/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.04.02
 */
#ifndef EXAMPLESOFTRECORD_H
#define EXAMPLESOFTRECORD_H

#include <pv/pvDatabase.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>

#include <shareLib.h>


namespace epics { namespace exampleCPP { namespace database {


class ExampleSoftRecord;
typedef std::tr1::shared_ptr<ExampleSoftRecord> ExampleSoftRecordPtr;

/**
 * @brief A PVRecord that implements a hello service accessed via a channelPutGet request.
 *
 */
class epicsShareClass ExampleSoftRecord :
    public epics::pvDatabase::PVRecord
{
public:
    POINTER_DEFINITIONS(ExampleSoftRecord);
    /**
     * @brief Create an instance of ExampleSoftRecord.
     *
     * @param recordName The name of the record.
     * @return The new instance.
     */
    static ExampleSoftRecordPtr create(
        std::string const & recordName);
    /**
     *  @brief Implement hello semantics.
     */
    virtual void process();
    virtual ~ExampleSoftRecord() {}
    virtual bool init() {return true;}
private:
    ExampleSoftRecord(std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure);

};


}}}

#endif  /* EXAMPLESOFTRECORD_H */
