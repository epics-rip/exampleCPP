/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <string>
#include <iostream>

#include <pv/traceRecord.h>
#include <pv/removeRecord.h>
#include <pv/processRecord.h>
#include <pv/supportRecord.h>
#include <pv/scalarArrayRecord.h>
#include <pv/scalarRecord.h>

#include <pv/channelProviderLocal.h>
#include <pv/serverContext.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::exampleCPP::support;

int main(int argc,char *argv[])
{
    PVDatabasePtr master = PVDatabase::getMaster();
    ChannelProviderLocalPtr channelProvider = getChannelProviderLocal();
    PVRecordPtr pvRecord;
    string recordName;

    recordName = "PVRtraceRecord";
    pvRecord = TraceRecord::create(recordName);
    master->addRecord(pvRecord);
    recordName = "PVRremoveRecord";
    pvRecord = RemoveRecord::create(recordName);
    master->addRecord(pvRecord);
    recordName = "PVRprocessRecord";
    pvRecord = ProcessRecord::create(recordName,.5);
    master->addRecord(pvRecord);
    recordName = "PVRsupportDouble";
    pvRecord = SupportRecord::create(recordName,epics::pvData::pvDouble);
    master->addRecord(pvRecord);
    recordName = "PVRsupportUByte";
    pvRecord = SupportRecord::create(recordName,epics::pvData::pvUByte);
    master->addRecord(pvRecord);
    recordName = "PVRscalarArrayDouble";
    pvRecord = ScalarArrayRecord::create(recordName,epics::pvData::pvDouble);
    master->addRecord(pvRecord);
    recordName = "PVRscalarArrayUByte";
    pvRecord = ScalarArrayRecord::create(recordName,epics::pvData::pvUByte);
    master->addRecord(pvRecord);
    recordName = "PVRscalarArrayString";
    pvRecord = ScalarArrayRecord::create(recordName,epics::pvData::pvString);
    master->addRecord(pvRecord);
    recordName = "PVRscalarArrayBoolean";
    pvRecord = ScalarArrayRecord::create(recordName,epics::pvData::pvBoolean);
    master->addRecord(pvRecord);
    recordName = "PVRscalarDouble";
    pvRecord = ScalarRecord::create(recordName,epics::pvData::pvDouble,-10,10,.5);
    master->addRecord(pvRecord);
    recordName = "PVRscalarUByte";
    pvRecord = ScalarRecord::create(recordName,epics::pvData::pvUByte,0,20,1);
    master->addRecord(pvRecord);


    ServerContext::shared_pointer ctx =
        startPVAServer("local",0,true,true);
    string str;
    while(true) {
        cout << "Type exit to stop: \n";
        getline(cin,str);
        if(str.compare("exit")==0) break;

    }
    return 0;
}

