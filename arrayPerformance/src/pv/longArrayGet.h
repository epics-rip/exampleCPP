// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/* longArrayGet.h */

/**
 * @author mrk
 * @date 2013.08.09
 */
#ifndef LONGARRAYGET_H
#define LONGARRAYGET_H


#ifdef epicsExportSharedSymbols
#   define longarraygetEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <epicsThread.h>
#include <pv/pvaClient.h>

#ifdef longarraygetEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef longarraygetEpicsExportSharedSymbols
#   include <shareLib.h>
#endif

namespace epics { namespace exampleCPP { namespace arrayPerformance {


class LongArrayGet;
typedef std::tr1::shared_ptr<LongArrayGet> LongArrayGetPtr;


class epicsShareClass  LongArrayGet :
    public epicsThreadRunable
{
public:
    LongArrayGet(
        std::string  providerName,
        std::string  channelName,
        int iterBetweenCreateChannel,
        int iterBetweenCreateChannelGet,
        double delayTime);
    virtual ~LongArrayGet(){}
    virtual void run();
private:
    size_t checkResult(
        const epics::pvData::PVStructurePtr &pvStructure,
        const epics::pvData::BitSetPtr & bitSet);
    std::string  providerName;
    std::string  channelName;
    int iterBetweenCreateChannel;
    int iterBetweenCreateChannelGet;
    double delayTime;
    std::string threadName;
    std::auto_ptr<epicsThread> thread;
};


}}}

#endif  /* LONGARRAYGET_H */
