/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2021.03.16
 */


/* Author: Marty Kraimer */
#include <iocsh.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>
#include <pv/alarm.h>
#include <pv/pvAlarm.h>
#include <pv/pvDatabase.h>
#include <pv/pvaClient.h>
// The following must be the last include for code exampleLink uses
#include <epicsExport.h>
#define epicsExportSharedSymbols
#include "pv/putLinkScalarArrayRecord.h"

using namespace epics::pvData;
using namespace epics::nt;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::exampleCPP::exampleLink;
using std::cout;
using std::endl;
using std::string;

static StandardPVFieldPtr standardPVField = getStandardPVField();

static const iocshArg testArg0 = { "recordName", iocshArgString };

static const iocshArg *testArgs[] = {&testArg0};

static const iocshFuncDef putLinkScalarArrayFuncDef = {
    "putLinkScalarArray", 1, testArgs};
static void putLinkScalarArrayCallFunc(const iocshArgBuf *args)
{
    string putLinkScalarArrayRecord("putLinkScalarArray");
    char *sval = args[0].sval;
    if(sval) putLinkScalarArrayRecord = string(sval);
    PVDatabasePtr master = PVDatabase::getMaster();
    bool result(false);
    PutLinkScalarArrayRecordPtr record = PutLinkScalarArrayRecord::create(putLinkScalarArrayRecord);
    if(record) 
        result = master->addRecord(record);
    if(!result) cout << "recordname" << " not added" << endl;
}

static void putLinkScalarArrayRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&putLinkScalarArrayFuncDef, putLinkScalarArrayCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(putLinkScalarArrayRegister);
} 
