// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/* arrayPerformance.h */

/**
 * @author mrk
 * @date 2013.08.08
 */
#ifndef ARRAYPERFORMANCE_H
#define ARRAYPERFORMANCE_H


#ifdef epicsExportSharedSymbols
#   define arrayperformanceEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <epicsThread.h>
#include <pv/standardPVField.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>
#include <pv/pvDatabase.h>

#ifdef arrayperformanceEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef arrayperformanceEpicsExportSharedSymbols
#   include <shareLib.h>
#endif

namespace epics { namespace exampleCPP { namespace arrayPerformance { 

class ArrayPerformance;
typedef std::tr1::shared_ptr<ArrayPerformance> ArrayPerformancePtr;

class epicsShareClass  ArrayPerformance :
    public epics::pvDatabase::PVRecord,
    public epicsThreadRunable
{
public:
    static ArrayPerformancePtr create(
        std::string const & recordName,
        size_t size,
        double delay);
    virtual ~ArrayPerformance();
    virtual bool init();
    virtual void process();
    virtual void destroy();
    virtual void run();
private:
     ArrayPerformance(std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure,
        size_t size,
        double delay);
    size_t size;
    double delay;
    bool isDestroyed;
    epics::pvData::PVLongArrayPtr pvValue;
    epics::pvData::PVTimeStamp pvTimeStamp;
    epics::pvData::TimeStamp timeStamp;
    std::string threadName;
    std::auto_ptr<epicsThread> thread;
};

}}}

#endif  /* ARRAYPERFORMANCE_H */
