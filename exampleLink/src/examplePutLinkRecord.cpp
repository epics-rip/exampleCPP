/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2016.06.17
 */

#include <pv/standardPVField.h>
#include <pv/ntscalar.h>
#include <pv/pvaClient.h>

#include <epicsExport.h>
#include <pv/examplePutLinkRecord.h>

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

class PutLinkRecordRequester :
    public PvaClientChannelStateChangeRequester,
    public PvaClientPutRequester
{
    ExamplePutLinkRecordWPtr examplePutLinkRecord;
    PvaClient::weak_pointer pvaClient;
public:
    POINTER_DEFINITIONS(PutLinkRecordRequester);

    PutLinkRecordRequester(
        ExamplePutLinkRecordPtr const & examplePutLinkRecord,
        PvaClientPtr const &pvaClient)
    : examplePutLinkRecord(examplePutLinkRecord),
      pvaClient(pvaClient)
    {}
    virtual ~PutLinkRecordRequester() {
        if(PvaClient::getDebug()) std::cout << "~PutLinkRecordRequester" << std::endl;
    }

    virtual void channelStateChange(PvaClientChannelPtr const & channel, bool isConnected)
    {
        ExamplePutLinkRecordPtr putLinkRecord(examplePutLinkRecord.lock());
        if(!putLinkRecord) return;
        putLinkRecord->channelStateChange(channel,isConnected);  
    }

    virtual void channelPutConnect(
        const epics::pvData::Status& status,
        PvaClientPutPtr const & clientPut)
    {
        ExamplePutLinkRecordPtr putLinkRecord(examplePutLinkRecord.lock());
        if(!putLinkRecord) return;
        putLinkRecord->channelPutConnect(status,clientPut);  
    }

    virtual void putDone(
        const epics::pvData::Status& status,
        PvaClientPutPtr const & clientPut) 
    {
        ExamplePutLinkRecordPtr putLinkRecord(examplePutLinkRecord.lock());
        if(!putLinkRecord) return;
        putLinkRecord->putDone(status,clientPut);  
    }

};

ExamplePutLinkRecordPtr ExamplePutLinkRecord::create(
    PvaClientPtr  const & pva,
    string const & recordName,
    string const & providerName,
    string const & channelName)
{
    PVStructurePtr pvStructure = getStandardPVField()->scalarArray(pvDouble,"timeStamp,alarm");
    ExamplePutLinkRecordPtr pvRecord(
        new ExamplePutLinkRecord(
           recordName,pvStructure)); 
    PutLinkRecordRequesterPtr linkRecordRequester(
        PutLinkRecordRequesterPtr(new PutLinkRecordRequester(pvRecord,pva)));
    pvRecord->linkRecordRequester = linkRecordRequester;
    if(!pvRecord->init(pva,channelName,providerName)) pvRecord.reset();
    return pvRecord;
}

ExamplePutLinkRecord::ExamplePutLinkRecord(
    string const & recordName,
    PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure),
  channelConnected(false),
  isPutConnected(false),
  isPutDone(true),
  setAlarmGood(false)
{
}


bool ExamplePutLinkRecord::init(
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

void ExamplePutLinkRecord::process()
{
    if(!channelConnected)
    {
        alarm.setMessage("disconnected");
        alarm.setSeverity(invalidAlarm);
        pvAlarm.set(alarm);
        setAlarmGood = true;
    } else if(!isPutConnected) 
    {
        alarm.setMessage("channelPut not connected");
        alarm.setSeverity(invalidAlarm);
        pvAlarm.set(alarm);
        setAlarmGood = true;
    } else if(!isPutDone) 
    {
        alarm.setMessage("previous put not done");
        alarm.setSeverity(invalidAlarm);
        pvAlarm.set(alarm);
        setAlarmGood = true;
    } else {
        try {
              shared_vector<const double> value(pvValue->view());
              pvaClientPut->getData()->putDoubleArray(pvValue->view());
              if(setAlarmGood) {
                setAlarmGood = false;
                alarm.setMessage("connected");
                alarm.setSeverity(noAlarm);
                pvAlarm.set(alarm);
            }
            isPutDone = false;
            pvaClientPut->put();
        } catch (std::runtime_error e) {
            alarm.setMessage(e.what());
            alarm.setSeverity(invalidAlarm);
            pvAlarm.set(alarm);
            setAlarmGood = true;
        }
    }
    PVRecord::process();
}

void ExamplePutLinkRecord::channelStateChange(
    PvaClientChannelPtr const & channel,
    bool isConnected)
{
    channelConnected = isConnected;
    if(isConnected) {
        setAlarmGood = true;
        if(!pvaClientPut) {
            pvaClientPut = pvaClientChannel->createPut("value");
            pvaClientPut->setRequester(linkRecordRequester);
            pvaClientPut->issueConnect();
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

void ExamplePutLinkRecord::channelPutConnect(
    const epics::pvData::Status& status,
    PvaClientPutPtr const & clientPut)
{
    if(status.isOK()) {
        isPutConnected = true;
        return;
    }
    lock();
        isPutConnected = false;
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

void ExamplePutLinkRecord::putDone(
    const epics::pvData::Status& status,
    PvaClientPutPtr const & clientPut)
{
    if(status.isOK()) {
        isPutDone = true;
        return;
    }
    lock();
        isPutDone = false;
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
