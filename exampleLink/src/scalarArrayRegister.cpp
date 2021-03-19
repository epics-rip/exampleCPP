/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2021.03.12
 */
#include <iocsh.h>
#include <pv/standardField.h>
#include <pv/pvDatabase.h>
// The following must be the last include for code exampleLink uses
#include <epicsExport.h>
#define epicsExportSharedSymbols

using namespace epics::pvData;
using namespace epics::pvDatabase;
using namespace std;



static const iocshArg arg0 = { "recordName", iocshArgString };
static const iocshArg arg1 = { "scalarType", iocshArgString };
static const iocshArg *args[] = {&arg0,&arg1};

static const iocshFuncDef scalarArrayFuncDef = {"scalarArray", 2,args};

static void scalarArrayCallFunc(const iocshArgBuf *args)
{
    char *sval = args[0].sval;
    if(!sval) {
        throw std::runtime_error("scalarArray recordName not specified");
    }
    string recordName = string(sval);
    sval = args[1].sval;
    if(!sval) {
        throw std::runtime_error("scalarArray scalarType not specified");
    }
    string scalarType = string(sval);
    ScalarType st = epics::pvData::ScalarTypeFunc::getScalarType(scalarType);
    FieldCreatePtr fieldCreate = getFieldCreate();
    StandardFieldPtr standardField = getStandardField();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    StructureConstPtr top = fieldCreate->createFieldBuilder()->
        addArray("value",st) ->
        add("timeStamp",standardField->timeStamp()) ->
        add("alarm",standardField->alarm()) ->
        createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);   
    PVRecordPtr record = PVRecord::create(recordName,pvStructure);
    PVDatabasePtr master = PVDatabase::getMaster();
    bool result =  master->addRecord(record);
    if(!result) cout << "recordname " << recordName << " not added" << endl;
}

static void scalarArrayRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&scalarArrayFuncDef, scalarArrayCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(scalarArrayRegister);
}
