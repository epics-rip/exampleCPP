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
#include <pv/convert.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

class ClientGetField;
typedef std::tr1::shared_ptr<ClientGetField> ClientGetFieldPtr;

class ClientGetField :
    public GetFieldRequester,
    public std::tr1::enable_shared_from_this<ClientGetField>
{
private:
    PvaClientChannelPtr pvaClientChannel;
public:
    POINTER_DEFINITIONS(ClientGetField);
    ClientGetField(
        const PvaClientChannelPtr & pvaClientChannel)
    : pvaClientChannel(pvaClientChannel)
    {
    }
    
    static ClientGetFieldPtr create(
        PvaClientPtr const &pvaClient,
        const string & channelName,
        const string & providerName)
    {
        PvaClientChannelPtr pvaClientChannel = pvaClient->createChannel(channelName,providerName);
        ClientGetFieldPtr clientGetField(new ClientGetField(pvaClientChannel));
        pvaClientChannel->issueConnect();
        return clientGetField;
    }

    virtual void getDone(
        const epics::pvData::Status& status,
        epics::pvData::FieldConstPtr const & field)
    {
        cout << "getDone\n";
        if(status.isOK()) {
            cout << field << endl;
            return;
        }
        cout << " status " << status.getMessage() << endl;
    }

    virtual std::string getRequesterName()
    {
        return "ClientGetField";
    }

    void getField(const string & subField)
    {
         if(!pvaClientChannel->getChannel()->isConnected())
         {
              cout << "channel not connected\n";
              return;
         }
         pvaClientChannel->getChannel()->getField(shared_from_this(),subField);
    }
};


int main(int argc,char *argv[])
{
    string provider("pva");
    string channelName("PVRdouble");
    string debugString;
    bool debug(false);
    int opt;
    while((opt = getopt(argc, argv, "hp:d:")) != -1) {
        switch(opt) {
            case 'p':
                provider = optarg;
                break;
            case 'h':
             cout << " -h -p provider - d debug channelNames " << endl;
             cout << "default" << endl;
             cout << "-p " << provider 
                  << " -d " << (debug ? "true" : "false") 
                  << " " <<  channelName
                  << endl;           
                return 0;
            case 'd' :
               debugString =  optarg;
               if(debugString=="true") debug = true;
               break;
            default:
                std::cerr<<"Unknown argument: "<<opt<<"\n";
                return -1;
        }
    }
    bool pvaSrv(((provider.find("pva")==string::npos) ? false : true));
    bool caSrv(((provider.find("ca")==string::npos) ? false : true));
    if(pvaSrv&&caSrv) {
        cerr<< "multiple providers are not allowed\n";
        return 1;
    }
    cout << "provider " << provider
         << " channelName " <<  channelName
         << " debug " << (debug ? "true" : "false") << endl;

    cout << "_____getField starting__\n";
    
    try {   
        if(debug) PvaClient::setDebug(true);
        vector<string> channelNames;
        vector<ClientGetFieldPtr> clientGetFields;
        int nPvs = argc - optind;       /* Remaining arg list are PV names */
        if (nPvs==0)
        {
            channelNames.push_back(channelName);
            nPvs = 1;
        } else {
            for (int n = 0; optind < argc; n++, optind++) channelNames.push_back(argv[optind]);
        }
        PvaClientPtr pva= PvaClient::get(provider);
        for(int i=0; i<nPvs; ++i) {
            clientGetFields.push_back(ClientGetField::create(pva,channelNames[i],provider));
        }
        while(true) {
            cout << "Type exit, or a fieldname, or just enter return\n";
            int c = std::cin.peek();  // peek character
            if ( c == EOF ) continue;
            string str;
            getline(cin,str);
            if(str.compare("exit")==0) break;
            for(int i=0; i<nPvs; ++i) {
                try {
                    clientGetFields[i]->getField(str);
                } catch (std::exception& e) {
                   cerr << "exception " << e.what() << endl;
                }
            }
        }
    } catch (std::exception& e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
