/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.04.02
 */
#ifndef PUTLINKSCALARRECORD_H
#define PUTLINKSCALARRECORD_H

#include <pv/timeStamp.h>
#include <pv/alarm.h>
#include <pv/pvTimeStamp.h>
#include <pv/pvAlarm.h>
#include <pv/pvDatabase.h>
#include <shareLib.h>

namespace epics { namespace example { namespace linkRecord {

class PutLinkScalarRecord;
typedef std::tr1::shared_ptr<PutLinkScalarRecord> PutLinkScalarRecordPtr;

class epicsShareClass PutLinkScalarRecord :
    public epics::pvDatabase::PVRecord
{
public:
    POINTER_DEFINITIONS(PutLinkScalarRecord);
    static PutLinkScalarRecordPtr create(
        std::string const & recordName);
    virtual ~PutLinkScalarRecord() {}
    virtual bool init();
    virtual void process();
    
private:
    PutLinkScalarRecord(
        std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure);    
    epics::pvData::PVStringPtr pvValue;      
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

#endif  /* PUTLINKSCALARRECORD_H */
