/*exampleDatabase.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
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
#include <vector>
#include <iostream>

#include <pv/standardField.h>
#include <pv/standardPVField.h>
#include <pv/channelProviderLocal.h>
#include <pv/serverContext.h>
#include <pv/traceRecord.h>

#include <pv/powerSupply.h>

#define epicsExportSharedSymbols
#include <pv/exampleDatabase.h>
#include <pv/exampleHello.h>
#include <pv/exampleHelloRPC.h>

using namespace std;
using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;
using namespace epics::exampleHello;
using namespace epics::exampleHelloRPC;

static FieldCreatePtr fieldCreate = getFieldCreate();
static StandardFieldPtr standardField = getStandardField();
static PVDataCreatePtr pvDataCreate = getPVDataCreate();
static StandardPVFieldPtr standardPVField = getStandardPVField();

static void createStructureArrayRecord(
    PVDatabasePtr const &master,
    string const &recordName)
{
    StructureConstPtr top = fieldCreate->createFieldBuilder()->
         addNestedStructureArray("value")->
             add("name",pvString)->
             add("value",pvString)->
             endNested()->
         createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);
    PVRecordPtr pvRecord = PVRecord::create(recordName,pvStructure);
    bool result = master->addRecord(pvRecord); 
    if(!result) cout<< "record " << recordName << " not added" << endl;
}

static void createRegularUnionRecord(
    PVDatabasePtr const &master,
    string const &recordName)
{
    StructureConstPtr top = fieldCreate->createFieldBuilder()->
         addNestedUnion("value")->
             add("string",pvString)->
             addArray("stringArray",pvString)->
             endNested()->
         createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);
    PVRecordPtr pvRecord = PVRecord::create(recordName,pvStructure);
    bool result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;
}

static void createVariantUnionRecord(
    PVDatabasePtr const &master,
    string const &recordName)
{
    StructureConstPtr top = fieldCreate->createFieldBuilder()->
         add("value",fieldCreate->createVariantUnion())->
         createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);
    PVRecordPtr pvRecord = PVRecord::create(recordName,pvStructure);
    bool result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;
}

static void createRegularUnionArrayRecord(
    PVDatabasePtr const &master,
    string const &recordName)
{
    StructureConstPtr top = fieldCreate->createFieldBuilder()->
         addNestedUnionArray("value")->
             add("string",pvString)->
             addArray("stringArray",pvString)->
             endNested()->
         createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);
    PVRecordPtr pvRecord = PVRecord::create(recordName,pvStructure);
    bool result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;
}

static void createVariantUnionArrayRecord(
    PVDatabasePtr const &master,
    string const &recordName)
{
    StructureConstPtr top = fieldCreate->createFieldBuilder()->
         addArray("value",fieldCreate->createVariantUnion())->
         createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);
    PVRecordPtr pvRecord = PVRecord::create(recordName,pvStructure);
    bool result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;
}

static void createDumbPowerSupplyRecord(
    PVDatabasePtr const &master,
    string const &recordName)
{
     StructureConstPtr top = fieldCreate->createFieldBuilder()->
         add("alarm",standardField->alarm()) ->
            add("timeStamp",standardField->timeStamp()) ->
            addNestedStructure("power") ->
               add("value",pvDouble) ->
               add("alarm",standardField->alarm()) ->
               endNested()->
            addNestedStructure("voltage") ->
               add("value",pvDouble) ->
               add("alarm",standardField->alarm()) ->
               endNested()->
            addNestedStructure("current") ->
               add("value",pvDouble) ->
               add("alarm",standardField->alarm()) ->
               endNested()->
            createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(top);
    PVRecordPtr pvRecord = PVRecord::create(recordName,pvStructure);
    bool result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;
}


static void createRecords(
    PVDatabasePtr const &master,
    ScalarType scalarType,
    string const &recordNamePrefix,
    string const &properties)
{
    string recordName = recordNamePrefix;
    PVStructurePtr pvStructure = standardPVField->scalar(scalarType,properties);
    PVRecordPtr pvRecord = PVRecord::create(recordName,pvStructure);
    bool result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;
    recordName += "Array";
    pvStructure = standardPVField->scalarArray(scalarType,properties);
    pvRecord = PVRecord::create(recordName,pvStructure);
    result = master->addRecord(pvRecord);
}

void ExampleDatabase::create()
{
    PVDatabasePtr master = PVDatabase::getMaster();
    PVRecordPtr pvRecord;
    string recordName;
    bool result(false);
    recordName = "PVRtraceRecordPGRPC";
    pvRecord = TraceRecord::create(recordName);
    result = master->addRecord(pvRecord);
    if(!result) cout<< "record " << recordName << " not added" << endl;
    string properties;
    properties = "alarm,timeStamp";
    createRecords(master,pvByte,"PVRbyte01",properties);
    createRecords(master,pvShort,"PVRshort01",properties);
    createRecords(master,pvLong,"PVRlong01",properties);
    createRecords(master,pvUByte,"PVRubyte01",properties);
    createRecords(master,pvUInt,"PVRuint01",properties);
    createRecords(master,pvUShort,"PVRushort01",properties);
    createRecords(master,pvULong,"PVRulong01",properties);
    createRecords(master,pvFloat,"PVRfloat01",properties);

    StringArray choices(2);
    choices[0] = "zero";
    choices[1] = "one";
    master->addRecord(PVRecord::create(
         "PVRenum",standardPVField->enumerated(choices,properties)));

    createRecords(master,pvBoolean,"PVRboolean",properties);
    createRecords(master,pvByte,"PVRbyte",properties);
    createRecords(master,pvShort,"PVRshort",properties);
    createRecords(master,pvInt,"PVRint",properties);
    createRecords(master,pvLong,"PVRlong",properties);
    createRecords(master,pvFloat,"PVRfloat",properties);
    createRecords(master,pvDouble,"PVRdouble",properties);
    createRecords(master,pvDouble,"PVRdouble01",properties);
    createRecords(master,pvDouble,"PVRdouble02",properties);
    createRecords(master,pvDouble,"PVRdouble03",properties);
    createRecords(master,pvDouble,"PVRdouble04",properties);
    createRecords(master,pvDouble,"PVRdouble05",properties);
    createRecords(master,pvString,"PVRstring",properties);
    createStructureArrayRecord(master,"PVRstructureArray");
    createRegularUnionRecord(master,"PVRregularUnion");
    createVariantUnionRecord(master,"PVRvariantUnion");
    createRegularUnionArrayRecord(master,"PVRregularUnionArray");
    createVariantUnionArrayRecord(master,"PVRvariantUnionArray");
    createDumbPowerSupplyRecord(master,"PVRdumbPowerSupply");
    recordName = "PVRpowerSupply";
    PVStructurePtr pvStructure = createPowerSupply();
    PowerSupplyPtr psr = PowerSupply::create(recordName,pvStructure);
    if(!psr) {
          cout << "PowerSupply::create failed" << endl;
    } else {
        result = master->addRecord(psr);
        if(!result) cout<< "record " << recordName << " not added" << endl;
    }
    recordName = "PVRhelloPutGet";
    result = master->addRecord(ExampleHello::create(recordName));
    if(!result) cout<< "record " << recordName << " not added" << endl;
    RPCServer::shared_pointer server(new RPCServer());
    server->registerService("helloRPC",ExampleHelloRPC::create());
}

