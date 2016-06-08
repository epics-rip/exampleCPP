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
    string provider("pva");
    string recordName("PVRdouble");
    if(argc==2 && string(argv[1])==string("-help")) {
        cout << "provider recordName" << endl;
        cout << "default" << endl;
        cout << provider << " " <<  recordName  << endl;
        return 0;
    }
    if(argc>1) provider = argv[1];
    if(argc>2) recordName = argv[2];
    cout << "_____getForever starting__ "
         << " provider " << provider 
         << " recordName " << recordName 
         << endl;
    try {
        PvaClientPtr pva= PvaClient::get(provider);
        PvaClientChannelPtr channel(pva->channel(recordName,provider));
        while(true) {
            double value = channel->get()->getData()->getDouble();
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
