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

#include <pv/rpcServer.h>

namespace channelArchiverService
{

class ArchiverServiceRPC : public epics::pvAccess::RPCService
{
    std::string indexFilename;
public:
    POINTER_DEFINITIONS(ArchiverServiceRPC);
    ArchiverServiceRPC(char * indexFilename) : indexFilename(indexFilename) {}
    virtual ~ArchiverServiceRPC();

    epics::pvData::PVStructure::shared_pointer request(
        epics::pvData::PVStructure::shared_pointer const & args
    ) throw (epics::pvAccess::RPCRequestException);

private:
    epics::pvData::PVStructure::shared_pointer QueryRaw(
        epics::pvData::PVStructure::shared_pointer const & pvArgument,
        std::string & name, 
        const epicsTimeStamp & t0,
        const epicsTimeStamp & t1,
        int64_t count);
};

}

#endif
