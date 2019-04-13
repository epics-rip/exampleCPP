/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <iostream>

#include <pv/pvaClientMultiChannel.h>
#include <pv/convert.h>

using std::tr1::static_pointer_cast;
using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;


static void example(
     PvaClientPtr const &pva,
     string provider,
     shared_vector<const string> const &channelNames)
{
    cout << "_example provider " << provider << " channels " << channelNames << "_\n";
    size_t num = channelNames.size();
    PvaClientMultiChannelPtr multiChannel(
        PvaClientMultiChannel::create(pva,channelNames,provider));
    Status status = multiChannel->connect();
    if(!status.isSuccess()) {
         cout << "Did not connect: ";
         shared_vector<epics::pvData::boolean> isConnected = multiChannel->getIsConnected();
         for(size_t i=0; i<num; ++i) {
             if(!isConnected[i]) cout << channelNames[i] << " ";
         }
         cout << endl;
         return;
    }
    PvaClientNTMultiGetPtr multiGet(multiChannel->createNTGet());
    multiGet->get();
    PvaClientNTMultiDataPtr multiData = multiGet->getData();
    PVStructurePtr pvStructure = multiData->getNTMultiChannel()->getPVStructure();
    PVUnionArrayPtr pvUnionArray = static_pointer_cast<PVUnionArray>(pvStructure->getSubField("value"));
    shared_vector<const PVUnionPtr> values = pvUnionArray->view();
    for(size_t ind=0; ind < values.size(); ++ind)
    {
        PVUnionPtr pvUnion = values[ind];
        PVFieldPtr pvField = pvUnion->get();
        cout << channelNames[ind] << " = " << pvField << "\n";
    }
}

int main(int argc,char *argv[])
{
    cout << "_____examplePvaClientNTMultiGet starting_______\n";
    try {
        PvaClientPtr pva = PvaClient::get("pva ca");
        size_t num = 4;
        shared_vector<string> channelNames(num);
        channelNames[0] = "PVRdouble";
        channelNames[1] = "PVRstring";
        channelNames[2] = "PVRdoubleArray";
        channelNames[3] = "PVRstringArray";
        shared_vector<const string> names(freeze(channelNames));
        example(pva,"pva",names);
        cout << "_____examplePvaClientNTMultiGet done_______\n";
    } catch (std::exception& e) {
        cout << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
