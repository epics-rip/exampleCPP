/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS exampleCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */


#ifndef ARCHIVERSERVICERPC_H
#define ARCHIVERSERVICERPC_H

#include <string>
#include <cstring>
#include <stdexcept>
#include <memory>

#include <pv/service.h>

namespace channelArchiverService
{

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
                  const epicsTimeStamp & t0,
                  const epicsTimeStamp & t1,
                  int64_t count);
    ArchiverServiceRPC::shared_pointer getPtrSelf()
    {
        return shared_from_this();
    }
};

}

#endif
