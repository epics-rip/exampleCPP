/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <iostream>

#include <pv/pvaClient.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

class ClientPutGet;
typedef std::tr1::shared_ptr<ClientPutGet> ClientPutGetPtr;

class ClientPutGet :
    public PvaClientChannelStateChangeRequester,
    public PvaClientPutGetRequester,
    public std::tr1::enable_shared_from_this<ClientPutGet>
{
private:
    string channelName;
    string providerName;
    string request;
    bool channelConnected;
    bool putGetConnected;
    bool gotFirstGetPut;

    PvaClientChannelPtr pvaClientChannel;
    PvaClientPutGetPtr pvaClientPutGet;

    void init(PvaClientPtr const &pvaClient)
    {
        pvaClientChannel = pvaClient->createChannel(channelName,providerName);
        pvaClientChannel->setStateChangeRequester(shared_from_this());
        pvaClientChannel->issueConnect();
    }
public:
    POINTER_DEFINITIONS(ClientPutGet);
    ClientPutGet(
        const string &channelName,
        const string &providerName,
        const string &request)
    : channelName(channelName),
      providerName(providerName),
      request(request),
      channelConnected(false),
      putGetConnected(false),
      gotFirstGetPut(false)
    {
    }
    
    static ClientPutGetPtr create(
        PvaClientPtr const &pvaClient,
        const string & channelName,
        const string & providerName,
        const string  & request)
    {
        ClientPutGetPtr client(ClientPutGetPtr(
             new ClientPutGet(channelName,providerName,request)));
        client->init(pvaClient);
        return client;
    }

    virtual void channelStateChange(PvaClientChannelPtr const & channel, bool isConnected)
    {
        cout << "channeStateChange " << (isConnected ? "true" : "false") << endl;
        channelConnected = isConnected;
        if(isConnected) {
            if(!pvaClientPutGet) {
                pvaClientPutGet = pvaClientChannel->createPutGet(request);
                pvaClientPutGet->setRequester(shared_from_this());
                pvaClientPutGet->issueConnect();
            }
        }
    }

    virtual void channelPutGetConnect(
        const epics::pvData::Status& status,
        PvaClientPutGetPtr const & clientPutGet)
    {
        putGetConnected = true;
        cout << "channelPutGetConnect " << channelName << " status " << status << endl;
    }

    virtual void putGetDone(
        const epics::pvData::Status& status,
        PvaClientPutGetPtr const & clientPutGet)
    {
        cout << "putGetDone " << channelName << " status " << status << endl;
    }

     virtual void getPutDone(
        const epics::pvData::Status& status,
        PvaClientPutGetPtr const & clientPutGet)
    {
         cout << "getPutDone " << channelName << " status " << status << endl;
    }

    virtual void getGetDone(
        const epics::pvData::Status& status,
        PvaClientPutGetPtr const & clientPutGet)
    {
        cout << "getGetDone " << channelName << " status " << status << endl;
    }


    void putGet(const string & value)
    {
        if(!channelConnected) {
            cout << channelName << " channel not connected\n";
            return;
        }
        if(!putGetConnected) {
            cout << channelName << " channelPutGet not connected\n";
            return;
        }
        if(!gotFirstGetPut) {
             pvaClientPutGet->getPut();
        }
        PvaClientPutDataPtr putData = pvaClientPutGet->getPutData();
        PVStructurePtr arg = putData->getPVStructure();
        PVStringPtr pvValue = arg->getSubField<PVString>("argument.value");
        pvValue->put(value);
        pvaClientPutGet->putGet();
        PvaClientGetDataPtr getData = pvaClientPutGet->getGetData();
        cout << getData->getPVStructure() << endl;
    }

    
    void getPut()
    {
        if(!channelConnected) {
            cout << channelName << " channel not connected\n";
            return;
        }
        if(!putGetConnected) {
            cout << channelName << " channelPutGet not connected\n";
            return;
        }
        pvaClientPutGet->getPut();
        PvaClientPutDataPtr putData = pvaClientPutGet->getPutData();
        cout << putData->getPVStructure() << endl;
    }

    void getGet()
    {
        if(!channelConnected) {
            cout << channelName << " channel not connected\n";
            return;
        }
        if(!putGetConnected) {
            cout << channelName << " channelPutGet not connected\n";
            return;
        }
        pvaClientPutGet->getGet();
        PvaClientGetDataPtr getData = pvaClientPutGet->getGetData();
        cout << getData->getPVStructure() << endl;
    }

};


int main(int argc,char *argv[])
{
    string providerName("pva");
    string channelName("helloPutGet");
    string request("putField(argument)getField(result)");
    bool debug(false);
    if(argc==2 && string(argv[1])==string("-help")) {
        cout << "providerName channelName request debug" << endl;
        cout << "default" << endl;
        cout << providerName << " " <<  channelName
             << " " << '"' << request << '"'
             << " " << (debug ? "true" : "false") << endl;
        return 0;
    }
    if(argc>1) providerName = argv[1];
    if(argc>2) channelName = argv[2];
    if(argc>3) request = argv[3];
    if(argc>4) {
        string value(argv[4]);
        if(value=="true") debug = true;
    }
    bool pvaSrv(((providerName.find("pva")==string::npos) ? false : true));
    bool caSrv(((providerName.find("ca")==string::npos) ? false : true));
    if(pvaSrv&&caSrv) {
        cerr<< "multiple providerNames are not allowed\n";
        return 1;
    }
    cout << "providerName " << providerName
         << " pvaSrv " << (pvaSrv ? "true" : "false")
         << " caSrv " << (caSrv ? "true" : "false")
         << " channelName " <<  channelName
         << " request " << request
         << " debug " << (debug ? "true" : "false") << endl;

    cout << "_____helloNoWaitPutGet starting__\n";
    
    try {   
        if(debug) PvaClient::setDebug(true);
        PvaClientPtr pva= PvaClient::get(providerName);
        ClientPutGetPtr clientPutGet(ClientPutGet::create(pva,channelName,providerName,request));
        while(true) {
            cout << "enter command\n";
            string str;
            getline(cin,str);
            if(str.compare("help")==0){
                 cout << "exit putGet getPut getGet\n";
                 getline(cin,str);
            }
            if(str.compare("putGet")==0){
                 cout << "value?\n";
                 getline(cin,str);
                 clientPutGet->putGet(str);
                 continue;
            }
            if(str.compare("getPut")==0){
                 clientPutGet->getPut();
                 continue;
            }
            if(str.compare("getGet")==0){
                 clientPutGet->getGet();
                 continue;
            }
            if(str.compare("exit")==0) break;
            cout << "illegal inout\n";
            continue;
        }
    } catch (std::runtime_error e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}


