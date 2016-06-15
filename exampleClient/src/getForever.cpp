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
    string channelName("DBRdouble00");
    bool debug(false);
    if(argc==2 && string(argv[1])==string("-help")) {
        cout << "provider channelName  debug" << endl;
        cout << "default" << endl;
        cout << provider << " " <<  channelName << " "
             << (debug ? "true" : "false") << endl;
        return 0;
    }
    if(argc>1) provider = argv[1];
    if(argc>2) channelName = argv[2];
    if(argc>3) {
        string value(argv[3]);
        if(value=="true") debug = true;
    }
    bool pvaSrv(((provider.find("pva")==string::npos) ? false : true));
    bool caSrv(((provider.find("ca")==string::npos) ? false : true));
    if(pvaSrv&&caSrv) {
        cerr<< "multiple providers are not allowed\n";
        return 1;
    }
    cout << "provider \"" << provider << "\""
         << " pvaSrv " << (pvaSrv ? "true" : "false")
         << " caSrv " << (caSrv ? "true" : "false")
         << " channelName " <<  channelName
         << " debug " << (debug ? "true" : "false") << endl;

    cout << "_____getForever starting__ "
         << " provider " << provider 
         << " channelName " << channelName 
         << endl;
    try {
        PvaClientPtr pva= PvaClient::get(provider);
        if(debug) PvaClient::setDebug(true);
        PvaClientChannelPtr channel(pva->channel(channelName,provider));
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
