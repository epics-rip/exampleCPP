// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/* longArrayGet.cpp */

/**
 * @author mrk
 * @date 2013.08.09
 */


#define epicsExportSharedSymbols
#include <pv/longArrayGet.h>

namespace epics { namespace exampleCPP { namespace arrayPerformance { 

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

LongArrayGet::LongArrayGet(
        string  providerName,
        string  channelName,
        int iterBetweenCreateChannel,
        int iterBetweenCreateChannelGet,
        double delayTime)
    : providerName(providerName),
      channelName(channelName),
      iterBetweenCreateChannel(iterBetweenCreateChannel),
      iterBetweenCreateChannelGet(iterBetweenCreateChannelGet),
      delayTime(delayTime),
      threadName("longArrayGet")
{
     thread = std::auto_ptr<epicsThread>(new epicsThread(
        *this,
        threadName.c_str(),
        epicsThreadGetStackSize(epicsThreadStackSmall),
        epicsThreadPriorityLow));
     thread->start();
}

void LongArrayGet::destroy()
{
    runStop.signal();
    runReturn.wait();
}

void LongArrayGet::run()
{
    PvaClientPtr pva(PvaClient::create());
    PvaClientChannelPtr pvaChannel(pva->createChannel(channelName,providerName));
    PvaClientGetPtr pvaGet(pvaChannel->createGet("value,alarm,timeStamp"));
    TimeStamp timeStamp;
    TimeStamp timeStampLast;
    timeStampLast.getCurrent();
    int numChannelGet = 0;
    int numChannelCreate = 0;
    size_t nElements = 0;
    while(true) {
        if(runStop.tryWait()) {
             runReturn.signal();
             return;
        }
        pvaGet->get();
        PvaClientGetDataPtr pvaData = pvaGet->getData();
        PVStructurePtr pvStructure = pvaData->getPVStructure();
        BitSetPtr bitSet = pvaData->getChangedBitSet();
        size_t latestSize = checkResult(pvStructure,bitSet);
        nElements += latestSize;
        timeStamp.getCurrent();
        double diff = TimeStamp::diff(timeStamp,timeStampLast);
        if(diff>=1.0) {
            ostringstream out;
            out << "get";
            double elementsPerSec = nElements;
            elementsPerSec /= diff;
            if(elementsPerSec>10.0e9) {
                 elementsPerSec /= 1e9;
                 out << " gigaElements/sec " << elementsPerSec;
            } else if(elementsPerSec>10.0e6) {
                 elementsPerSec /= 1e6;
                 out << " megaElements/sec " << elementsPerSec;
            } else if(elementsPerSec>10.0e3) {
                 elementsPerSec /= 1e3;
                 out << " kiloElements/sec " << elementsPerSec;
            } else  {
                 out << " Elements/sec " << elementsPerSec;
            }
            cout << out.str() << endl;
            timeStampLast = timeStamp;
            nElements = 0;
        }
        if(delayTime>0.0) epicsThreadSleep(delayTime);
        ++numChannelGet;
        bool createGet = false;
        if(iterBetweenCreateChannelGet!=0) {
            if(numChannelGet>=iterBetweenCreateChannelGet) createGet = true;
        }
        if(createGet) {
             numChannelGet = 0;
             pvaGet->destroy();
             pvaGet = pvaChannel->createGet("value,timeStamp,alarm");
        }
        ++numChannelCreate;
        if(iterBetweenCreateChannel!=0) {
            if(numChannelCreate>=iterBetweenCreateChannel) {
                pvaChannel->destroy();
                pvaChannel = pva->createChannel(channelName,providerName);
                pvaGet = pvaChannel->createGet("value,timeStamp,alarm");
                numChannelCreate = 0;
            }
        }
    }
}

size_t LongArrayGet::checkResult(const PVStructurePtr &pvStructure, const BitSetPtr & bitSet)
{
    PVLongArrayPtr pvValue = pvStructure->getSubField<PVLongArray>("value");
    if(!bitSet->get(pvValue->getFieldOffset())) {
        return 0;
    }
    bitSet->clear();
    shared_vector<const int64> data = pvValue->view();
    if(data.size()>0) {
        int64 first = data[0];
        int64 last = data[data.size()-1];
        if(first!=last) {
           cout << "error first=" << first << " last=" << last << endl;
        }
    }
    return data.size();
}

}}}


