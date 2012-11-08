/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS exampleCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <iostream>
#include <string>

#include <pv/pvData.h>
#include <pv/convert.h>

#include <pv/pvAccess.h>
#include <pv/clientFactory.h>
#include <pv/event.h>
#include <pv/logger.h>
#include <pv/rpcService.h>

#include "rpcClient.h"


using namespace epics::pvData;
using namespace epics::pvAccess;


namespace epics
{

namespace 
{

// copied from eget/pvutils. This is a lot of boilerplate and needs refactoring to common code.

class ChannelRequesterImpl :
    public epics::pvAccess::ChannelRequester
{
    private:
        epics::pvData::Event m_event;
    
    public:
        virtual epics::pvData::String getRequesterName();
        virtual void message(epics::pvData::String const & message, epics::pvData::MessageType messageType);
    
        virtual void channelCreated(const epics::pvData::Status& status, epics::pvAccess::Channel::shared_pointer const & channel);
        virtual void channelStateChange(epics::pvAccess::Channel::shared_pointer const & channel, epics::pvAccess::Channel::ConnectionState connectionState);
    
        bool waitUntilConnected(double timeOut);
};

class ChannelRPCRequesterImpl : public ChannelRPCRequester
{
    private:
    ChannelRPC::shared_pointer m_channelRPC;
    Mutex m_pointerMutex;
    Event m_event;
    Event m_connectionEvent;
    String m_channelName;

    public:
    epics::pvData::PVStructure::shared_pointer response;   
    ChannelRPCRequesterImpl(String channelName) : m_channelName(channelName) {}
    
    virtual String getRequesterName()
    {
        return "ChannelRPCRequesterImpl";
    }

    virtual void message(String const & message, MessageType messageType)
    {
        std::cerr << "[" << getRequesterName() << "] message(" << message << ", " << getMessageTypeName(messageType) << ")" << std::endl;
    }

    virtual void channelRPCConnect(const epics::pvData::Status& status,ChannelRPC::shared_pointer const & channelRPC)
    {
        if (status.isSuccess())
        {
            // show warning
            if (!status.isOK())
            {
                std::cerr << "[" << m_channelName << "] channel RPC create: " << status.toString() << std::endl;
            }
            
            // assign smart pointers
            {
                Lock lock(m_pointerMutex);
                m_channelRPC = channelRPC;
            }
            
            m_connectionEvent.signal();
        }
        else
        {
            std::cerr << "[" << m_channelName << "] failed to create channel get: " << status.toString() << std::endl;
        }
    }

    virtual void requestDone (const epics::pvData::Status &status, epics::pvData::PVStructure::shared_pointer const &pvResponse)
    {
        if (status.isSuccess())
        {
            // show warning
            if (!status.isOK())
            {
                std::cerr << "[" << m_channelName << "] channel RPC: " << status.toString() << std::endl;
            }

            // access smart pointers
            {
                Lock lock(m_pointerMutex);

                response = pvResponse;
                // this is OK since calle holds also owns it
                m_channelRPC.reset();                
            }
            
            m_event.signal();
            
        }
        else
        {
            std::cerr << "[" << m_channelName << "] failed to RPC: " << status.toString() << std::endl;
            {
                Lock lock(m_pointerMutex);
                // this is OK since caller holds also owns it
                m_channelRPC.reset();
            }
        }
        
    }
    
    /*
    void request(epics::pvData::PVStructure::shared_pointer const &pvRequest)
    {
        Lock lock(m_pointerMutex);
        m_channelRPC->request(pvRequest, false);
    }
    */

    bool waitUntilRPC(double timeOut)
    {
        return m_event.wait(timeOut);
    }

    bool waitUntilConnected(double timeOut)
    {
        return m_connectionEvent.wait(timeOut);
    }
};


String ChannelRequesterImpl::getRequesterName()
{
	return "ChannelRequesterImpl";
}

void ChannelRequesterImpl::message(String const & message, MessageType messageType)
{
	std::cerr << "[" << getRequesterName() << "] message(" << message << ", " << getMessageTypeName(messageType) << ")" << std::endl;
}

void ChannelRequesterImpl::channelCreated(const epics::pvData::Status& status, Channel::shared_pointer const & channel)
{
	if (status.isSuccess())
	{
		// show warning
		if (!status.isOK())
		{
			std::cerr << "[" << channel->getChannelName() << "] channel create: " << status.toString() << std::endl;
		}
	}
	else
	{
		std::cerr << "[" << channel->getChannelName() << "] failed to create a channel: " << status.toString() << std::endl;
	}
}

void ChannelRequesterImpl::channelStateChange(Channel::shared_pointer const & /*channel*/, Channel::ConnectionState connectionState)
{
	if (connectionState == Channel::CONNECTED)
	{
		m_event.signal();
	}
	/*
	else if (connectionState != Channel::DESTROYED)
	{
		std::cerr << "[" << channel->getChannelName() << "] channel state change: "  << Channel::ConnectionStateNames[connectionState] << std::endl;
	}
	*/
}
    
bool ChannelRequesterImpl::waitUntilConnected(double timeOut)
{
	return m_event.wait(timeOut);
}


}

namespace rpcClient
{

/*
  sendRequest is boilerplate for blocking RPC get
*/
PVStructure::shared_pointer sendRequest(const std::string & serviceName,
     PVStructure::shared_pointer request,
     double timeOut)
{
    using namespace std::tr1;

    PVStructure::shared_pointer response;

    bool allOK = true;

    ClientFactory::start();
    ChannelProvider::shared_pointer provider = getChannelAccess()->getProvider("pvAccess");
    
    shared_ptr<ChannelRequesterImpl> channelRequesterImpl(new ChannelRequesterImpl()); 
    Channel::shared_pointer channel = provider->createChannel(serviceName, channelRequesterImpl);

    if (channelRequesterImpl->waitUntilConnected(timeOut))
    {
        shared_ptr<ChannelRPCRequesterImpl> rpcRequesterImpl(new ChannelRPCRequesterImpl(channel->getChannelName()));
        ChannelRPC::shared_pointer channelRPC = channel->createChannelRPC(rpcRequesterImpl, request);

        if (rpcRequesterImpl->waitUntilConnected(timeOut))
        {
            channelRPC->request(request, true);
            allOK &= rpcRequesterImpl->waitUntilRPC(timeOut);
            response = rpcRequesterImpl->response;
        }
        else
        {
            allOK = false;
            channel->destroy();
            std::cerr << "[" << channel->getChannelName() << "] RPC create timeout" << std::endl;
        }
    }
    else
    {
        allOK = false;
        channel->destroy();
        std::cerr << "[" << channel->getChannelName() << "] connection timeout" << std::endl;
    }

    ClientFactory::stop();

    if (!allOK)
    {
        throw epics::pvAccess::RPCRequestException(Status::STATUSTYPE_ERROR, "RPC request failed");
    }

    return response;
}

}

}

