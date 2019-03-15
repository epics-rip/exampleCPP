/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.08.09
 */
#ifndef LONGARRAYGET_H
#define LONGARRAYGET_H


#include <epicsThread.h>
#include <pv/pvaClient.h>

#include <shareLib.h>

namespace epics { namespace exampleCPP { namespace arrayPerformance {

typedef std::tr1::shared_ptr<epicsThread> EpicsThreadPtr;

class LongArrayGet;
typedef std::tr1::shared_ptr<LongArrayGet> LongArrayGetPtr;


class epicsShareClass  LongArrayGet :
    public epicsThreadRunable
{
public:
    LongArrayGet(
        std::string  channelName,
        int iterBetweenCreateChannel,
        int iterBetweenCreateChannelGet,
        double delayTime);
    virtual ~LongArrayGet(){}
    virtual void run();
    void stop();
private:
    std::string  channelName;
    int iterBetweenCreateChannel;
    int iterBetweenCreateChannelGet;
    double delayTime;
    EpicsThreadPtr thread;
    epics::pvData::Event runStop;
    epics::pvData::Event runReturn;
};


}}}

#endif  /* LONGARRAYGET_H */
