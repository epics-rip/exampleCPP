/* supportRecord.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2019.06.01
 */
#ifndef NUMERICRECORD_H
#define NUMERICRECORD_H

#include <shareLib.h>

#include <pv/channelProviderLocal.h>
#include <pv/controlSupport.h>
#include <pv/scalarAlarmSupport.h>

namespace epics { namespace exampleCPP { namespace support {


class SupportRecord;
typedef std::tr1::shared_ptr<SupportRecord> SupportRecordPtr;

/**
 * @brief support for control and scalarAlarm for a support scalar record
 *
 * This is support for a record with a top level field that has type scalar.
 * It provides support for control and scalarAlarm
 */
class epicsShareClass SupportRecord :
    public epics::pvDatabase::PVRecord
{
public:
    POINTER_DEFINITIONS(SupportRecord);
    /**
     * Factory methods to create SupportRecord.
     * @param recordName The name for the SupportRecord.
     * @param scalarType The scalar type. It must be a support type.
     * @return A shared pointer to SupportRecord..
     */
    static SupportRecordPtr create(
        std::string const & recordName,epics::pvData::ScalarType scalarType);
    /**
     * standard init method required by PVRecord
     * @return true unless record name already exists.
     */
    virtual bool init();
    /**
     * @brief Remove the record specified by  recordName.
     */
    virtual void process();
    ~SupportRecord();
private:
    SupportRecord(
        std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure);
    epics::pvDatabase::ControlSupportPtr controlSupport;
    epics::pvDatabase::ScalarAlarmSupportPtr scalarAlarmSupport;
    epics::pvData::PVBooleanPtr pvReset;
};

}}}

#endif  /* NUMERICRECORD_H */
