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


static void example(PvaClientPtr const &pva,string const & channelName,string const & providerName )
{
    cout << "_____example starting_______\n";
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
    cout << "as double " << value << endl;
    cout << "__example__ returning\n";
}


int main(int argc,char *argv[])
{
    cout << "_____testRAII starting_______\n";
    try {
        PvaClientPtr pva= PvaClient::get("pva");
PvaClient::setDebug(true);
        example(pva,"PVRdouble","pva");
cout << "after example\n";
//epicsThreadSleep(3.0);
//cout << "after epicsThreadSleep\n";
        cout << "_____testRAII done_______\n";
    } catch (std::runtime_error e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    cout << "after pvaClient looses context\n";
//epicsThreadSleep(3.0);
//cout << "after epicsThreadSleep\n";
    return 0;
}
