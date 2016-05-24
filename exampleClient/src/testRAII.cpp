// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/*testRAII.cpp */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */
#include <iostream>
#include <epicsThread.h>

#include <pv/pvaClient.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

static void getQuick(PvaClientPtr const &pva,string const & channelName,string const & providerName )
{
    cout << "_____getQuick starting_______\n";
    double value =  pva->channel(channelName,providerName)->get()->getData()->getDouble();
    cout << " get value " << value << endl;
    cout << "__getQuick__ returning\n";
}

static void getLongWay(PvaClientPtr const &pva,string const & channelName,string const & providerName )
{
    cout << "_____getLongWay starting_______\n";
    PvaClientChannelPtr pvaChannel = pva->createChannel(channelName,providerName);
    pvaChannel->issueConnect();
    Status status = pvaChannel->waitConnect(2.0);
    if(!status.isOK()) {cout << " connect failed\n"; return;}
    PvaClientGetPtr pvaGet = pvaChannel->createGet();
    pvaGet->issueConnect();
    status = pvaGet->waitConnect();
    if(!status.isOK()) {cout << " createGet failed\n"; return;}
    PvaClientGetDataPtr pvaData = pvaGet->getData();
    double value = pvaData->getDouble();
    cout << "get value " << value << endl;
    cout << "__getLongWay__ returning\n";
}



static void put(PvaClientPtr const &pva,string const & channelName,string const & providerName )
{
    cout << "_____put starting_______\n";
    double value =  pva->channel(channelName,providerName)->get()->getData()->getDouble();
    value += 1.0;
    PvaClientChannelPtr channel = pva->channel(channelName,providerName);
    PvaClientPutPtr put = channel->put();
    PvaClientPutDataPtr putData = put->getData();
    putData->putDouble(value); put->put();
    cout << "__put__ returning\n";
}

static void monitor(PvaClientPtr const &pva,string const & recordName,string const& provider)
{
    cout << "__monitor recordName " << recordName << " provider " << provider << "__\n";
    PvaClientMonitorPtr monitor = pva->channel(recordName,provider)->monitor("value,timeStamp");
    PvaClientMonitorDataPtr monitorData = monitor->getData();
    PvaClientPutPtr put = pva->channel(recordName,provider,2.0)->put("");
    PvaClientPutDataPtr putData = put->getData();
    for(size_t ntimes=0; ntimes<5; ++ntimes)
    {
         double value = ntimes;
         cout << "put " << value << endl;
         putData->putDouble(value); put->put();
         if(!monitor->waitEvent(.1)) {
               cout << "waitEvent returned false. Why???";
               continue;
         } else while(true) {
             cout << "monitor " << monitorData->getDouble() << endl;
             cout << "changed\n";
             monitorData->showChanged(cout);
             cout << "overrun\n";
             monitorData->showOverrun(cout);
             monitor->releaseEvent();
             if(!monitor->poll()) break;
         }
     }
     cout << "__monitor returning\n";
}


int main(int argc,char *argv[])
{
    string provider("pva");
    string channelName("DBRdouble00");
    if(argc==2 && string(argv[1])==string("-help")) {
        cout << "provider  channelName" << endl;
        cout << "default" << endl;
        cout << provider << " " << channelName  << endl;
        return 0;
    }
    if(argc>1) provider = argv[1];
    if(argc>2) channelName = argv[2];
    cout << "_____testRAII starting_______\n";
    try {
        PvaClientPtr pva= PvaClient::get(provider);
//PvaClient::setDebug(true);
        getQuick(pva,channelName,provider);
        put(pva,channelName,provider);
        getLongWay(pva,channelName,provider);
        monitor(pva,channelName,provider);
        cout << "_____testRAII done_______\n";
    } catch (std::runtime_error e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    cout << "after pvaClient looses context\n";
    return 0;
}
