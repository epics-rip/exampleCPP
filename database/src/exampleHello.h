/* exampleHello.h */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2013.04.02
 */
#ifndef EXAMPLEHELLO_H
#define EXAMPLEHELLO_H

#ifdef epicsExportSharedSymbols
#   define exampleHelloEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <pv/pvDatabase.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>

#ifdef exampleHelloEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef exampleHelloEpicsExportSharedSymbols
#endif

#include <shareLib.h>


namespace epics { namespace exampleHello { 


class ExampleHello;
typedef std::tr1::shared_ptr<ExampleHello> ExampleHelloPtr;

class epicsShareClass ExampleHello :
    public epics::pvDatabase::PVRecord
{
public:
    POINTER_DEFINITIONS(ExampleHello);
    static ExampleHelloPtr create(
        std::string const & recordName);
    virtual ~ExampleHello();
    virtual void destroy();
    virtual bool init();
    virtual void process();
private:
    ExampleHello(std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure);

    epics::pvData::PVStringPtr pvArgumentValue;
    epics::pvData::PVStringPtr pvResultValue;
    epics::pvData::PVTimeStamp pvTimeStamp;
    epics::pvData::TimeStamp timeStamp;
};


}}

#endif  /* EXAMPLEHELLO_H */
