/*examplePvaClientGet.cpp */
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


static void exampleDouble(PvaClientPtr const &pva)
{
    cout << "example double scalar\n";
    double value;
    try {
        cout << "short way\n";
        value =  pva->channel("PVRdouble")->get()->getData()->getDouble();
        cout << "as double " << value << endl;
    } catch (std::runtime_error e) {
        cout << "exception " << e.what() << endl;
    }
    cout << "long way\n";
    PvaClientChannelPtr pvaChannel = pva->createChannel("PVRdouble");
    pvaChannel->issueConnect();
    Status status = pvaChannel->waitConnect(2.0);
    if(!status.isOK()) {cout << " connect failed\n"; return;}
    PvaClientGetPtr pvaGet = pvaChannel->createGet();
    pvaGet->issueConnect();
    status = pvaGet->waitConnect();
    if(!status.isOK()) {cout << " createGet failed\n"; return;}
    PvaClientGetDataPtr pvaData = pvaGet->getData();
    value = pvaData->getDouble();
    cout << "as double " << value << endl;
}

static void exampleDoubleArray(PvaClientPtr const &pva)
{
    cout << "example double array\n";
    shared_vector<const double> value;
    try {
        cout << "short way\n";
        value =  pva->channel("PVRdoubleArray")->get()->getData()->getDoubleArray();
        cout << "as doubleArray " << value << endl;
    } catch (std::runtime_error e) {
        cout << "exception " << e.what() << endl;
    }
    try {
        cout << "long way\n";
        PvaClientChannelPtr pvaChannel = pva->createChannel("PVRdoubleArray");
        pvaChannel->connect(2.0);
        PvaClientGetPtr pvaGet = pvaChannel->createGet();
        PvaClientGetDataPtr pvaData = pvaGet->getData();
        value = pvaData->getDoubleArray();
        cout << "as doubleArray " << value << endl;
    } catch (std::runtime_error e) {
        cout << "exception " << e.what() << endl;
    }
}

static void examplePowerSupply(PvaClientPtr const &pva)
{
    cout << "example powerSupply\n";
    PVStructurePtr pvStructure;
    try {
        cout << "short way\n";
        pvStructure =  pva->channel("PVRpowerSupply")->
            get("field()")->getData()->getPVStructure();
        cout << pvStructure << endl;
    } catch (std::runtime_error e) {
        cout << "exception " << e.what() << endl;
    }
     
}

static void exampleCADouble(PvaClientPtr const &pva)
{
    cout << "example double scalar\n";
    double value;
    try {
        cout << "short way\n";
        value =  pva->channel("DBRdouble00","ca",5.0)->get()->getData()->getDouble();
        cout << "as double " << value << endl;
    } catch (std::runtime_error e) {
        cout << "exception " << e.what() << endl;
    }
    cout << "long way\n";
    PvaClientChannelPtr pvaChannel = pva->createChannel("DBRdouble00","ca");
    pvaChannel->issueConnect();
    Status status = pvaChannel->waitConnect(2.0);
    if(!status.isOK()) {cout << " connect failed\n"; return;}
    PvaClientGetPtr pvaGet = pvaChannel->createGet();
    pvaGet->issueConnect();
    status = pvaGet->waitConnect();
    if(!status.isOK()) {cout << " createGet failed\n"; return;}
    PvaClientGetDataPtr pvaData = pvaGet->getData();
    value = pvaData->getDouble();
    cout << "as double " << value << endl;
}

static void exampleCADoubleArray(PvaClientPtr const &pva)
{
    cout << "example double array\n";
    shared_vector<const double> value;
    try {
        cout << "short way\n";
        value =  pva->channel("DBRdoubleArray","ca",5.0)->get()->getData()->getDoubleArray();
        cout << "as doubleArray " << value << endl;
    } catch (std::runtime_error e) {
        cout << "exception " << e.what() << endl;
    }
    try {
        cout << "long way\n";
        PvaClientChannelPtr pvaChannel = pva->createChannel("DBRdoubleArray","ca");
        pvaChannel->connect(2.0);
        PvaClientGetPtr pvaGet = pvaChannel->createGet();
        PvaClientGetDataPtr pvaData = pvaGet->getData();
        value = pvaData->getDoubleArray();
        cout << "as doubleArray " << value << endl;
    } catch (std::runtime_error e) {
        cout << "exception " << e.what() << endl;
    }
}

int main(int argc,char *argv[])
{
    PvaClientPtr pva= PvaClient::create();
    exampleDouble(pva);
    exampleDoubleArray(pva);
    examplePowerSupply(pva);
    exampleCADouble(pva);
    exampleCADoubleArray(pva);
    cout << "done\n";
    return 0;
}
