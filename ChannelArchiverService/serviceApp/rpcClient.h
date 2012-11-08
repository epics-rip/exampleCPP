/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS exampleCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#ifndef SERVICECLIENT_H
#define SERVICECLIENT_H

#include <string>

#include <pv/pvData.h>


namespace epics
{

namespace rpcClient
{
    /**
     * Performs complete blocking RPC call, opening a channel and connecting to the
     * service and sending the request.
     * The PVStructure sent on connection is null.
     *
     * @param  serviceName         the name of the service to connect to
     * @param  request             the request sent to the service
     * @return                     the result of the RPC call.
     */
    epics::pvData::PVStructure::shared_pointer sendRequest(const std::string & serviceName,
        epics::pvData::PVStructure::shared_pointer request, double timeOut);
}

}

#endif


