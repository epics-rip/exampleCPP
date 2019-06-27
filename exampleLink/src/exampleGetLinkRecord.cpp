/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.08.02
 */

#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>
#include <pv/alarm.h>
#include <pv/pvAlarm.h>
#include <pv/pvDatabase.h>
#include <pv/pvaClient.h>
#include <pv/standardPVField.h>
#include <pv/ntscalar.h>

#define epicsExportSharedSymbols
#include "pv/exampleGetLinkRecord.h"

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;
using namespace epics::pvDatabase;
using std::tr1::static_pointer_cast;
using std::tr1::dynamic_pointer_cast;
using std::cout;
using std::endl;
using std::string;

namespace epics { namespace exampleCPP { namespace exampleLink {

class GetLinkRecordRequester :
    public PvaClientChannelStateChangeRequester,
    public PvaClientGetRequester
{
    ExampleGetLinkRecordWPtr exampleGetLinkRecord;
    PvaClient::weak_pointer pvaClient;
public:
    POINTER_DEFINITIONS(GetLinkRecordRequester);

    GetLinkRecordRequester(
        ExampleGetLinkRecordPtr const & exampleGetLinkRecord,
        PvaClientPtr const &pvaClient)
    : exampleGetLinkRecord(exampleGetLinkRecord),
      pvaClient(pvaClient)
    {}
    virtual ~GetLinkRecordRequester() {
        if(PvaClient::getDebug()) std::cout << "~GetLinkRecordRequester" << std::endl;
    }

    virtual void channelStateChange(PvaClientChannelPtr const & channel, bool isConnected)
    {
        ExampleGetLinkRecordPtr getLinkRecord(exampleGetLinkRecord.lock());
        if(!getLinkRecord) return;
        getLinkRecord->channelStateChange(channel,isConnected);  
    }

    virtual void channelGetConnect(
        const Status& status,
        PvaClientGetPtr const & clientGet)
    {
        ExampleGetLinkRecordPtr getLinkRecord(exampleGetLinkRecord.lock());
        if(!getLinkRecord) return;
        getLinkRecord->channelGetConnect(status,clientGet);  
    }

    virtual void getDone(
        const Status& status,
        PvaClientGetPtr const & clientGet) 
    {
// nothing to do
    }

};

ExampleGetLinkRecordPtr ExampleGetLinkRecord::create(
    PvaClientPtr  const & pva,
    string const & recordName,
    string const & providerName,
    string const & channelName)
{
    PVStructurePtr pvStructure = getStandardPVField()->scalarArray(pvDouble,"timeStamp,alarm");
    ExampleGetLinkRecordPtr pvRecord(
        new ExampleGetLinkRecord(
           recordName,pvStructure)); 
    GetLinkRecordRequesterPtr linkRecordRequester(
        GetLinkRecordRequesterPtr(new GetLinkRecordRequester(pvRecord,pva)));
    pvRecord->linkRecordRequester = linkRecordRequester;
    if(!pvRecord->init(pva,channelName,providerName)) pvRecord.reset();
    return pvRecord;
}

ExampleGetLinkRecord::ExampleGetLinkRecord(
    string const & recordName,
    PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure),
  channelConnected(false),
  isGetConnected(false)
{
}


bool ExampleGetLinkRecord::init(
    PvaClientPtr const & pvaClient,
    string const & channelName,
    string const & providerName)
{
    initPVRecord();

    PVStructurePtr pvStructure = getPVRecordStructure()->getPVStructure();
    pvValue = pvStructure->getSubField<PVDoubleArray>("value");
    if(!pvValue) {
        throw std::runtime_error("value is not a double array");
    }
    pvAlarmField = pvStructure->getSubField<PVStructure>("alarm");
    if(!pvAlarmField) {
        throw std::runtime_error("no alarm field");
    }
    if(!pvAlarm.attach(pvAlarmField)) {
        throw std::runtime_error(string("bad alarm field"));
    }
    alarm.setMessage("never connected");
    alarm.setSeverity(invalidAlarm);
    alarm.setStatus(clientStatus);
    pvAlarm.set(alarm);
    pvaClientChannel = pvaClient->createChannel(channelName,providerName);
    pvaClientChannel->setStateChangeRequester(linkRecordRequester);
    pvaClientChannel->issueConnect();
    return true;
}

void ExampleGetLinkRecord::process()
{
    if(!channelConnected)
    {
        alarm.setMessage("disconnected");
        alarm.setSeverity(invalidAlarm);
        pvAlarm.set(alarm);
    } else if(!isGetConnected) 
    {
        alarm.setMessage("channelGet not connected");
        alarm.setSeverity(invalidAlarm);
        pvAlarm.set(alarm);
    } else {
        try {
            pvaClientGet->get();
            PVStructurePtr pvStructure = pvaClientGet->getData()->getPVStructure();
            shared_vector<const double> value = pvaClientGet->getData()->getDoubleArray();
            pvValue->replace(value);
            bool setAlarm = false;
            PVStructurePtr linkAlarmField = pvStructure->getSubField<PVStructure>("alarm");
            if(linkAlarmField && linkPVAlarm.attach(linkAlarmField)) {
                linkPVAlarm.get(linkAlarm);
            } else {
                linkAlarm.setMessage("connected");
                linkAlarm.setSeverity(noAlarm);
                linkAlarm.setStatus(clientStatus);
            }
            if(alarm.getMessage()!=linkAlarm.getMessage()) {
                alarm.setMessage(linkAlarm.getMessage());
                setAlarm = true;
            }
            if(alarm.getSeverity()!=linkAlarm.getSeverity()) {
                alarm.setSeverity(linkAlarm.getSeverity());
                setAlarm = true;
            }
            if(alarm.getStatus()!=linkAlarm.getStatus()) {
                alarm.setStatus(linkAlarm.getStatus());
                setAlarm = true;
             }
            if(setAlarm) pvAlarm.set(alarm);
        } catch (std::exception& e) {
            alarm.setMessage(e.what());
            alarm.setSeverity(invalidAlarm);
            pvAlarm.set(alarm);
        }
    }
    PVRecord::process();
}

void ExampleGetLinkRecord::channelStateChange(
    PvaClientChannelPtr const & channel,
    bool isConnected)
{
    channelConnected = isConnected;
    if(isConnected) {
        if(!pvaClientGet) {
            pvaClientGet = pvaClientChannel->createGet("value,alarm");
            pvaClientGet->setRequester(linkRecordRequester);
            pvaClientGet->issueConnect();
        }
        return;
    }
    lock();
    try {
        beginGroupPut();
        process();
        endGroupPut();
    } catch(...) {
       unlock();
       throw;
    }
    unlock();
}

void ExampleGetLinkRecord::channelGetConnect(
    const Status& status,
    PvaClientGetPtr const & clientGet)
{
    if(status.isOK()) {
        isGetConnected = true;
        return;
    }
    lock();
        isGetConnected = false;
        try {
            beginGroupPut();
            process();
            endGroupPut();
        } catch(...) {
           unlock();
           throw;
        }
    unlock();
}

}}}
