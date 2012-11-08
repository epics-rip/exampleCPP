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
     * RPCClient is an interface class that is called by a service client.
     *
     */
    class RPCClient
    {
    public:
        POINTER_DEFINITIONS(RPCClient);

        virtual epics::pvData::PVStructure::shared_pointer request(epics::pvData::PVStructure::shared_pointer pvRequest,
            double timeOut) = 0;

        virtual ~RPCClient() {}
    };



    class RPCClientFactory
    {
    public:
	    /**
	     * Create a RPCClient and connect to the service.
         *
	     * @param  serviceName  the service name 
	     * @return              the RPCClient interface
	     */
	    static RPCClient::shared_pointer create(const std::string & serviceName);
    };

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


