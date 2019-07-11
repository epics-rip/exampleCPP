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
#include <pv/pvAlarm.h>
#include <pv/channelProviderLocal.h>

// The following must be the last include for code powerSupply uses
#include <epicsExport.h>
#define epicsExportSharedSymbols
#include "pv/powerSupplyRecord.h"

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::exampleCPP::powerSupply;
using namespace std;


static const iocshArg testArg0 = { "recordName", iocshArgString };
static const iocshArg *testArgs[] = {
    &testArg0};

static const iocshFuncDef powerSupplyFuncDef = {
    "powerSupplyCreateRecord", 1, testArgs};
static void powerSupplyCallFunc(const iocshArgBuf *args)
{
    char *recordName = args[0].sval;
    if(!recordName) {
        throw std::runtime_error("powerSupplyCreateRecord invalid number of arguments");
    }
    PowerSupplyRecordPtr record = PowerSupplyRecord::create(recordName);
    bool result = PVDatabase::getMaster()->addRecord(record);
    if(!result) cout << "recordname" << " not added" << endl;
}

static void powerSupplyRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&powerSupplyFuncDef, powerSupplyCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(powerSupplyRegister);
}
