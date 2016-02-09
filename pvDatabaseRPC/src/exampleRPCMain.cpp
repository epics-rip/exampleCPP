// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/*ExampleRPCMain.cpp */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <string>
#include <iostream>

#include <pv/exampleRPC.h>
#include <pv/channelProviderLocal.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::exampleCPP::exampleRPC;

int main(int argc,char *argv[])
{
    PVDatabasePtr master = PVDatabase::getMaster();
    PVRecordPtr pvRecord;
    bool result = false;
    string recordName;

    recordName = "mydevice";
    pvRecord = ExampleRPC::create(recordName);
    result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;

    ContextLocal::shared_pointer contextLocal = ContextLocal::create();
    contextLocal->start();

    PVStringArrayPtr pvNames = master->getRecordNames();
    shared_vector<const string> names = pvNames->view();
    for(size_t i=0; i<names.size(); ++i) cout << names[i] << endl;

    contextLocal->waitForExit();

    return 0;
}

