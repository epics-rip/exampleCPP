// Copyright information and license terms for this software can be
// found in the file LICENSE that is included with the distribution

/*getForever.cpp */

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
    cout << "_____getForever starting_______\n";
    try {
        PvaClientPtr pva= PvaClient::get("pva");
        
        while(true) {
            double value =  pva->channel("PVRdouble")->get()->getData()->getDouble();
            cout << "value " << value << endl;
            int c = std::cin.peek();  // peek character
            if ( c == EOF ) continue;
            cout << "Type exit to stop: \n";
            string str;
            getline(cin,str);
            if(str.compare("exit")==0) break;
        }
    } catch (std::runtime_error e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
