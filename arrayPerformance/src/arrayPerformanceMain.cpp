// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/*arrayPerformanceMain.cpp */

/**
 * @author mrk
 * @date 2013.08.08
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
    bool result(false);
    string recordName;
    recordName = "arrayPerformance";
    size_t size = 10000000;
    double delay = .0001;
    string providerName("local");
    size_t nMonitor = 1;
    int queueSize = 2;

    if(argc==2 && string(argv[1])==string("-help")) {
        cout << "recordName size";
        cout << " delay providerName nMonitor queueSize" << endl;
        cout << "default" << endl;
        cout << recordName << " ";
        cout << size << " ";
        cout << delay << " ";
        cout << providerName << " ";
        cout << nMonitor << " ";
        cout << queueSize << endl;
        return 0;
    }
    if(argc>1) recordName = argv[1];
    if(argc>2) size = strtoul(argv[2],0,0);
    if(argc>3) delay = atof(argv[3]);
    if(argc>4) providerName = argv[4];
    if(argc>5) nMonitor = strtoul(argv[5],0,0);
    if(argc>6) queueSize = strtol(argv[6],0,0);
    cout << recordName << " ";
    cout << size << " ";
    cout << delay << " ";
    cout << providerName << " ";
    cout << nMonitor << " ";
    cout << queueSize << endl;
    PVDatabasePtr master = PVDatabase::getMaster();
    ChannelProviderLocalPtr channelProvider = getChannelProviderLocal();
    PVRecordPtr pvRecord;
    pvRecord = ArrayPerformance::create(recordName,size,delay);
    result = master->addRecord(pvRecord);
    PVRecordPtr arrayPreformance = pvRecord;
    arrayPreformance->setTraceLevel(0);
    pvRecord = TraceRecord::create("traceRecordPGRPC");
    result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;
    pvRecord.reset();
    ServerContext::shared_pointer pvaServer = 
        startPVAServer(PVACCESS_ALL_PROVIDERS,0,true,true);
    
    epicsThreadSleep(1.0);
    try {
        std::vector<LongArrayMonitorPtr> longArrayMonitor(nMonitor);
        for(size_t i=0; i<nMonitor; ++i) {
           longArrayMonitor[i] = LongArrayMonitorPtr(
                new LongArrayMonitor(providerName,recordName,queueSize));
        }
        
        cout << "arrayPerformance\n";
        string str;
        while(true) {
            cout << "Type exit to stop: \n";
            getline(cin,str);
            if(str.compare("exit")==0) {
                 exit(0);
            }
        }
    } catch (std::runtime_error e) {
        cout << "exception " << e.what() << endl;
        exit(1);
    }
    return 0;
}

