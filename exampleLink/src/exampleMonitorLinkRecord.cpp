/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.08.02
 */

#include <pv/standardPVField.h>
#include <pv/ntscalar.h>
#include <pv/pvaClient.h>

#define epicsExportSharedSymbols
#include <pv/exampleMonitorLinkRecord.h>

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

class MonitorLinkRecordRequester :
    public PvaClientChannelStateChangeRequester,
    public PvaClientMonitorRequester
{
    ExampleMonitorLinkRecordWPtr exampleMonitorLinkRecord;
    PvaClient::weak_pointer pvaClient;
public:
    POINTER_DEFINITIONS(MonitorLinkRecordRequester);

    MonitorLinkRecordRequester(
        ExampleMonitorLinkRecordPtr const & exampleMonitorLinkRecord,
        PvaClientPtr const &pvaClient)
    : exampleMonitorLinkRecord(exampleMonitorLinkRecord),
      pvaClient(pvaClient)
    {}
    virtual ~MonitorLinkRecordRequester() {
        if(PvaClient::getDebug()) std::cout << "~MonitorLinkRecordRequester" << std::endl;
    }

    virtual void channelStateChange(PvaClientChannelPtr const & channel, bool isConnected)
    {
        ExampleMonitorLinkRecordPtr monitorLinkRecord(exampleMonitorLinkRecord.lock());
        if(!monitorLinkRecord) return;
        monitorLinkRecord->channelStateChange(channel,isConnected);  
    }

    virtual void monitorConnect(
        const Status& status,
        PvaClientMonitorPtr const & monitor,
        StructureConstPtr const & structure)
    {
        ExampleMonitorLinkRecordPtr monitorLinkRecord(exampleMonitorLinkRecord.lock());
        if(!monitorLinkRecord) return;
        monitorLinkRecord->monitorConnect(status,monitor,structure);  
    }

    virtual void event(PvaClientMonitorPtr const & monitor)
    {
        ExampleMonitorLinkRecordPtr monitorLinkRecord(exampleMonitorLinkRecord.lock());
        if(!monitorLinkRecord) return;
        monitorLinkRecord->event(monitor);  
    }   
};


ExampleMonitorLinkRecordPtr ExampleMonitorLinkRecord::create(
    PvaClientPtr  const & pva,
    string const & recordName,
    string const & providerName,
    string const & channelName)
{
    PVStructurePtr pvStructure = getStandardPVField()->scalarArray(
        pvDouble,"timeStamp,alarm");
    ExampleMonitorLinkRecordPtr pvRecord(
        new ExampleMonitorLinkRecord(recordName,pvStructure)); 
    
    MonitorLinkRecordRequesterPtr linkRecordRequester(
        MonitorLinkRecordRequesterPtr(new MonitorLinkRecordRequester(pvRecord,pva)));
    pvRecord->linkRecordRequester = linkRecordRequester;
    if(!pvRecord->init(pva,channelName,providerName)) pvRecord.reset();
    return pvRecord;
}

ExampleMonitorLinkRecord::ExampleMonitorLinkRecord(
    string const & recordName,
    PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure),
  channelConnected(false),
  monitorConnected(false),
  setAlarmGood(false)
{
}


bool ExampleMonitorLinkRecord::init(
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
    pvaClientChannel = pvaClient->createChannel(channelName,providerName);
    pvaClientChannel->setStateChangeRequester(linkRecordRequester);
    pvaClientChannel->issueConnect();
    return true;
}

void ExampleMonitorLinkRecord::process()
{
    PVRecord::process();
}

void ExampleMonitorLinkRecord::channelStateChange(
    PvaClientChannelPtr const & channel, bool isConnected)
{
    channelConnected = isConnected;
    if(isConnected) {
        setAlarmGood = true;
        if(!pvaClientMonitor) {
            pvaClientMonitor = pvaClientChannel->createMonitor("value");
            pvaClientMonitor->setRequester(linkRecordRequester);
            pvaClientMonitor->issueConnect();
        }
        return;
    }
    lock();
    try {
        beginGroupPut();
        alarm.setMessage("disconnected");
        alarm.setSeverity(invalidAlarm);
        pvAlarm.set(alarm);
        process();
        endGroupPut();
    } catch(...) {
       unlock();
       throw;
    }
    unlock();
}

void ExampleMonitorLinkRecord::monitorConnect(
    const Status& status,
    PvaClientMonitorPtr const & monitor,
    StructureConstPtr const & structure)
{
    if(status.isOK()) {
        monitorConnected = true;
        return;
    }
    lock();
        monitorConnected = false;
        try {
            beginGroupPut();
            alarm.setMessage(status.getMessage());
            alarm.setSeverity(invalidAlarm);
            pvAlarm.set(alarm);
            process();
            endGroupPut();
        } catch(...) {
           unlock();
           throw;
        }
    unlock();
}

void ExampleMonitorLinkRecord::event(PvaClientMonitorPtr const & monitor)
{
    while(monitor->poll()) {
        PVStructurePtr pvStructure = monitor->getData()->getPVStructure();
        PVDoubleArrayPtr pvDoubleArray = pvStructure->getSubField<PVDoubleArray>("value");
        if(!pvDoubleArray) throw std::runtime_error("value is not a double array");
        lock();
        try {
            beginGroupPut();
            if(setAlarmGood) {
                setAlarmGood = false;
                alarm.setMessage("connected");
                alarm.setSeverity(noAlarm);
                pvAlarm.set(alarm);
            }
            pvValue->replace(pvDoubleArray->view());
            process();
            endGroupPut();
        } catch(...) {
           unlock();
           throw;
        }
        unlock();
        monitor->releaseEvent();
    }
}

}}}
