/* scalarRecord.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2019.06.14
 */
#ifndef SCALAR_RECORD_H
#define SCALAR_RECORD_H

#include <pv/channelProviderLocal.h>
#include <shareLib.h>

namespace epics { namespace exampleCPP { namespace support {


class ScalarRecord;
typedef std::tr1::shared_ptr<ScalarRecord> ScalarRecordPtr;

/**
 * @brief a record for testing scalarArray plugin
 *
 * This is support for a record with a top level field that has type scalarScalarArray
 * It provides support for control and scalarAlarm
 */
class epicsShareClass ScalarRecord :
    public epics::pvDatabase::PVRecord
{
public:
    POINTER_DEFINITIONS(ScalarRecord);
    /**
     * Factory methods to create ScalarRecord.
     * @param recordName The name for the ScalarRecord.
     * @param scalarType The scalar type.
     * @param minValue The minimum value.
     * @param maxValue The maximum value.
     * @param stepSize The amount to change each time the record is processed.
     * @return A shared pointer to ScalarRecord..
     */
    static ScalarRecordPtr create(
        std::string const & recordName,
        epics::pvData::ScalarType scalarType,
        double minValue,
        double maxValue,
        double stepSize);
    /**
     * standard init method required by PVRecord
     * @return true unless record name already exists.
     */
    virtual bool init();
    virtual void process();
    ~ScalarRecord();
private:
    ScalarRecord(
        std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure,
        double minValue,
        double maxValue,
        double stepSize);
    epics::pvData::PVScalarPtr pvValue;
    double minValue;
    double maxValue;
    double stepSize;
    bool stepPositive;
};

}}}

#endif  /* SCALAR_RECORD_H */
