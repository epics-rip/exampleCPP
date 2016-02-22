// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/* longArrayMonitor.h */

/**
 * @author mrk
 * @date 2013.08.09
 */
#ifndef LONGARRAYMONITOR_H
#define LONGARRAYMONITOR_H

#ifdef epicsExportSharedSymbols
#   define longarraymonitorEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <epicsThread.h>
#include <pv/pvaClient.h>


#ifdef longarraymonitorEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef longarraymonitorEpicsExportSharedSymbols
#   include <shareLib.h>
#endif


namespace epics { namespace exampleCPP { namespace arrayPerformance {


class LongArrayMonitor;
typedef std::tr1::shared_ptr<LongArrayMonitor> LongArrayMonitorPtr;

class epicsShareClass  LongArrayMonitor :
    public epicsThreadRunable
{
public:
    LongArrayMonitor(
        std::string const & providerName,
        std::string const & channelName,
        int queueSize = 1);
    virtual void run();
    void destroy();
    void  nextMonitor();
private:
    epics::pvaClient::PvaClientPtr pva;
    epics::pvaClient::PvaClientMonitorPtr monitor;
    epics::pvData::PVTimeStamp pvTimeStamp;
    epics::pvData::TimeStamp timeStamp;
    epics::pvData::TimeStamp timeStampLast;
    long nElements;
    long nSinceLastReport;
    std::string threadName;
    std::auto_ptr<epicsThread> thread;
    epics::pvData::Event runStop;
    epics::pvData::Event runReturn;
};


}}}

#endif  /* LONGARRAYMONITOR_H */
