/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 */

#include <iostream>
#include <epicsGetopt.h>
#include <pv/pvaClient.h>


using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

int main(int argc,char *argv[])
{
   string channelName("PVRdoubleArray");
   int nPvs = argc - optind;
   if(nPvs==1) channelName = argv[optind];
   double value = 0;
   while(true) {
       cout << "Type exit to stop: \n";
       int c = std::cin.peek();  // peek character
       if ( c == EOF ) continue;
       string str;
       getline(cin,str);
       if(str.compare("exit")==0) break;
       try {
           shared_vector<const double> getvalue = PvaClient::get("pva")->channel(channelName)->getDoubleArray();
           cout << getvalue << "\n";
           size_t num = 5;
           shared_vector<double> data(num,0);
           for(size_t i=0; i<num; ++i) data[i] = value + i;
           PvaClient::get("pva")->channel(channelName)->putDoubleArray(freeze(data));
           value = value + 1;
       } catch (std::exception& e) {
           cerr << "exception " << e.what() << endl;
       }     
   }
   return 0;
}
