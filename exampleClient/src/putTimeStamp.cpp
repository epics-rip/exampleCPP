/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */
#include <iostream>
#include <epicsGetopt.h>
#include <pv/pvaClient.h>
#include <pv/convert.h>
#include <time.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

class ClientPut;
typedef std::tr1::shared_ptr<ClientPut> ClientPutPtr;

class ClientPut :
    public std::tr1::enable_shared_from_this<ClientPut>
{
private:
    string channelName;
    string providerName;
    string request;
    
    PvaClientChannelPtr pvaClientChannel;

    void init(PvaClientPtr const &pvaClient)
    {
        pvaClientChannel = pvaClient->createChannel(channelName,providerName);
        pvaClientChannel->connect();
    }
public:
    POINTER_DEFINITIONS(ClientPut);
    ClientPut(
        const string &channelName,
        const string &providerName,
        const string &request)
    : channelName(channelName),
      providerName(providerName),
      request(request)
    {
    }
    
    static ClientPutPtr create(
        PvaClientPtr const &pvaClient,
        const string & channelName,
        const string & providerName,
        const string  & request)
    {
        ClientPutPtr client(ClientPutPtr(
             new ClientPut(channelName,providerName,request)));
        client->init(pvaClient);
        return client;
    }

   

    void put(const TimeStamp & timeStamp)
    {
        PvaClientPutPtr pvaClientPut = pvaClientChannel->createPut(request);
        pvaClientPut->connect();
        PvaClientPutDataPtr putData = pvaClientPut->getData();
        PVStructurePtr pvStructure = putData->getPVStructure();
        PVFieldPtr pvSend(pvStructure->getSubField<PVField>("timeStamp"));
        if(!pvSend) {
            cout << pvStructure << "\n does not have a timeStamp field\n";
            return;
        } 
        PVTimeStamp pvTimeStamp;
        if(!pvTimeStamp.attach(pvSend)) {
           cout << pvStructure << "\n does not have a valid timeStamp field\n";
           return;
        }
        pvTimeStamp.set(timeStamp);
        pvaClientPut->put();
    }

    void get()
    {
        PvaClientGetPtr pvaClientGet = pvaClientChannel->get(request);
        PvaClientGetDataPtr getData = pvaClientGet->getData();
        cout << getData->getPVStructure() << endl;
    }
};


int main(int argc,char *argv[])
{    
    cout << "_____putTimeStamp starting__\n";
    string channelName("PVRdouble");
    string request("record[process=false]field(timeStamp)");
    if(argc==2) {channelName= argv[1];}
    cout << "channel=" << channelName << "\n";
    try {   
        PvaClientPtr pva= PvaClient::get("pva");
        ClientPutPtr clientPut = ClientPut::create(pva,channelName,"pva",request);
        TimeStamp timeStamp;
        timeStamp.put(1000,100);
        clientPut->get();
        clientPut->put(timeStamp);
        clientPut->get();
        timeStamp.put(0,0);
        clientPut->put(timeStamp);
        clientPut->get();
    } catch (std::exception& e) {
         cerr << "exception " << e.what() << endl;
         return 1;
    }
    return 0;
}
