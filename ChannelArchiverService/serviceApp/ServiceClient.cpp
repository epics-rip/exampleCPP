/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS exampleCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <iostream>
#include <string>

#include <pv/pvData.h>

#include <pv/pvAccess.h>
#include <pv/clientFactory.h>
#include <pv/event.h>
#include <pv/logger.h>



using namespace std;
using namespace std::tr1;
using namespace epics::pvData;
using namespace epics::pvAccess;

namespace epics
{

namespace 
{

class ChannelRPCRequesterImpl : public ChannelRPCRequester
{
    auto_ptr<Event> m_event;
    public:
    shared_ptr<epics::pvData::PVStructure> pvResponse;
    ChannelRPCRequesterImpl(String channelName) 
    {
        resetEvent();
    }

    virtual void channelRPCConnect(const epics::pvData::Status &status, ChannelRPC::shared_pointer const &channelRPC)
    {
        m_event->signal();
    }

    virtual void requestDone(const epics::pvData::Status &status, epics::pvData::PVStructure::shared_pointer const &pvResponse)
    {
        this->pvResponse = pvResponse;
        m_event->signal();
    }

    virtual String getRequesterName()
    {
        return "ChannelRPCRequesterImpl";
    };

    virtual void message(String const &  message, MessageType messageType)
    {
        std::cerr << "[" << getRequesterName() << "] message(" << message << ", " << getMessageTypeName(messageType) << ")" << std::endl;
    }

    void resetEvent()
    {
        m_event.reset(new Event());
    }

    bool waitUntilDone(double timeOut)
    {
        return m_event->wait(timeOut);
    }
};

class ChannelRequesterImpl : public ChannelRequester
{
private:
    Event m_event;    
    
public:
    
    virtual String getRequesterName()
    {
        return "ChannelRequesterImpl";
    };

    virtual void message(String const & message, MessageType messageType)
    {
        std::cerr << "[" << getRequesterName() << "] message(" << message << ", " << getMessageTypeName(messageType) << ")" << std::endl;
    }

    virtual void channelCreated(const epics::pvData::Status& status, Channel::shared_pointer const & channel)
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

    virtual void channelStateChange(Channel::shared_pointer const & channel, Channel::ConnectionState connectionState)
    {
        if (connectionState == Channel::CONNECTED)
        {
            m_event.signal();
        }
    }
    
    bool waitUntilConnected(double timeOut)
    {
        return m_event.wait(timeOut);
    }
};

}

namespace serviceClient
{

/*
  SendRequest is boilerplate for blocking RPC get
*/
PVStructure::shared_pointer SendRequest(string serviceName, PVStructure::shared_pointer connectionStructure,
                                        PVStructure::shared_pointer request, double timeOut)
{
    PVStructure::shared_pointer response;

    ClientFactory::start();
    ChannelProvider::shared_pointer provider = getChannelAccess()->getProvider("pvAccess");
    
    bool allOK = true;
    
    try
    {
        do
        {
            // first connect
            shared_ptr<ChannelRequesterImpl> channelRequesterImpl(new ChannelRequesterImpl()); 
            Channel::shared_pointer channel = provider->createChannel(serviceName, channelRequesterImpl);
            
            if (channelRequesterImpl->waitUntilConnected(timeOut))
            {
                shared_ptr<ChannelRPCRequesterImpl> rpcRequesterImpl(new ChannelRPCRequesterImpl(channel->getChannelName()));

                ChannelRPC::shared_pointer channelRPC = channel->createChannelRPC(rpcRequesterImpl, connectionStructure);
                allOK &= rpcRequesterImpl->waitUntilDone(timeOut);
                //std::cout << "connected" << std::endl;
                if (allOK)
                {
                    rpcRequesterImpl->resetEvent();
                    channelRPC->request(request, false);
                    allOK &= rpcRequesterImpl->waitUntilDone(timeOut);
                    if (allOK)
                    {
                        response = rpcRequesterImpl->pvResponse;
                    }
                    else
                    {
                        std::cerr << "Error" << std::endl;
                    }
                }
            }
            else
            {
                allOK = false;
                channel->destroy();
                std::cerr << "[" << channel->getChannelName() << "] connection timeout" << std::endl;
                break;
            }
        }
        while (false);
    } catch (std::out_of_range& oor) {
        allOK = false;
        std::cerr << "parse error: not enough values" << std::endl;
    } catch (std::exception& ex) {
        allOK = false;
        std::cerr << ex.what() << std::endl;
    } catch (...) {
        allOK = false;
        std::cerr << "unknown exception caught" << std::endl;
    }
        
    ClientFactory::stop();
    return response;   
}


/*
  SendRequest is boilerplate for blocking RPC get
*/
PVStructure::shared_pointer SendRequest(string serviceName, PVStructure::shared_pointer request, double timeOut)
{
    // A PVStructure is sent at ChannelRPC connect time but we aren't going to use it, so send an empty one
    StringArray fieldNames;
    FieldConstPtrArray fields;
    PVStructure::shared_pointer nothing(
        new PVStructure(getFieldCreate()->createStructure(fieldNames, fields)));

    return SendRequest(serviceName, nothing, request, timeOut);
}

}

}

