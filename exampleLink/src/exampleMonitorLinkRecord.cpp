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

class LinkRecordRequesterImpl :
    public PvaClientChannelStateChangeRequester,
    public PvaClientMonitorRequester
{
    ExampleMonitorLinkRecordWPtr exampleMonitorLinkRecord;
    PvaClient::weak_pointer pvaClient;
public:
    POINTER_DEFINITIONS(LinkRecordRequesterImpl);

    LinkRecordRequesterImpl(
        ExampleMonitorLinkRecordPtr const & exampleMonitorLinkRecord,
        PvaClientPtr const &pvaClient)
    : exampleMonitorLinkRecord(exampleMonitorLinkRecord),
      pvaClient(pvaClient)
    {}
    virtual ~LinkRecordRequesterImpl() {
        if(PvaClient::getDebug()) std::cout << "~LinkRecordRequesterImpl" << std::endl;
    }

    virtual void channelStateChange(PvaClientChannelPtr const & channel, bool isConnected)
    {
        ExampleMonitorLinkRecordPtr monitorLinkRecord(exampleMonitorLinkRecord.lock());
        if(!monitorLinkRecord) return;
        monitorLinkRecord->channelStateChange(channel,isConnected);  
    }

    virtual void monitorConnect(
        Status const & status,
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
        pvDouble,"timeStamp");
    ExampleMonitorLinkRecordPtr pvRecord(
        new ExampleMonitorLinkRecord(
           recordName,pvStructure)); 
    LinkRecordRequesterImplPtr linkRecordRequester(
        LinkRecordRequesterImplPtr(new LinkRecordRequesterImpl(pvRecord,pva)));
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
  isStarted(false)
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
        return false;
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
        if(!pvaClientMonitor) {
            pvaClientMonitor = pvaClientChannel->createMonitor("value");
            pvaClientMonitor->setRequester(linkRecordRequester);
            pvaClientMonitor->issueConnect();
        }
    }
}

void ExampleMonitorLinkRecord::monitorConnect(
    Status const & status,
    PvaClientMonitorPtr const & monitor,
    StructureConstPtr const & structure)
{
    if(!status.isOK()) return;
    monitorConnected = true;
    if(isStarted) return;
    isStarted = true;
    pvaClientMonitor->start();
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
