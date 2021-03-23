/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2021.03.16
 */
#ifndef PUTLINKSCALARARRAYRECORD_H
#define PUTLINKSCALARARRAYRECORD_H

#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>
#include <pv/alarm.h>
#include <pv/pvAlarm.h>
#include <pv/pvDatabase.h>
#include <pv/pvaClient.h>

#include <shareLib.h>

namespace epics { namespace exampleCPP { namespace exampleLink {


class PutLinkScalarArrayRecord;
typedef std::tr1::shared_ptr<PutLinkScalarArrayRecord> PutLinkScalarArrayRecordPtr;
typedef std::tr1::weak_ptr<PutLinkScalarArrayRecord> PutLinkScalarArrayRecordWPtr;


class epicsShareClass PutLinkScalarArrayRecord :
    public epics::pvDatabase::PVRecord
{
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
    epics::pvDatabase::PVRecordPtr linkRecord;
    epics::pvaClient::PvaClientChannelPtr linkChannel;
    epics::pvaClient::PvaClientPutPtr clientPut;
    epics::pvData::PVBooleanPtr pvReconnect;
    void clientProcess();
    void databaseProcess();
public:
    POINTER_DEFINITIONS(PutLinkScalarArrayRecord);
    static PutLinkScalarArrayRecordPtr create(std::string const & recordName);
    virtual ~PutLinkScalarArrayRecord() {}
    virtual void process();
    virtual bool init();

};

}}}

#endif  /* PUTLINKSCALARARRAYRECORD_H */
