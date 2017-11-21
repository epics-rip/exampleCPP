/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */
#include <iostream>

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
    string providerName("pva");
    string channelName("PVRdouble");
    bool debug(false);
    if(argc==2 && string(argv[1])==string("-help")) {
        cout << "providerName channelName request debug" << endl;
        cout << "default" << endl;
        cout << providerName << " " <<  channelName
             << " " << (debug ? "true" : "false") << endl;
        return 0;
    }
    if(argc>1) providerName = argv[1];
    if(argc>2) channelName = argv[2];
    if(argc>3) {
        string value(argv[3]);
        if(value=="true") debug = true;
    }
    cout << "providerName " << providerName
         << " debug " << (debug ? "true" : "false") << endl;

    cout << "_____getFieldNoBlock starting__\n";
    
    try {   
        if(debug) PvaClient::setDebug(true);
        ConvertPtr convert = getConvert();
        PvaClientPtr pva= PvaClient::get(providerName);
        ClientGetFieldPtr clientGetField(ClientGetField::create(pva,channelName,providerName));
        while(true) {
            cout << "Type exit, or a fieldname, or just enter return\n";
            int c = std::cin.peek();  // peek character
            if ( c == EOF ) continue;
            string str;
            getline(cin,str);
            if(str.compare("exit")==0) break;
            clientGetField->getField(str);
        }
    } catch (std::runtime_error e) {
        cerr << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
