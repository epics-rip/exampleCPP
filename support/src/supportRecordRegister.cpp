/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.07.24
 */


/* Author: Marty Kraimer */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <memory>

#include <cantProceed.h>
#include <epicsStdio.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <epicsThread.h>
#include <iocsh.h>

#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/pvAccess.h>
#include <pv/pvDatabase.h>

#include <epicsExport.h>
#include <pv/supportRecord.h>

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::exampleCPP::support;
using namespace std;

static const iocshArg testArg0 = { "recordName", iocshArgString };
static const iocshArg testArg1 = { "scalarType", iocshArgString };
static const iocshArg *testArgs[] = {
    &testArg0,&testArg1};

static const iocshFuncDef supportRecordFuncDef = {"supportRecordCreate", 2,testArgs};

static void supportRecordCallFunc(const iocshArgBuf *args)
{
    char *recordName = args[0].sval;
    if(!recordName) {
        throw std::runtime_error("supportRecordCreate invalid number of arguments");
    }
    char *stype = args[1].sval;
    epics::pvData::ScalarType scalarType  = epics::pvData::pvDouble;
    if(stype) {
        string val(stype);
        if(val.compare("pvByte")==0) {
            scalarType = epics::pvData::pvByte;
        } else if(val.compare("pvShort")==0) {
            scalarType = epics::pvData::pvShort;
        } else if(val.compare("pvInt")==0) {
            scalarType = epics::pvData::pvInt;
        } else if(val.compare("pvLong")==0) {
            scalarType = epics::pvData::pvLong;
        } else if(val.compare("pvFloat")==0) {
            scalarType = epics::pvData::pvFloat;
        } else if(val.compare("pvDouble")==0) {
            scalarType = epics::pvData::pvDouble;
        } else if(val.compare("pvUByte")==0) {
            scalarType = epics::pvData::pvUByte;
        } else if(val.compare("pvUShort")==0) {
            scalarType = epics::pvData::pvUShort;
        } else if(val.compare("pvUInt")==0) {
            scalarType = epics::pvData::pvUInt;
        } else if(val.compare("pvULong")==0) {
            scalarType = epics::pvData::pvULong;
        } else {
             cout << val << " is not a support scalar type\n";
             return; 
        }
    }
    SupportRecordPtr record = SupportRecord::create(recordName,scalarType);
    bool result = PVDatabase::getMaster()->addRecord(record);
    if(!result) cout << "recordname" << " not added" << endl;
}

static void supportRecordRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&supportRecordFuncDef, supportRecordCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(supportRecordRegister);
}
