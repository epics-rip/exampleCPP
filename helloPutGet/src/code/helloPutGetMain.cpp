// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/*HelloPutGetMain.cpp */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <string>
#include <iostream>

#include <pv/helloPutGet.h>
#include <pv/channelProviderLocal.h>
#include <pv/serverContext.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::helloPutGet;

int main(int argc,char *argv[])
{
    PVDatabasePtr master = PVDatabase::getMaster();
    ChannelProviderLocalPtr channelProvider = getChannelProviderLocal();
    PVRecordPtr pvRecord;
    bool result = false;
    string recordName;

    recordName = "helloPutGet";
    pvRecord = HelloPutGet::create(recordName);
    result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;
    ServerContext::shared_pointer ctx =
        startPVAServer(PVACCESS_ALL_PROVIDERS,0,true,true);
    master.reset();
    string str;
    while(true) {
        cout << "Type exit to stop: \n";
        getline(cin,str);
        if(str.compare("exit")==0) break;

    }
    ctx->destroy();
    channelProvider->destroy();
    epicsThreadSleep(3.0);
    return 0;
}

