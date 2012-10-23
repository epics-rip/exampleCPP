/*ArchiverServiceRPCMain.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS exampleCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/* Author: Marty Kraimer */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <memory>
#include <iostream>

#include <cantProceed.h>
#include <epicsStdio.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <epicsThread.h>

#include <epicsExport.h>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/pvAccess.h>
#include <pv/rpcServer.h>

#include <ArchiverServiceRPC.h>


using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;


int main(int argc,char *argv[])
{
    using namespace  channelArchiverService;

    if(argc < 3)
    {
        cerr << "usage: ArchiverServiceRPC index channel" << endl;
        exit(1);
    }

    char * index = argv[1];
    char * channel = argv[2];
    RPCServer server;

    server.registerService(channel, RPCService::shared_pointer(new ArchiverServiceRPC(index)));
    // you can register as many services as you want here ...

    server.printInfo();
    server.run();

    return 0;
}

