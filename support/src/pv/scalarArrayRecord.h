/* scalarArrayRecord.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2019.06.14
 */
#ifndef SCALARARRAY_RECORD_H
#define SCALARARRAY_RECORD_H

#include <pv/channelProviderLocal.h>
#include <shareLib.h>

namespace epics { namespace exampleCPP { namespace support {


class ScalarArrayRecord;
typedef std::tr1::shared_ptr<ScalarArrayRecord> ScalarArrayRecordPtr;

/**
 * @brief a record for testing scalarArray plugin
 *
 * This is support for a record with a top level field that has type scalarScalarArray
 * It provides support for control and scalarAlarm
 */
class epicsShareClass ScalarArrayRecord :
    public epics::pvDatabase::PVRecord
{
public:
    POINTER_DEFINITIONS(ScalarArrayRecord);
    /**
     * Factory methods to create ScalarArrayRecord.
     * @param recordName The name for the ScalarArrayRecord.
     * @param scalarType The scalar type.
     * @return A shared pointer to ScalarArrayRecord..
     */
    static ScalarArrayRecordPtr create(
        std::string const & recordName,
        epics::pvData::ScalarType scalarType);
    ~ScalarArrayRecord();
private:
    ScalarArrayRecord(
        std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure);
};

}}}

#endif  /* SCALARARRAY_RECORD_H */
