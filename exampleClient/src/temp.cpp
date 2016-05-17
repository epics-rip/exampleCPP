// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/*examplePvaClientGet.cpp */

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


static void exampleDouble(PvaClientPtr const &pva,string const & channelName,string const & providerName )
{
    cout << "__exampleDouble__ channelName " << channelName << " providerName " << providerName << endl;
    double value;
    cout << "long way\n";
    PvaClientChannelPtr pvaChannel = pva->createChannel(channelName,providerName);
    pvaChannel->issueConnect();
    Status status = pvaChannel->waitConnect(2.0);
    if(!status.isOK()) {cout << " connect failed\n"; return;}
    PvaClientGetPtr pvaGet = pvaChannel->createGet();
    pvaGet->issueConnect();
    status = pvaGet->waitConnect();
    if(!status.isOK()) {cout << " createGet failed\n"; return;}
    PvaClientGetDataPtr pvaData = pvaGet->getData();
    value = pvaData->getDouble();
    pvaChannel->destroy();
    cout << "as double " << value << endl;
    cout << "__exampleDouble__ returning\n";
}


int main(int argc,char *argv[])
{
    cout << "_____examplePvaClientGet starting_______\n";
    try {
        PvaClientPtr pva= PvaClient::get("pva ca");
PvaClient::setDebug(true);
        exampleDouble(pva,"PVRdouble","pva");
epicsThreadSleep(1.0);
cout << "after epicsThreadSleep\n";
        cout << "_____examplePvaClientGet done_______\n";
    } catch (std::runtime_error e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
epicsThreadSleep(1.0);
cout << "after epicsThreadSleep\n";
    return 0;
}
