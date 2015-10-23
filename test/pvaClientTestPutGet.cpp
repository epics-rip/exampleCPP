/*pvaClientTestPutGet.cpp */
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

#include <pv/pvaClient.h>
#include <epicsUnitTest.h>
#include <testMain.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;


static void example(PvaClientPtr const &pvaClient)
{
    testDiag("== example ==");

    PvaClientChannelPtr pvaChannel;
    try {
        pvaChannel = pvaClient->createChannel("PVRpowerSupply");
        pvaChannel->connect(2.0);
        testDiag("channel connected");
    } catch (std::runtime_error e) {
        testAbort("channel connection exception '%s'", e.what());
    }

    PvaClientPutGetPtr putGet;
    PvaClientPutDataPtr putData;
    PvaClientGetDataPtr getData;
    try {
        putGet = pvaChannel->createPutGet(
            "putField(power.value,voltage.value)getField()");
        testDiag("putGet created");
        putData = putGet->getPutData();
        getData = putGet->getGetData();
        if (!putData || !getData)
            testAbort("NULL data pointer from putGet");
    } catch (std::runtime_error e) {
        testAbort("putGet connection exception '%s'", e.what());
    }

    PVStructurePtr pvStructure;
    PVDoublePtr putPower, putVoltage, putCurrent;
    try {
        pvStructure = putData->getPVStructure();
        putPower = pvStructure->getSubField<PVDouble>("power.value");
        testOk(!!putPower, "putField power.value exists");
        putVoltage = pvStructure->getSubField<PVDouble>("voltage.value");
        testOk(!!putVoltage, "putField voltage.value exists");
        putCurrent = pvStructure->getSubField<PVDouble>("current.value");
        testOk(!putCurrent, "putField current.value not present");
    } catch (std::runtime_error e) {
        testAbort("structure connection exception '%s'", e.what());
    }

    try {
        testDiag("Setting Power and Voltage to 5.0");
        putPower->put(5.0);
        putVoltage->put(5.0);
        putGet->putGet();
    } catch (std::runtime_error e) {
        testAbort("putGet exception '%s'", e.what());
    }

    PVDoublePtr gotPower, gotVoltage, gotCurrent;
    try {
        pvStructure = getData->getPVStructure();
        gotPower = pvStructure->getSubField<PVDouble>("power.value");
        testOk(gotPower && gotPower->get() == 5.0, "returned correct power");
        gotVoltage = pvStructure->getSubField<PVDouble>("voltage.value");
        testOk(gotVoltage && gotVoltage->get() == 5.0, "returned correct voltage");
        gotCurrent = pvStructure->getSubField<PVDouble>("current.value");
        testOk(gotCurrent && gotCurrent->get() == 1.0, "returned correct current");

        testDiag("Setting Power to 10.0");
        putPower->put(10.0);
        putGet->putGet();

        pvStructure = getData->getPVStructure();
        gotPower = pvStructure->getSubField<PVDouble>("power.value");
        testOk(gotPower && gotPower->get() == 10.0, "returned correct power");
        gotVoltage = pvStructure->getSubField<PVDouble>("voltage.value");
        testOk(gotVoltage && gotVoltage->get() == 5.0, "returned correct voltage");
        gotCurrent = pvStructure->getSubField<PVDouble>("current.value");
        testOk(gotCurrent && gotCurrent->get() == 2.0, "returned correct current");
    } catch (std::runtime_error e) {
        testAbort("exception '%s'", e.what());
    }
}


MAIN(pvaClientTestPutGet)
{
    testPlan(9);
    testDiag("=== pvaClientTestPutGet ===");

    PvaClientPtr pvaClient = PvaClient::create();
    example(pvaClient);

    testDone();
    return 0;
}
