/*pvaClientTestMonitorData.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <iostream>

#include <epicsUnitTest.h>
#include <testMain.h>

#include <pv/pvaClient.h>
#include <pv/bitSet.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

static PvaClientPtr pvaClient = PvaClient::create();
static FieldCreatePtr fieldCreate = getFieldCreate();
static StandardFieldPtr standardField = getStandardField();
static PVDataCreatePtr pvDataCreate = getPVDataCreate();

static void testDouble()
{
    testDiag("== testDouble ==");
    StructureConstPtr structure =
       fieldCreate->createFieldBuilder() ->
            add("alarm",standardField->alarm()) ->
            add("timeStamp",standardField->timeStamp()) ->
            add("value",pvDouble) ->
            createStructure();

    PvaClientMonitorDataPtr pvaData = PvaClientMonitorData::create(structure);
    MonitorElementPtr monitorElement(new MonitorElement(pvDataCreate->createPVStructure(pvaData->getStructure())));
    pvaData->setData(monitorElement);

    PVDoublePtr pvDouble = pvaData->getPVStructure() ->
        getSubField<PVDouble>("value");
    size_t valueOffset = pvDouble->getFieldOffset();

    BitSetPtr change = pvaData->getChangedBitSet();
    pvDouble->put(5.0);
    change->set(pvDouble->getFieldOffset());

    testOk(change->cardinality()==1,"1 field changed");
    testOk(change->get(valueOffset),"value changed");

    testOk(pvaData->hasValue(),"hasValue");
    testOk(pvaData->isValueScalar(),"isValueScalar");
    testOk(!pvaData->isValueScalarArray(),"!isValueScalarArray");

    try {
        testOk(!!pvaData->getValue(), "getValue");
    } catch (std::runtime_error e) {
        testFail("getValue exception '%s'", e.what());
    }

    try {
        testOk(!!pvaData->getScalarValue(), "getScalarValue");
    } catch (std::runtime_error e) {
        testFail("getScalarValue exception '%s'", e.what());
    }
    try {
        testOk(!pvaData->getArrayValue(), "!getArrayValue");
    } catch (std::runtime_error e) {
        testPass("getArrayValue exception '%s'", e.what());
    }
    try {
        testOk(!pvaData->getScalarArrayValue(), "!getScalarArrayValue");
    } catch (std::runtime_error e) {
        testPass("getScalarArrayValue exception '%s'", e.what());
    }

    try {
        testOk(pvaData->getDouble() == 5.0, "getDouble value");
    } catch (std::runtime_error e) {
        testFail("getDouble exception '%s'", e.what());
    }
    try {
        testOk(pvaData->getString() == "5", "getString value");
    } catch (std::runtime_error e) {
        testFail("getString exception '%s'", e.what());
    }

    try {
        shared_vector<const double> value = pvaData->getDoubleArray();
        testFail("getDoubleArray");
    } catch (std::runtime_error e) {
        testPass("getDoubleArray exception '%s'", e.what());
    }
    try {
        shared_vector<const string> value = pvaData->getStringArray();
        testFail("getStringArray");
    } catch (std::runtime_error e) {
        testPass("getStringArray exception '%s'", e.what());
    }
}

static void testDoubleArray()
{
    testDiag("== testDoubleArray ==");
    StructureConstPtr structure =
       fieldCreate->createFieldBuilder() ->
            add("alarm",standardField->alarm()) ->
            add("timeStamp",standardField->timeStamp()) ->
            addArray("value",pvDouble) ->
            createStructure();

    PvaClientMonitorDataPtr pvaData = PvaClientMonitorData::create(structure);
    MonitorElementPtr monitorElement(new MonitorElement(pvDataCreate->createPVStructure(pvaData->getStructure())));
    pvaData->setData(monitorElement);

    PVDoubleArrayPtr pvalue = pvaData->getPVStructure()->getSubField<PVDoubleArray>("value");

    BitSetPtr change = pvaData->getChangedBitSet();
    size_t valueOffset = pvalue->getFieldOffset();

    size_t len = 5;
    shared_vector<double> value(len);
    for (size_t i=0; i<len; ++i)
        value[i] = i*10.0;
    pvalue->replace(freeze(value));
    change->set(valueOffset);

    testOk(change->cardinality()==1,"1 field changed");
    testOk(change->get(valueOffset),"value changed");

    testOk(pvaData->hasValue(),"hasValue");
    testOk(!pvaData->isValueScalar(),"!isValueScalar");
    testOk(pvaData->isValueScalarArray(),"isValueScalarArray");

    try {
        testOk(!!pvaData->getValue(), "getValue");
    } catch (std::runtime_error e) {
        testFail("getValue exception '%s'", e.what());
    }

    try {
        testOk(!pvaData->getScalarValue(), "!getScalarValue");
    } catch (std::runtime_error e) {
        testPass("getScalarValue exception '%s'", e.what());
    }
    try {
        testOk(!!pvaData->getArrayValue(), "getArrayValue");
    } catch (std::runtime_error e) {
        testFail("getArrayValue exception '%s'", e.what());
    }
    try {
        testOk(!!pvaData->getScalarArrayValue(), "getScalarArrayValue");
    } catch (std::runtime_error e) {
        testPass("getScalarArrayValue exception '%s'", e.what());
    }

    try {
        testFail("getDouble %g", pvaData->getDouble());
    } catch (std::runtime_error e) {
        testPass("getDouble exception '%s'", e.what());
    }
    try {
        testFail("getString %s", pvaData->getString().c_str());
    } catch (std::runtime_error e) {
        testPass("getString exception '%s'", e.what());
    }

    try {
        shared_vector<const double> value = pvaData->getDoubleArray();
        testPass("getDoubleArray");
    } catch (std::runtime_error e) {
        testFail("getDoubleArray exception '%s'", e.what());
    }
    try {
        shared_vector<const string> value = pvaData->getStringArray();
    } catch (std::runtime_error e) {
        testPass("getStringArray exception '%s'", e.what());
    }
}

MAIN(pvaClientTestMonitorData)
{
    testPlan(26);
    testDiag("=== pvaClientTestMonitorData ===");

    testDouble();
    testDoubleArray();

    testDone();
    return 0;
}

