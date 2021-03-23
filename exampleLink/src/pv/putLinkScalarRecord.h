/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2021.03.16
 */
#ifndef PUTLINKSCALARRECORD_H
#define PUTLINKSCALARRECORD_H

#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>
#include <pv/alarm.h>
#include <pv/pvAlarm.h>
#include <pv/pvDatabase.h>
#include <pv/pvaClient.h>

#include <shareLib.h>

namespace epics { namespace exampleCPP { namespace exampleLink {


class PutLinkScalarRecord;
typedef std::tr1::shared_ptr<PutLinkScalarRecord> PutLinkScalarRecordPtr;
typedef std::tr1::weak_ptr<PutLinkScalarRecord> PutLinkScalarRecordWPtr;


class epicsShareClass PutLinkScalarRecord :
    public epics::pvDatabase::PVRecord
{
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
    epics::pvDatabase::PVRecordPtr linkRecord;
    epics::pvaClient::PvaClientChannelPtr linkChannel;
    epics::pvaClient::PvaClientPutPtr clientPut;
    epics::pvData::PVBooleanPtr pvReconnect;
    void clientProcess();
    void databaseProcess();
public:
    POINTER_DEFINITIONS(PutLinkScalarRecord);
    static PutLinkScalarRecordPtr create(std::string const & recordName);
    virtual ~PutLinkScalarRecord() {}
    virtual void process();
    virtual bool init();
};

}}}

#endif  /* PUTLINKSCALARRECORD_H */
