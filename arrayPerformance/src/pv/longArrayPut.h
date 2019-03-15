/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.08.09
 */
#ifndef LONGARRAYPUT_H
#define LONGARRAYPUT_H


#include <epicsThread.h>
#include <pv/pvaClient.h>

#include <shareLib.h>

namespace epics { namespace exampleCPP { namespace arrayPerformance {

typedef std::tr1::shared_ptr<epicsThread> EpicsThreadPtr;

class LongArrayPut;
typedef std::tr1::shared_ptr<LongArrayPut> LongArrayPutPtr;


class epicsShareClass  LongArrayPut :
    public epicsThreadRunable
{
public:
    LongArrayPut(
        std::string const & channelName,
        size_t arraySize = 100,
        int iterBetweenCreateChannel = 0,
        int iterBetweenCreateChannelPut = 0,
        double delayTime = 0.0);
    virtual void run();
    void stop();
private:
    std::string channelName;
    size_t arraySize;
    int iterBetweenCreateChannel;
    int iterBetweenCreateChannelPut;
    double delayTime;
    EpicsThreadPtr thread;
    epics::pvData::Event runStop;
    epics::pvData::Event runReturn;
};


}}}

#endif  /* LONGARRAYPUT_H */
