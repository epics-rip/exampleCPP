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
     * ResponseHandler is an interface class for processing responses from a server to
     * a client RPC request.
     */
    class ResponseHandler
    {
    public:
        POINTER_DEFINITIONS(ResponseHandler);
        /**
         * Processes the response from the server to a client request
         *
         * @param  pvResponse      the response received from the server to the client request
         */
        virtual void handle(epics::pvData::PVStructure::shared_pointer const & pvResponse) = 0;
        virtual ~ResponseHandler() {};
    };

    /**
     * Performs complete blocking RPC call, opening a channel and connecting to service,
     * sending the request and processing the response.
     *
     * @param  serviceName         the name of the service to connect to
     * @param  connectionStructure the PVStructure sent when connecting to service
     * @param  request             the request sent to the service
     * @param  handler             the handler to be used to process the response
     * @return                     true if the call was successful, false otherwise
     */
    bool sendRequest(
        const std::string & serviceName,
        epics::pvData::PVStructure::shared_pointer connectionStructure,
        epics::pvData::PVStructure::shared_pointer request,
        ResponseHandler::shared_pointer handler,
        double timeOut);

    /**
     * Performs complete blocking RPC call, opening a channel and connecting to the
     * service, sending the request and processing the response.
     *
     * The PVStructure sent on connection is null.
     *
     * @param  serviceName         the name of the service to connect to
     * @param  request             the request sent to the service
     * @param  handler             the handler to be used to process the response
     * @return                     true if the call was successful, false otherwise
     */
    bool sendRequest(
        const std::string & serviceName,
        epics::pvData::PVStructure::shared_pointer request,
        ResponseHandler::shared_pointer handler,
        double timeOut);

}

}

#endif


