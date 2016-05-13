// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/*monitorForever.cpp */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <epicsThread.h>

#include <iostream>

#include <pv/pvaClient.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

int main(int argc,char *argv[])
{
    cout << "_____monitorForever starting_______\n";
    try {
        PvaClientPtr pva = PvaClient::get("pva");
        PvaClientMonitorPtr monitor = pva->channel("PVRdouble")->monitor("");
        PvaClientMonitorDataPtr monitorData = monitor->getData();
        while(true) {
            epicsThreadSleep(.1);
            while(monitor->poll()) {
                monitorData = monitor->getData();
                cout << "monitor " << monitorData->getDouble() << endl;
                cout << "changed\n";
                monitorData->showChanged(cout);
                cout << "overrun\n";
                monitorData->showOverrun(cout);
                monitor->releaseEvent();
            }
            int c = std::cin.peek();  // peek character
            if ( c == EOF ) continue;
            cout << "Type exit to stop: \n";
            string str;
            getline(cin,str);
            if(str.compare("exit")==0) break;
        }
    } catch (std::runtime_error e) {
            cerr << "exception " << e.what() << endl;
            return 1;
    }
    return 0;
}
