// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/*examplePvaClientProcess.cpp */

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


static void exampleProcess(PvaClientPtr const &pva)
{
    cout << "example process\n";
    try {
        PvaClientChannelPtr channel = pva->channel("PVRdouble");
        PvaClientProcessPtr process = channel->createProcess();
        process->process();
        cout <<  channel->get("field()")->getData()->showChanged(cout) << endl;
        process->process();
        cout <<  channel->get("field()")->getData()->showChanged(cout) << endl;
    } catch (std::runtime_error e) {
        cout << "exception " << e.what() << endl;
    }
}


int main(int argc,char *argv[])
{
    PvaClientPtr pva = PvaClient::create();
    exampleProcess(pva);
    return 0;
}
