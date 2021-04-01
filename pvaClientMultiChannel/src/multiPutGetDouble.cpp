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
    channelNames.push_back("PVRshort");
    channelNames.push_back("PVRint");
    channelNames.push_back("PVRlong");
    channelNames.push_back("PVRubyte");
    channelNames.push_back("PVRushort");
    channelNames.push_back("PVRuint");
    channelNames.push_back("PVRulong");
    channelNames.push_back("PVRfloat");
    channelNames.push_back("PVRdouble");
    shared_vector<const string> names(freeze(channelNames));
    PvaClientPtr pvaClient = PvaClient::get("pva");
    PvaClientMultiChannelPtr multiChannel = 
       PvaClientMultiChannel::create(pvaClient,names);
    multiChannel->connect();
    PvaClientMultiGetDoublePtr multiGetDouble =
       multiChannel->createGet();
    PvaClientMultiPutDoublePtr multiPutDouble =
       multiChannel->createPut();
    double value = 0;
    while(true) {
       cout << "Type exit to stop: \n";
       int c = std::cin.peek();  // peek character
       if ( c == EOF ) continue;
       string str;
       getline(cin,str);
       if(str.compare("exit")==0) break;
       epics::pvData::shared_vector<double> getvalue(multiGetDouble->get());
       cout << getvalue << "\n";
       int num = names.size();
       shared_vector<double> data(num,0);
       for(int i=0; i<num; ++i) data[i] = value + i;
       multiPutDouble->put(data);
       value = value + 1;
    }
    return 0;
}
