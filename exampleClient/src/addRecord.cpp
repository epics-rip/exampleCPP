/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */
#include <iostream>
#include <epicsGetopt.h>
#include <pv/pvaClient.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;


int main(int argc,char *argv[])
{
    if(argc!=4) {
        cerr << "must specify: addRecord protoRecord newRecord\n";
        return 1;
    }
    string addRecord(argv[1]);
    string protoRecord(argv[2]);
    string newRecord(argv[3]);
    PvaClientPtr pva= PvaClient::get("pva");
    try {
        PvaClientChannelPtr pvaProto = pva->createChannel(protoRecord);
        pvaProto->connect();
        PvaClientGetPtr protoGet = pvaProto->get("");
        protoGet->get();
        StructureConstPtr protoStructure = protoGet->getData()->getStructure();
        PvaClientChannelPtr pvaAdd = pva->createChannel(addRecord);
        pvaAdd->connect();
        PvaClientPutGetPtr addPutGet = pvaAdd->createPutGet();
        addPutGet->getPut();
        PvaClientPutDataPtr putData = addPutGet->getPutData();
        PVStructurePtr addPVStructure = putData->getPVStructure(); 
        PVStringPtr pvName = addPVStructure->getSubField<PVString>("argument.recordName");
        if(!pvName) {
             cout << "argument.recordName not found\n";
             return 1;
        }
        pvName->put(newRecord);
        PVUnionPtr pvUnion = addPVStructure->getSubField<PVUnion>("argument.union");
        if(!pvUnion) {
             cout << "argument.union not found\n";
             return 1;
        }
        PVStructurePtr pvStruct = getPVDataCreate()->createPVStructure(protoStructure);
        pvUnion->set(pvStruct);
        putData->getChangedBitSet()->set(pvUnion->getFieldOffset());
        addPutGet->putGet();
        cout << "result=" << addPutGet->getGetData()->getPVStructure() << "\n";
        
    } catch (std::exception& e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
