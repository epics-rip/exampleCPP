#ifndef ARCHIVER_RPCSERVICE_H
#define ARCHIVER_RPCSERVICE_H

#include <string>
#include <cstring>
#include <stdexcept>
#include <memory>

#include <pv/service.h>

class ArchiverServiceRPC;

class ArchiverServiceRPC : public epics::pvIOC::ServiceRPC,
  public std::tr1::enable_shared_from_this<ArchiverServiceRPC>
{
public:
    POINTER_DEFINITIONS(ArchiverServiceRPC);
    ArchiverServiceRPC();
    virtual ~ArchiverServiceRPC();
    virtual void destroy();
    virtual void request(
        epics::pvAccess::ChannelRPCRequester::shared_pointer const & channelRPCRequester,
        epics::pvData::PVStructure::shared_pointer const & pvArgument);
private:
    ArchiverServiceRPC::shared_pointer getPtrSelf()
    {
        return shared_from_this();
    }
};

#endif
