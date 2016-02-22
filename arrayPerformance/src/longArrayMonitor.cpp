// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/* longArrayMonitor.cpp */

/**
 * @author mrk
 * @date 2013.08.09
 */

#include <epicsThread.h>
#include <pv/pvaClient.h>

#define epicsExportSharedSymbols
#include <pv/longArrayMonitor.h>


namespace epics { namespace exampleCPP { namespace arrayPerformance { 

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;
using namespace std;

LongArrayMonitor::LongArrayMonitor(
   string  const &providerName,
    string  const & channelName,
    int queueSize)
: pva(PvaClient::create()),
  nElements(0),
  nSinceLastReport(0),
  threadName("longArrayMonitor")
{
    string  request("record[queueSize=");
    char buff[20];
    sprintf(buff,"%d",queueSize);
    request += buff;
    request += "]field(value,timeStamp,alarm)";
    monitor = pva->channel(channelName,providerName,2.0)->monitor(request);

    thread = std::auto_ptr<epicsThread>(new epicsThread(
        *this,
        threadName.c_str(),
        epicsThreadGetStackSize(epicsThreadStackSmall),
        epicsThreadPriorityLow));
     thread->start();
}

void LongArrayMonitor::destroy()
{
    runStop.signal();
    runReturn.wait();
}

void LongArrayMonitor::run()
{
    while(true) {   
        if(runStop.tryWait()) {
             runReturn.signal();
             return;
        }    
        nextMonitor();
    }
}



void LongArrayMonitor::nextMonitor()
{
   if(!monitor->waitEvent(0.0)) {
	cout << "waitEvent returned false. Why???" << endl;
    	return;
    }
    PvaClientMonitorDataPtr pvaData = monitor->getData();
	PVStructurePtr pvStructure = pvaData->getPVStructure();
	pvTimeStamp.attach(pvStructure->getSubField<PVStructure>("timeStamp"));
	pvTimeStamp.get(timeStamp);
	PVLongArrayPtr pvValue = pvStructure->getSubField<PVLongArray>("value");

    shared_vector<const int64> data = pvValue->view();
    if(data.size()>0) {
        nElements += data.size();
        int64 first = data[0];
        int64 last = data[data.size()-1];
        if(first!=last) {
            cout << "error first=" << first << " last=" << last << endl;
        }
        double diff = TimeStamp::diff(timeStamp,timeStampLast);
        if(diff>=1.0) {
            ostringstream out;
            out << " monitors/sec " << nSinceLastReport << " ";
            out << "first " << first << " last " << last ;
            out << " changed " << *pvaData->getChangedBitSet();
            out << " overrun " << *pvaData->getOverrunBitSet();
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
            nSinceLastReport = 0;
            nElements = 0;
        }
        ++nSinceLastReport;
    } else {
        cout << "size = 0" << endl;
    }
    monitor->releaseEvent();
}

}}}


