/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.07.24
 */


/* Author: Marty Kraimer */
#include <epicsThread.h>
#include <epicsExport.h>
#include <iocsh.h>
#include <pv/channelProviderLocal.h>

#define epicsExportSharedSymbols
#include "pv/exampleRPC.h"

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::exampleCPP::exampleRPC;
using std::cout;
using std::endl;

static const iocshArg testArg0 = { "recordName", iocshArgString };
static const iocshArg *testArgs[] = {
    &testArg0};

static const iocshFuncDef exampleRPCFuncDef = {
    "exampleRPCCreateRecord", 1, testArgs};
static void exampleRPCCallFunc(const iocshArgBuf *args)
{
    PVDatabasePtr master = PVDatabase::getMaster();
    char *recordName = args[0].sval;
    ExampleRPCPtr record = ExampleRPC::create(recordName);
    bool result = master->addRecord(record);
    if(!result) cout << "recordname" << " not added" << endl;
}

static void exampleRPCRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&exampleRPCFuncDef, exampleRPCCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(exampleRPCRegister);
}
