/*pvaClientTest.cpp */
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

class RequesterImpl : public Requester
{
public:
    RequesterImpl(){}
    string getRequesterName()
    {
         static string name("RequesterImpl");
         return name;
    }
    void message(
        string const & message,
        MessageType messageType)
    {
        testDiag("message: '%s'", message.c_str());
    }
    void destroy() {}
};


static void example(PvaClientPtr const &pva)
{
    testDiag("== requester and cache tests ==");

    RequesterPtr requester(new RequesterImpl());
    pva->setRequester(requester);
    pva->message("this is a test",infoMessage);
    testOk(pva->getRequesterName()=="RequesterImpl","getRequesterName");

    pva->clearRequester();
    pva->message("# this is a test",infoMessage);
    testOk(pva->getRequesterName()=="pvaClient","getRequesterName");

    try {
        // connect with pvAccess
        double value = pva->channel("DBRdouble01")
            -> get() -> getData() -> getDouble();
        testDiag("value = %g", value);
        // after default provider:
        testOk(pva->cacheSize()==1,"cacheSize should be 1");

        value = pva->channel("DBRdouble01","ca",2.0)
            -> get() -> getData() -> getDouble();
        // after ca provider
        // pva->showCache();
        testOk(pva->cacheSize()==2,"cacheSize should be 2");

        value = pva->channel("DBRdouble01")
            -> get() -> getData() -> getDouble();
        value = pva->channel("DBRdouble01","ca",2.0)
            -> get() -> getData() -> getDouble();
        // after two more requests
        // pva->showCache();
        testOk(pva->cacheSize()==2,"cacheSize should still be 2");

        PvaClientChannelPtr pvaChannel = pva->createChannel("DBRstring01");
        pvaChannel->connect();
        PvaClientGetPtr pvaGet = pvaChannel->createGet();
        PvaClientPutPtr pvaPut = pvaChannel->createPut();
        pvaGet->connect();
        pvaPut->connect();

        PvaClientPutDataPtr pvaPutData = pvaPut->getData();
        pvaPutData->putString("test");
        pvaPut->put();
        pvaGet->get();
        testOk(pvaGet->getData()->getString() == "test", "string round-trip");
        testOk(pva->cacheSize()==2,"cacheSize should be 2");
    } catch (std::runtime_error e) {
        testAbort("channel example exception '%s'", e.what());
    }
}


MAIN(pvaClientTest)
{
    testPlan(7);
    testDiag("=== pvaClientTest ===");

    PvaClientPtr pvaClient = PvaClient::create();
    example(pvaClient);

    testDone();
    return 0;
}
