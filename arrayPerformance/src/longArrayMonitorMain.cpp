// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/*longArrayMonitorMain.cpp */

/**
 * @author mrk
 * @date 2013.08.10
 */

/* Author: Marty Kraimer */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <memory>
#include <iostream>
#include <vector>

#include <pv/standardField.h>
#include <pv/standardPVField.h>
#include <pv/traceRecord.h>
#include <pv/channelProviderLocal.h>
#include <pv/serverContext.h>
#include <pv/clientFactory.h>

#include <pv/arrayPerformance.h>
#include <pv/longArrayMonitor.h>

using namespace std;
using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::exampleCPP::arrayPerformance;

int main(int argc,char *argv[])
{
    string channelName("arrayPerformance");
    int queueSize = 2;
    double waitTime = 0.0;
    if(argc==2 && string(argv[1])==string("-help")) {
        cout << "longArrayMonitorMain channelName queueSize waitTime" << endl;
        cout << "default" << endl;
        cout << "longArrayMonitorMain " << channelName << " ";
        cout << queueSize  << " ";
        cout << "0.0"  << endl;
        return 0;
    }
    ClientFactory::start();
    if(argc>1) channelName = argv[1];
    if(argc>2) queueSize = strtol(argv[2],0,0);
    if(argc>3) waitTime = atof(argv[3]);
    cout << "longArrayMonitorMain " << channelName << " ";
    cout << queueSize << " ";
    cout << waitTime << endl;
    LongArrayMonitorPtr longArrayMonitor
         = LongArrayMonitor::create("pvAccess",channelName,queueSize,waitTime);
    longArrayMonitor->start();
    cout << "longArrayMonitor\n";
    string str;
    while(true) {
        cout << "Type exit to stop: \n";
        getline(cin,str);
        if(str.compare("exit")==0) break;

    }
    longArrayMonitor->destroy();
    longArrayMonitor.reset();
    ClientFactory::stop();
    epicsThreadSleep(1.0);
    return 0;
}

