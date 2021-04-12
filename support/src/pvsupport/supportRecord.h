/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2021.04.16
 */
#ifndef SUPPORTRECORD_H
#define SUPPORTRECORD_H

#include <pv/pvDatabase.h>
#include <pv/pvSupport.h>
#include <pv/pvStructureCopy.h>

#include <shareLib.h>

namespace epics { namespace example { namespace support {

class SupportRecord;
typedef std::tr1::shared_ptr<SupportRecord> SupportRecordPtr;

class epicsShareClass SupportRecord :
    public epics::pvDatabase::PVRecord
{
public:
    POINTER_DEFINITIONS(SupportRecord);
    static SupportRecordPtr create(
        std::string const & recordName,std::string const & scalarType,
        int asLevel=0,std::string const & asGroup=std::string("DEFAULT"));
    virtual bool init();
    virtual void process();
    ~SupportRecord(){}
private:
    SupportRecord(
        std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure,
        int asLevel,std::string const & asGroup);
    epics::pvDatabase::ControlSupportPtr controlSupport;
    epics::pvDatabase::ScalarAlarmSupportPtr scalarAlarmSupport;
    epics::pvData::PVBooleanPtr pvReset;
};

}}}

#endif
