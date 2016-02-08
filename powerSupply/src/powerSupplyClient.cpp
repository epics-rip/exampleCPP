// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/*powerSupplyClient.cpp */

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


static void example(PvaClientPtr const &pvaClient)
{
    PvaClientChannelPtr pvaChannel;
    try {
        pvaChannel = pvaClient->createChannel("powerSupply");
        pvaChannel->connect(2.0);
    } catch (std::runtime_error e) {
        cout << "exception " << e.what() << endl;
        return;
    }
    Status status;
    PvaClientPutGetPtr putGet;
    PvaClientPutDataPtr putData;
    PvaClientGetDataPtr getData;
    try {
        putGet = pvaChannel->createPutGet(
            "putField(power.value,voltage.value)getField()");
        putGet->issueConnect();
        status = putGet->waitConnect();
        if(!status.isOK()) {cout << " createPutGet failed\n"; return;}
        putData = putGet->getPutData();
        getData = putGet->getGetData();
        if (!putData || !getData) {cout << " createPutGet failed\n"; return;}
    } catch (std::runtime_error e) {
        cout << "exception " << e.what() << endl;
        return;
    }

    PVStructurePtr pvStructure;
    PVDoublePtr putPower, putVoltage, putCurrent;
    try {
        pvStructure = putData->getPVStructure();
        putPower = pvStructure->getSubField<PVDouble>("power.value");
        putVoltage = pvStructure->getSubField<PVDouble>("voltage.value");
        putCurrent = pvStructure->getSubField<PVDouble>("current.value");
    } catch (std::runtime_error e) {
        cout << "exception " << e.what() << endl;
        return;
    }

    try {
        putPower->put(5.0);
        putVoltage->put(5.0);
        putGet->putGet();
    } catch (std::runtime_error e) {
        cout << "exception " << e.what() << endl;
        return;
    }

    PVDoublePtr gotPower, gotVoltage, gotCurrent;
    try {
        pvStructure = getData->getPVStructure();
        gotPower = pvStructure->getSubField<PVDouble>("power.value");
        if(gotPower->get() == 5.0) cout <<  "returned correct power\n";
        gotVoltage = pvStructure->getSubField<PVDouble>("voltage.value");
        if(gotVoltage && gotVoltage->get() == 5.0) cout <<  "returned correct voltage\n";
        gotCurrent = pvStructure->getSubField<PVDouble>("current.value");
        if(gotCurrent && gotCurrent->get() == 1.0) cout <<  "returned correct current\n";

        putPower->put(10.0);
        putGet->putGet();

        pvStructure = getData->getPVStructure();
        gotPower = pvStructure->getSubField<PVDouble>("power.value");
        if(gotPower && gotPower->get() == 10.0) cout << "returned correct power\n";
        gotVoltage = pvStructure->getSubField<PVDouble>("voltage.value");
        if(gotVoltage && gotVoltage->get() == 5.0) cout << "returned correct voltage\n";
        gotCurrent = pvStructure->getSubField<PVDouble>("current.value");
        if(gotCurrent && gotCurrent->get() == 2.0) cout << "returned correct current\n";
    } catch (std::runtime_error e) {
        cout << "exception " << e.what() << endl;
        return;
    }

    try {
        putPower->put(5.0);
        putVoltage->put(0.0);
        putGet->putGet();
    } catch (std::runtime_error e) {
        cout << "exception " << e.what() << endl;
        return;
    }
}

int main(int argc,char *argv[])
{
    PvaClientPtr pva = PvaClient::create();
    example(pva);
    return 0;
}

