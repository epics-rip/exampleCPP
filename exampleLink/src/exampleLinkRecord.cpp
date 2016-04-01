// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/* exampleLink.cpp */

/**
 * @author mrk
 * @date 2013.08.02
 */

#include <pv/standardPVField.h>
#include <pv/ntscalar.h>

#define epicsExportSharedSymbols
#include <pv/exampleLinkRecord.h>

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using std::tr1::static_pointer_cast;
using std::tr1::dynamic_pointer_cast;
using std::cout;
using std::endl;
using std::string;

namespace epics { namespace exampleCPP { namespace exampleLink {

ExampleLinkRecordPtr ExampleLinkRecord::create(
    string const & recordName,
    string const & providerName,
    string const & channelName)
{
    PVStructurePtr pvStructure = getStandardPVField()->scalarArray(
        pvDouble,"timeStamp");
    ExampleLinkRecordPtr pvRecord(
        new ExampleLinkRecord(
           recordName,providerName,channelName,pvStructure));    
    if(!pvRecord->init()) pvRecord.reset();
    return pvRecord;
}

ExampleLinkRecord::ExampleLinkRecord(
    string const & recordName,
    string providerName,
    string channelName,
    PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure),
  providerName(providerName),
  channelName(channelName)
{
}

void ExampleLinkRecord::destroy()
{
    PVRecord::destroy();
}

bool ExampleLinkRecord::init()
{
    initPVRecord();

    PVStructurePtr pvStructure = getPVRecordStructure()->getPVStructure();
    pvValue = pvStructure->getSubField<PVDoubleArray>("value");
    if(!pvValue) {
        return false;
    }
cout<< "ExampleLinkRecord::init() provider " << providerName << endl;
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

void ExampleLinkRecord::process()
{
    PVRecord::process();
}

void ExampleLinkRecord::channelCreated(
        const Status& status,
        Channel::shared_pointer const & channel)
{
cout << "ExampleLinkRecord::channelCreated status " << status << endl;
    this->status = status;
    this->channel = channel;
    event.signal();
}

void ExampleLinkRecord::channelStateChange(
        Channel::shared_pointer const & channel,
        Channel::ConnectionState connectionState)
{
cout << "ExampleLinkRecord::channelStateChange connectionState " << connectionState << endl;
}

void ExampleLinkRecord::monitorConnect(
        const epics::pvData::Status& status,
        epics::pvData::Monitor::shared_pointer const & monitor,
        epics::pvData::StructureConstPtr const & structure)
{
cout << "ExampleLinkRecord::monitorConnect status " << status << endl;
   monitor->start();
}

void ExampleLinkRecord::monitorEvent(epics::pvData::MonitorPtr const & monitor)
{
cout << "ExampleLinkRecord::monitorEvent\n";
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

void ExampleLinkRecord::unlisten(epics::pvData::MonitorPtr const & monitor)
{
}

}}}
