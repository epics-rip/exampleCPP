/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <memory>
#include <vector>
#include <iostream>

#include <pv/channelProviderLocal.h>
#include <pv/serverContext.h>

#include <pv/exampleDatabase.h>

using namespace std;
using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::exampleCPP::database;

int main(int argc,char *argv[])
{
    PVDatabasePtr master = PVDatabase::getMaster();
    ChannelProviderLocalPtr channelProvider = getChannelProviderLocal();
    ExampleDatabase::create();
    ServerContext::shared_pointer ctx =
        startPVAServer("local",0,true,true);
    
    master.reset();
    cout << "exampleDatabase\n";
    string str;
    while(true) {
        cout << "enter: pvdbl or exit \n";
        getline(cin,str);
        if(str.compare("exit")==0) break;
        if(str.compare("pvdbl")==0) {
            PVDatabasePtr master = PVDatabase::getMaster();
            PVStringArrayPtr pvNames = master->getRecordNames();
            PVStringArray::const_svector xxx = pvNames->view();
            for(size_t i=0; i<xxx.size(); ++i) cout<< xxx[i] << endl;
        }
    }
    return 0;
}
