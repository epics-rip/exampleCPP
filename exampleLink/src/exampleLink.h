// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/* exampleLink.h */

/**
 * @author mrk
 * @date 2013.08.02
 */
#ifndef EXAMPLELINK_H
#define EXAMPLELINK_H

#ifdef epicsExportSharedSymbols
#   define exampleLinkEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>
#include <pv/alarm.h>
#include <pv/pvAlarm.h>
#include <pv/pvDatabase.h>
#include <pv/pvCopy.h>
#include <pv/pvAccess.h>
#include <pv/serverContext.h>

#ifdef exampleLinkEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef exampleLinkEpicsExportSharedSymbols
#endif

#include <shareLib.h>

namespace epics { namespace pvDatabase { 


class ExampleLink;
typedef std::tr1::shared_ptr<ExampleLink> ExampleLinkPtr;

class epicsShareClass ExampleLink :
    public PVRecord,
    public epics::pvAccess::ChannelRequester,
    public epics::pvData::MonitorRequester
{
public:
    POINTER_DEFINITIONS(ExampleLink);
    static ExampleLinkPtr create(
        std::string const & recordName,
        std::string const & providerName,
        std::string const & channelName
        );
    virtual ~ExampleLink() {}
    virtual void destroy();
    virtual bool init();
    virtual void process();
    virtual void channelCreated(
        const epics::pvData::Status& status,
        epics::pvAccess::Channel::shared_pointer const & channel);
    virtual void channelStateChange(
        epics::pvAccess::Channel::shared_pointer const & channel,
        epics::pvAccess::Channel::ConnectionState connectionState);
    virtual std::string getRequesterName() {return channelName;}
    virtual void message(
        std::string const & message,
        epics::pvData::MessageType messageType)
        {
           std::cout << "ExampleLink::message " << message << "\n";
        }
    virtual void monitorConnect(
        const epics::pvData::Status& status,
        epics::pvData::Monitor::shared_pointer const & monitor,
        epics::pvData::StructureConstPtr const & structure);
    virtual void monitorEvent(epics::pvData::MonitorPtr const & monitor);
    virtual void unlisten(epics::pvData::MonitorPtr const & monitor);
private:
    ExampleLink(std::string const & recordName,
        std::string providerName,
        std::string channelName,
        epics::pvData::PVStructurePtr const & pvStructure);
    std::string providerName;
    std::string channelName;
    epics::pvData::PVDoubleArrayPtr pvValue;
    epics::pvAccess::Channel::shared_pointer channel;
    epics::pvData::Event event;
    epics::pvData::Status status;
    epics::pvData::MonitorPtr monitor;
};

}}

#endif  /* EXAMPLELINK_H */
