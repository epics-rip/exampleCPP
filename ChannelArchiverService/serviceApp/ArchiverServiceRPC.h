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
    std::string indexFilename;
public:
    POINTER_DEFINITIONS(ArchiverServiceRPC);
    ArchiverServiceRPC(char * indexFilename) : indexFilename(indexFilename) {}
    virtual ~ArchiverServiceRPC();
    virtual void destroy();
    virtual void request(
        epics::pvAccess::ChannelRPCRequester::shared_pointer const & channelRPCRequester,
        epics::pvData::PVStructure::shared_pointer const & pvArgument);
private:
    void QueryRaw(epics::pvAccess::ChannelRPCRequester::shared_pointer const & channelRPCRequester,
                  epics::pvData::PVStructure::shared_pointer const & pvArgument,
                  std::string & name, 
                  const epicsTimeStamp & t0, int64_t count);
    ArchiverServiceRPC::shared_pointer getPtrSelf()
    {
        return shared_from_this();
    }
};

#endif
