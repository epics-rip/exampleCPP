/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.04.02
 */
#ifndef PUTLINKSCALARARRAYRECORD_H
#define PUTLINKSCALARARRAYRECORD_H

#include <pv/timeStamp.h>
#include <pv/alarm.h>
#include <pv/pvTimeStamp.h>
#include <pv/pvAlarm.h>
#include <pv/pvDatabase.h>

#include <shareLib.h>

namespace epics { namespace example { namespace linkRecord {

class PutLinkScalarArrayRecord;
typedef std::tr1::shared_ptr<PutLinkScalarArrayRecord> PutLinkScalarArrayRecordPtr;

class epicsShareClass PutLinkScalarArrayRecord :
    public epics::pvDatabase::PVRecord
{
public:
    POINTER_DEFINITIONS(PutLinkScalarArrayRecord);
    static PutLinkScalarArrayRecordPtr create(
        std::string const & recordName);
    virtual ~PutLinkScalarArrayRecord() {}
    virtual bool init();
    virtual void process();
    
private:
    PutLinkScalarArrayRecord(
        std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure);    
    epics::pvData::PVStringArrayPtr pvValue;      
    epics::pvData::PVStringPtr pvLink;
    epics::pvData::PVStringPtr pvAccessMethod;
    epics::pvData::PVStructurePtr pvAlarmField;
    epics::pvData::PVAlarm pvAlarm;
    epics::pvData::Alarm alarm;
    epics::pvDatabase::PVRecordWPtr linkRecord;
    epics::pvaClient::PvaClientChannelPtr linkChannel;
    epics::pvaClient::PvaClientPutPtr clientPut;
    epics::pvData::PVBooleanPtr pvReconnect;
    void clientProcess();
    void databaseProcess();
};


}}}

#endif  /* PUTLINKSCALARARRAYRECORD_H */
