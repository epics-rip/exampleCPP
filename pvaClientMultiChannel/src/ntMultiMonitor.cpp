/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 */


#include <iostream>
#include <epicsGetopt.h>
#include <pv/pvaClientMultiChannel.h>
#include <pv/convert.h>

using std::tr1::static_pointer_cast;
using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

int main(int argc,char *argv[])
{
    shared_vector<string> channelNames;
    channelNames.push_back("PVRbyte");
    channelNames.push_back("PVRshortArray");
    channelNames.push_back("PVRint");
    channelNames.push_back("PVRlongArray");
    channelNames.push_back("PVRubyte");
    channelNames.push_back("PVRushortArray");
    channelNames.push_back("PVRdouble");
    channelNames.push_back("PVRdoubleArray");
    channelNames.push_back("PVRvariantUnion");
    channelNames.push_back("PVRvariantUnionArray");
    shared_vector<const string> names(freeze(channelNames));
    PvaClientPtr pvaClient = PvaClient::get("pva");
    PvaClientMultiChannelPtr multiChannel = 
       PvaClientMultiChannel::create(pvaClient,names);
    multiChannel->connect();
    PvaClientNTMultiMonitorPtr multiMonitor =
       multiChannel->createNTMonitor();
    bool valueOnly(true);
    while(true) {
       cout << "Type exit to stop: \n";
       int c = std::cin.peek();  // peek character
       if ( c == EOF ) continue;
       string str;
       getline(cin,str);
       if(str.compare("exit")==0) break;
       if(!multiMonitor->poll(valueOnly)) {
           cout << "no new data\n";
           continue;
        }
        PvaClientNTMultiDataPtr multiData = multiMonitor->getData();
        PVStructurePtr pvStructure = multiData->getNTMultiChannel()->getPVStructure();
        PVUnionArrayPtr pvUnionArray = static_pointer_cast<PVUnionArray>(
             pvStructure->getSubField("value"));
        shared_vector<const PVUnionPtr> values = pvUnionArray->view();
        shared_vector<boolean> changeFlags(multiData->getChannelChangeFlags());
        shared_vector<boolean> isConnected(multiChannel->getIsConnected());
        for(size_t ind=0; ind < values.size(); ++ind)
        {
            if(changeFlags[ind]) {
                PVUnionPtr pvUnion = values[ind];
                PVFieldPtr pvField = pvUnion->get();
                cout << names[ind] << " = " << pvField << "\n";
            } else  {
                string connected("false");
                if(isConnected[ind]) connected = "true";
                cout << names[ind] << " no monitor event; isConnected=" << connected << "\n";
            }
        }
    }
    return 0;
}

