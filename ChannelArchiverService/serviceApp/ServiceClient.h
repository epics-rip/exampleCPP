/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS exampleCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <string>

#include <pv/pvData.h>

namespace epics
{

namespace serviceClient
{

    /**
     * Performs complete blocking RPC call, opening channel and connecting to service,
     * sending the request.
     *
     * If the command fails the response will be a smart pointer to null.
     * The caller should check for this.
     * 
     *
     * @param  serviceName         The name of the servie to connect to.
     * @param  connectionStructure the PVStructure sent when connecting to service.
     * @param  request             The request sent to service.
     * @return the result of the RPC call.
     */
    epics::pvData::PVStructure::shared_pointer SendRequest(std::string serviceName,
        epics::pvData::PVStructure::shared_pointer connectionStructure,
        epics::pvData::PVStructure::shared_pointer request, double timeOut);

    /**
     * Performs complete blocking RPC call, opening channel and connecting to service,
     * sending the request.
     *
     * If the command fails the response will be a smart pointer to null.
     * The caller should check for this.
     * The PVStructure sent on connection is null.
     *
     * @param  serviceName         The name of the servie to connect to.
     * @param  request             The request sent to service.
     * @return the result of the RPC call.
     */
    epics::pvData::PVStructure::shared_pointer SendRequest(std::string serviceName,
        epics::pvData::PVStructure::shared_pointer request, double timeOut);

}

}
