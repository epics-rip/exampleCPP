/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2021.03.16
 */
#include <iocsh.h>
#include <pv/pvDatabase.h>
// The following must be the last include for code exampleLink uses
#include <epicsExport.h>
#define epicsExportSharedSymbols
#include "pv/putLinkScalarRecord.h"

using namespace epics::pvDatabase;
using namespace epics::exampleCPP::exampleLink;
using namespace std;

static const iocshArg arg0 = { "recordName", iocshArgString };
static const iocshArg *args[] = {&arg0};

static const iocshFuncDef putLinkScalarFuncDef = {
    "putLinkScalar", 1, args};
static void putLinkScalarCallFunc(const iocshArgBuf *args)
{
    string putLinkScalarRecord("putLinkScalar");
    char *sval = args[0].sval;
    if(sval) putLinkScalarRecord = string(sval);
    PVDatabasePtr master = PVDatabase::getMaster();
    bool result(false);
    PutLinkScalarRecordPtr record = PutLinkScalarRecord::create(putLinkScalarRecord);
    if(record) 
        result = master->addRecord(record);
    if(!result) cout << "recordname" << " not added" << endl;
}

static void putLinkScalarRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&putLinkScalarFuncDef, putLinkScalarCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(putLinkScalarRegister);
} 
