/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2021.04.16
 */
#ifndef CONTROLRECORD_H
#define CONTROLRECORD_H

#include <pv/pvDatabase.h>
#include <pv/pvSupport.h>
#include <pv/pvStructureCopy.h>

#include <shareLib.h>

namespace epics { namespace example { namespace control {

class ControlRecord;
typedef std::tr1::shared_ptr<ControlRecord> ControlRecordPtr;

class epicsShareClass ControlRecord :
    public epics::pvDatabase::PVRecord
{
public:
    POINTER_DEFINITIONS(ControlRecord);
    static ControlRecordPtr create(
        std::string const & recordName,std::string const & scalarType,
        int asLevel=0,std::string const & asGroup=std::string("DEFAULT"));
    virtual bool init();
    virtual void process();
    ~ControlRecord(){}
private:
    ControlRecord(
        std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure,
        int asLevel,std::string const & asGroup);
    epics::pvDatabase::ControlSupportPtr controlSupport;
    epics::pvDatabase::ScalarAlarmSupportPtr scalarAlarmSupport;
    epics::pvData::PVBooleanPtr pvReset;
};

}}}

#endif
