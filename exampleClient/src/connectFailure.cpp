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


static void exampleConnect(PvaClientPtr const &pva,string const & channelName,string const & providerName )
{
    cout << "__exampleConnect__ channelName " << channelName << " providerName " << providerName << endl;
    cout << "long way\n";
    PvaClientChannelPtr pvaChannel = pva->createChannel(channelName,providerName);
    pvaChannel->issueConnect();
    Status status = pvaChannel->waitConnect(2.0);
    if(!status.isOK()) {cout << " connect failed\n"; return;}
    cout << "__exampleConnect__ returning\n";
}

int main(int argc,char *argv[])
{
    cout << "_____connectFailure starting_______\n";
    try {
        PvaClientPtr pva= PvaClient::get("pva ca");
PvaClient::setDebug(true);
        exampleConnect(pva,"PVRdouble","pva");
        cout << "after first call to exampleConnect\n";
epicsThreadSleep(1.0);
cout << "after epicsThreadSleep\n";
        exampleConnect(pva,"PVRdouble","pva");
epicsThreadSleep(1.0);
cout << "after epicsThreadSleep\n";
        cout << "after first call to exampleConnect\n";
        cout << "_____connectFailure done_______\n";
    } catch (std::runtime_error e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
