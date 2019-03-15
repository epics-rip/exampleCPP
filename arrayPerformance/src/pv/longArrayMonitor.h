/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.08.09
 */
#ifndef LONGARRAYMONITOR_H
#define LONGARRAYMONITOR_H

#include <epicsThread.h>
#include <pv/pvaClient.h>

#include <shareLib.h>


namespace epics { namespace exampleCPP { namespace arrayPerformance {

typedef std::tr1::shared_ptr<epicsThread> EpicsThreadPtr;

class LongArrayMonitor;
typedef std::tr1::shared_ptr<LongArrayMonitor> LongArrayMonitorPtr;

class epicsShareClass  LongArrayMonitor :
    public epicsThreadRunable
{
public:
    LongArrayMonitor(
        std::string const & channelName,
        int queueSize = 2);
    virtual void run();
    void stop();
private:
    std::string channelName;
    int queueSize;
    EpicsThreadPtr thread;
    epics::pvData::Event runStop;
    epics::pvData::Event runReturn;
};


}}}

#endif  /* LONGARRAYMONITOR_H */
