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

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;


int main(int argc,char *argv[])
{
    cout << "_____exampleLinkClient starting_______\n";
    PvaClientPtr pva = PvaClient::create();
    try {
        PvaClientChannelPtr channel = pva->channel("exampleServer");
        PvaClientPutGetPtr putGet = channel->createPutGet();
        putGet->connect();
        PvaClientPutDataPtr putData = putGet->getPutData();
        PVStructurePtr arg = putData->getPVStructure();
        PVStringPtr pvValue = arg->getSubField<PVString>("argument.value");
        pvValue->put("World");
        putGet->putGet();
        PvaClientGetDataPtr getData = putGet->getGetData();
        cout << getData->getPVStructure() << endl;
    } catch (std::runtime_error e) {
        cout << "exception " << e.what() << endl;
        return 1;
    }
    cout << "_____exampleLinkClient done_______\n";
    return 0;
}

