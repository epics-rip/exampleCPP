#include <iostream>
#include <pv/clientFactory.h>

#include <stdio.h>
#include <epicsStdlib.h>
#include <epicsGetopt.h>
#include <pv/logger.h>

#include <vector>
#include <string>

#include <pv/convert.h>
#include <pv/event.h>

#include <epicsTime.h>

using namespace std;
using namespace std::tr1;
using namespace epics::pvData;
using namespace epics::pvAccess;

#include "types.hpp"

class ChannelRPCRequesterImpl : public ChannelRPCRequester
{
    auto_ptr<Event> m_event;
    public:
    shared_ptr<epics::pvData::PVStructure> pvResponse;
    ChannelRPCRequesterImpl(String channelName) 
    {
        resetEvent();
    }

    virtual void channelRPCConnect (const epics::pvData::Status &status, ChannelRPC::shared_pointer const &channelRPC)
    {
        m_event->signal();
    }

    virtual void requestDone (const epics::pvData::Status &status, epics::pvData::PVStructure::shared_pointer const &pvResponse)
    {
        this->pvResponse = pvResponse;
        m_event->signal();
    }

    virtual String getRequesterName()
    {
        return "ChannelRPCRequesterImpl";
    };

    virtual void message(String message,MessageType messageType)
    {
        std::cout << "[" << getRequesterName() << "] message(" << message << ", " << messageTypeName[messageType] << ")" << std::endl;
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

    virtual void message(String message,MessageType messageType)
    {
        std::cout << "[" << getRequesterName() << "] message(" << message << ", " << messageTypeName[messageType] << ")" << std::endl;
    }

    virtual void channelCreated(const epics::pvData::Status& status, Channel::shared_pointer const & channel)
    {
        if (status.isSuccess())
        {
            // show warning
            if (!status.isOK())
            {
                std::cout << "[" << channel->getChannelName() << "] channel create: " << status.toString() << std::endl;
            }
        }
        else
        {
            std::cout << "[" << channel->getChannelName() << "] failed to create a channel: " << status.toString() << std::endl;
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

/*
  PrintRPCget is boilerplate for blocking RPC get
*/

void PrintRPCget(string pvName, PVStructure::shared_pointer pvRequest)
{

    double timeOut = 3.0;

    SET_LOG_LEVEL(logLevelDebug);

    ClientFactory::start();
    ChannelProvider::shared_pointer provider = getChannelAccess()->getProvider("pvAccess");
    
    bool allOK = true;
    
    try
    {
        do
        {
            // first connect
            shared_ptr<ChannelRequesterImpl> channelRequesterImpl(new ChannelRequesterImpl()); 
            Channel::shared_pointer channel = provider->createChannel(pvName, channelRequesterImpl);
            
            if (channelRequesterImpl->waitUntilConnected(timeOut))
            {
                shared_ptr<ChannelRPCRequesterImpl> rpcRequesterImpl(new ChannelRPCRequesterImpl(channel->getChannelName()));
                
                // A PVStructure is sent at ChannelRPC connect time but we don't use it, so send an empty one
                PVStructure::shared_pointer nothing(
                    new PVStructure(NULL, getFieldCreate()->createStructure("nothing", 0, NULL)));

                ChannelRPC::shared_pointer channelRPC = channel->createChannelRPC(rpcRequesterImpl, nothing);
                allOK &= rpcRequesterImpl->waitUntilDone(timeOut);
                std::cout << "connected" << std::endl;
                if (allOK)
                {
                    rpcRequesterImpl->resetEvent();
                    channelRPC->request(pvRequest, false);
                    allOK &= rpcRequesterImpl->waitUntilDone(timeOut);
                    if (allOK)
                    {
                        
                        if(rpcRequesterImpl->pvResponse == NULL)
                        {
                            std::cout << "Error: null response" << std::endl;
                        }
                        else
                        {
                            std::cout << toString(rpcRequesterImpl->pvResponse) << std::endl;
                        }
                    }
                    else
                    {
                        std::cout << "Error" << std::endl;
                    }
                }
            }
            else
            {
                allOK = false;
                channel->destroy();
                std::cout << "[" << channel->getChannelName() << "] connection timeout" << std::endl;
                break;
            }
        }
        while (false);
    } catch (std::out_of_range& oor) {
        allOK = false;
        std::cout << "parse error: not enough of values" << std::endl;
    } catch (std::exception& ex) {
        allOK = false;
        std::cout << ex.what() << std::endl;
    } catch (...) {
        allOK = false;
        std::cout << "unknown exception caught" << std::endl;
    }
        
    ClientFactory::stop();
    
}

int main (int argc, char *argv[])
{
    
    char * index = "/home/jr76/epics4/cpp/exampleCPP/ChannelArchiverService/data/index";
    
    // create request type and instance
    StructureConstPtr archiverStructure = MYArchiverQuery("MYArchiverQuery", *getFieldCreate());
    PVStructure::shared_pointer pvRequest(getPVDataCreate()->createPVStructure(NULL, archiverStructure));

    // set request
    pvRequest->getStringField("name")->put("fred");
    pvRequest->getStringField("index")->put(index);
    pvRequest->getLongField("count")->put(20);
    pvRequest->getLongField("t0secPastEpoch")->put(496169402);
    
    string pvName = "serviceRPC";

    std::cout << toString(pvRequest) << std::endl;

    PrintRPCget(pvName, pvRequest);
    
    return 0;

}

