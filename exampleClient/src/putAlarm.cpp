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
    bool channelConnected;
    bool putConnected;

    PvaClientChannelPtr pvaClientChannel;
    PvaClientPutPtr pvaClientPut;

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
      request(request),
      channelConnected(false),
      putConnected(false)
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

    void put(const Alarm & alarm)
    {
        PvaClientPutPtr pvaClientPut = pvaClientChannel->createPut(request);
        pvaClientPut->connect();
        PvaClientPutDataPtr putData = pvaClientPut->getData();
        PVStructurePtr pvStructure = putData->getPVStructure();
        PVFieldPtr pvSend(pvStructure->getSubField<PVField>("alarm"));
        if(!pvSend) {
            cout << pvStructure << "\n does not have an alarm field\n";
            return;
        }    
        PVAlarm pvAlarm;
        if(!pvAlarm.attach(pvSend)) {
           cout << pvStructure << "\n does not have a valid alarm field\n";
           return;
        }
        pvAlarm.set(alarm);
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
    cout << "_____putAlarm starting__\n";
    string channelName("PVRdouble");
    if(argc==2) {channelName= argv[1];}
    cout << "channel=" << channelName << "\n";
    try {   
        PvaClientPtr pva= PvaClient::get("pva");
        ClientPutPtr clientPut = ClientPut::create(pva,channelName,"pva","alarm");
        Alarm alarm;
        alarm.setMessage(string("testMessage"));
        alarm.setSeverity(majorAlarm);
        alarm.setStatus(clientStatus);
        clientPut->get();
        clientPut->put(alarm);
        clientPut->get();
        alarm.setMessage(string(""));
        alarm.setSeverity(noAlarm);
        alarm.setStatus(noStatus);
        clientPut->put(alarm);
        clientPut->get();
    } catch (std::exception& e) {
         cerr << "exception " << e.what() << endl;
         return 1;
    }
    return 0;
}
