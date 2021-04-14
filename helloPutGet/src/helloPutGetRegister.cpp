/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.07.24
 */


/* Author: Marty Kraimer */

#include <iocsh.h>
#include <pv/pvDatabase.h>
#include <pv/pvStructureCopy.h>
#include <pv/channelProviderLocal.h>

// The following must be the last include for code helloPutGet uses
#include <epicsExport.h>
#define epicsExportSharedSymbols
#include "pv/helloPutGetRecord.h"

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::exampleCPP::helloPutGet;
using std::cout;
using std::endl;

static const iocshArg testArg0 = { "recordName", iocshArgString };
static const iocshArg *testArgs[] = {
    &testArg0};

static const iocshFuncDef helloPutGetFuncDef = {
    "helloPutGetCreateRecord", 1, testArgs};
static void helloPutGetCallFunc(const iocshArgBuf *args)
{
    PVDatabasePtr master = PVDatabase::getMaster();
    char *recordName = args[0].sval;
    HelloPutGetRecordPtr record = HelloPutGetRecord::create(recordName);
    bool result = master->addRecord(record);
    if(!result) cout << "recordname" << " not added" << endl;
}

static void helloPutGetRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&helloPutGetFuncDef, helloPutGetCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(helloPutGetRegister);
}
