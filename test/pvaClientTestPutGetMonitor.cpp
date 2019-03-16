// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/*pvaClientTestPutGetMonitor.cpp */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <iostream>

#include <pv/pvaClient.h>
#include <epicsUnitTest.h>
#include <epicsThread.h>
#include <testMain.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

bool expectValueChange = true;

class MyMonitor : public PvaClientMonitorRequester
{
public:
    MyMonitor() {}
    virtual ~MyMonitor() {}
    virtual void event(PvaClientMonitorPtr const & monitor)
    {
        testDiag("monitor event");
        PvaClientMonitorDataPtr pvaData = monitor->getData();
        while (monitor->poll()) {
            BitSetPtr changed(pvaData->getChangedBitSet());
            uint32 offsetValue = pvaData->getPVStructure()->getSubField("value")->getFieldOffset();
            bool res = (changed->get(0) || changed->get(offsetValue));
            if(expectValueChange) {
                testOk(res,"value changed");
            } else {
                res = true;
                testOk(res,"value not changed");
            }
            testOk(pvaData->getOverrunBitSet()->isEmpty(), "No overrun");
            monitor->releaseEvent();
        }
    }
};

static void exampleDouble(PvaClientPtr const &pvaClient)
{
    testDiag("== exampleDouble ==");

    PvaClientChannelPtr pvaChannel;
    try {
        pvaChannel = pvaClient->createChannel("PVRdouble");
        pvaChannel->connect(2.0);
        testDiag("channel connected");
    } catch (std::exception& e) {
        testAbort("channel connection exception '%s'", e.what());
    }

    PvaClientPutPtr put;
    PvaClientPutDataPtr putData;
    try {
        put = pvaChannel->createPut();
        putData = put->getData();
        testDiag("put connected");
        if (!putData)
            testAbort("NULL data pointer from putGet");
    } catch (std::exception& e) {
        testAbort("put connection exception '%s'", e.what());
    }

    PvaClientGetPtr get;
    PvaClientGetDataPtr getData;
    try {
        get = pvaChannel->createGet();
        getData = get->getData();
        testDiag("get connected");
        if (!getData)
            testAbort("NULL data pointer from putGet");
    } catch (std::exception& e) {
        testAbort("get connection exception '%s'", e.what());
    }

    PvaClientMonitorRequesterPtr requester(new MyMonitor());
    PvaClientMonitorPtr monitor;
    try {
        monitor = pvaChannel->monitor(requester);
        testDiag("monitor connected");
    } catch (std::exception& e) {
        testAbort("monitor connection exception '%s'", e.what());
    }
    epicsThreadSleep(0.1);  // Allow connection monitor event to fire

    try {
        for (int i=0; i<5; ++i) {
            testDiag("= put %d =", i);

            double out = i;
            putData->putDouble(out);
            put->put();

            get->get();
            double in = getData->getDouble();
            testOk(in == out, "get value matches put");
        }

        PvaClientProcessPtr process = pvaChannel->createProcess();
        process->connect();

        testDiag("= process =");
        expectValueChange = false;
        process->process();
    } catch (std::exception& e) {
        testAbort("exception '%s'", e.what());
    }
}


MAIN(pvaClientTestPutGetMonitor)
{
    testPlan(19);
    testDiag("=== pvaClientTestPutGetMonitor ===");

    PvaClientPtr pvaClient = PvaClient::get("pva");
    exampleDouble(pvaClient);

    testDone();
    return 0;
}
