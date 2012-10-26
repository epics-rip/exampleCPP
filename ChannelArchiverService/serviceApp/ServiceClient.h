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

namespace serviceClient
{
    /**
     * Interface class for processing responses from a server to a client RPC request.
     */
    class ResponseHandler
    {
    public:
        POINTER_DEFINITIONS(ResponseHandler);
        /**
         * Process the response from the server to a clients request
         *
         * @param  pvResponse      The response received from the server to the clients request.
         */
        virtual void handle(epics::pvData::PVStructure::shared_pointer const &pvResponse) = 0;
        virtual ~ResponseHandler() {};
    };

    /**
     * Performs complete blocking RPC call, opening channel and connecting to service,
     * sending the request and processing the response.
     *
     * @param  serviceName         The name of the servie to connect to.
     * @param  connectionStructure the PVStructure sent when connecting to service.
     * @param  request             The request sent to service.
     * @param  handler             The handler to be used to process the response.
     * @return true iff the call was successful
     */
    bool SendRequest(
        std::string serviceName,
        epics::pvData::PVStructure::shared_pointer connectionStructure,
        epics::pvData::PVStructure::shared_pointer request,
        ResponseHandler::shared_pointer  handler,
        double timeOut);

    /**
     * Performs complete blocking RPC call, opening channel and connecting to service,
     * sending the request and processing the response.
     *
     * The PVStructure sent on connection is null.
     *
     * @param  serviceName         The name of the servie to connect to.
     * @param  request             The request sent to service.
     * @param  handler             The handler to be used to process the response.
     * @return true iff the call was successful
     */
    bool SendRequest(
        std::string serviceName,
        epics::pvData::PVStructure::shared_pointer request,
        ResponseHandler::shared_pointer  handler,
        double timeOut);

}

}

#endif


