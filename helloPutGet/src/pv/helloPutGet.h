// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/* helloPutGet.h */
/**
 * @author mrk
 * @date 2013.04.02
 */
#ifndef HELLOPUTGET_H
#define HELLOPUTGET_H

#ifdef epicsExportSharedSymbols
#   define helloPutGetEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <pv/pvDatabase.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>

#ifdef helloPutGetEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef helloPutGetEpicsExportSharedSymbols
#endif

#include <shareLib.h>


namespace epics { namespace exampleCPP { namespace helloPutGet { 


class HelloPutGet;
typedef std::tr1::shared_ptr<HelloPutGet> HelloPutGetPtr;

class epicsShareClass HelloPutGet :
    public epics::pvDatabase::PVRecord
{
public:
    POINTER_DEFINITIONS(HelloPutGet);
    static HelloPutGetPtr create(
        std::string const & recordName);
    virtual ~HelloPutGet();
    virtual void destroy();
    virtual bool init();
    virtual void process();
private:
    HelloPutGet(std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure);

    epics::pvData::PVStringPtr pvArgumentValue;
    epics::pvData::PVStringPtr pvResultValue;
    epics::pvData::PVTimeStamp pvTimeStamp;
    epics::pvData::TimeStamp timeStamp;
};


}}}

#endif  /* HELLOPUTGET_H */
