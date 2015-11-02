/* exampleLink.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2013.08.02
 */

#include <pv/standardPVField.h>

#define epicsExportSharedSymbols
#include <pv/exampleLink.h>

using namespace epics::pvData;
using namespace epics::pvAccess;
using std::tr1::static_pointer_cast;
using std::tr1::dynamic_pointer_cast;
using std::cout;
using std::endl;
using std::string;

namespace epics { namespace pvDatabase { 

ExampleLinkPtr ExampleLink::create(
    string const & recordName,
    string const & providerName,
    string const & channelName)
{
    PVStructurePtr pvStructure = getStandardPVField()->scalarArray(
        pvDouble,"timeStamp");
    ExampleLinkPtr pvRecord(
        new ExampleLink(
           recordName,providerName,channelName,pvStructure));    
    if(!pvRecord->init()) pvRecord.reset();
    return pvRecord;
}

ExampleLink::ExampleLink(
    string const & recordName,
    string providerName,
    string channelName,
    PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure),
  providerName(providerName),
  channelName(channelName)
{
}

void ExampleLink::destroy()
{
    PVRecord::destroy();
}

bool ExampleLink::init()
{
    initPVRecord();

    PVStructurePtr pvStructure = getPVRecordStructure()->getPVStructure();
    pvValue = pvStructure->getSubField<PVDoubleArray>("value");
    if(!pvValue) {
        return false;
    }
    ChannelProvider::shared_pointer provider =
        getChannelProviderRegistry()->getProvider(providerName);
    if(!provider) {
         cout << getRecordName() << " provider "
              << providerName << " does not exist" << endl;
        return false;
    }
    ChannelRequester::shared_pointer channelRequester =
        dynamic_pointer_cast<ChannelRequester>(getPtrSelf());
    channel = provider->createChannel(channelName,channelRequester);
    event.wait();
    if(!status.isOK()) {
        cout << getRecordName() << " createChannel failed "
             << status.getMessage() << endl;
        return false;
    }
    PVStructurePtr pvRequest = CreateRequest::create()->createRequest(
        "value");
    MonitorRequester::shared_pointer  monitorRequester =
        dynamic_pointer_cast<MonitorRequester>(getPtrSelf());
    monitor = channel->createMonitor(monitorRequester,pvRequest);
    return true;
}

void ExampleLink::process()
{
    PVRecord::process();
}

void ExampleLink::channelCreated(
        const Status& status,
        Channel::shared_pointer const & channel)
{
    this->status = status;
    this->channel = channel;
    event.signal();
}

void ExampleLink::channelStateChange(
        Channel::shared_pointer const & channel,
        Channel::ConnectionState connectionState)
{
}

void ExampleLink::monitorConnect(
        const epics::pvData::Status& status,
        epics::pvData::Monitor::shared_pointer const & monitor,
        epics::pvData::StructureConstPtr const & structure)
{
   monitor->start();
}

void ExampleLink::monitorEvent(epics::pvData::MonitorPtr const & monitor)
{
    while(true) {
        MonitorElementPtr monitorElement = monitor->poll();
        if(!monitorElement) break;
        PVStructurePtr pvStructurePtr = monitorElement->pvStructurePtr;
        PVDoubleArrayPtr pvDoubleArray = pvStructurePtr->getSubField<PVDoubleArray>("value");
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
        monitor->release(monitorElement);
    }
}

void ExampleLink::unlisten(epics::pvData::MonitorPtr const & monitor)
{
}

}}
