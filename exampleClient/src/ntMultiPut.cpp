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
#include <pv/pvaClientMultiChannel.h>
#include <pv/convert.h>

using std::tr1::static_pointer_cast;
using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

static void setValue(PVUnionPtr const &pvUnion, string value)
{
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    ConvertPtr convert = getConvert();
    UnionConstPtr u = pvUnion->getUnion();
    FieldConstPtr field = u->getField(0);
    Type type = field->getType();
    if(type==scalar) {
         ScalarConstPtr scalar = static_pointer_cast<const Scalar>(field);
         PVScalarPtr pvScalar(pvDataCreate->createPVScalar(scalar));
         convert->fromString(pvScalar,value);
         pvUnion->set(0,pvScalar);
         return;
    }
    if(type==scalarArray) {
        ScalarArrayConstPtr scalarArray = static_pointer_cast<const ScalarArray>(field);
         PVScalarArrayPtr pvScalarArray(pvDataCreate->createPVScalarArray(scalarArray));
         convert->fromString(pvScalarArray,value);
         pvUnion->set(0,pvScalarArray);
         return;
    }
    if(type==union_) {
       PVUnionPtr pvu(pvDataCreate->createPVVariantUnion());
       PVStringPtr pvString = pvDataCreate->createPVScalar<PVString>();
       pvString->put(value);
       pvu->set(pvString);
       pvUnion->set(0,pvu);
       return;
    }
    throw std::runtime_error("only scalar, scalarArray, and union are supported");
}

class MyPut;
typedef std::tr1::shared_ptr<MyPut> MyPutPtr;

class MyPut :
    public std::tr1::enable_shared_from_this<MyPut>
{
private:
    PvaClientMultiChannelPtr multiChannel;
    PvaClientNTMultiPutPtr multiPut;
    epics::pvData::shared_vector<const std::string> const channelNames;

    MyPut(
       shared_vector<const string> const &channelNames
     )
    : 
      channelNames(channelNames)
    {}

    void init(PvaClientPtr const &pva,string const & provider)
    {
        multiChannel = PvaClientMultiChannel::create(pva,channelNames,provider);
        Status status = multiChannel->connect();     
        if(!status.isSuccess()) {
             cout << "Did not connect: ";
             shared_vector<epics::pvData::boolean> isConnected = multiChannel->getIsConnected();
             size_t num = channelNames.size();
             for(size_t i=0; i<num; ++i) {
                 if(!isConnected[i]) cout << channelNames[i] << " ";
             }
             cout << endl;
        }
        multiPut = multiChannel->createNTPut();
    }
public:
    static MyPutPtr create(
        PvaClientPtr const &pva,
        string const & provider,
        shared_vector<const string> const &channelNames
        )
    {
        
        MyPutPtr clientPut(MyPutPtr(new MyPut(channelNames)));
        clientPut->init(pva,provider);
        return clientPut;
    }
    void put(string value)
    {
        shared_vector<epics::pvData::PVUnionPtr> data = multiPut->getValues();
        shared_vector<epics::pvData::boolean> isConnected = multiChannel->getIsConnected();
        for(size_t i=0; i<channelNames.size(); ++i)
        {
           if(isConnected[i])
           {
                PVUnionPtr pvUnion = data[i];
                setValue(pvUnion,value);
            }
        }
        multiPut->put();
    }
};


int main(int argc,char *argv[])
{
    string provider("pva");
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
                  << " " <<  channelNames
                  << endl;           
                return 0;
            case 'd' :
               if(string(optarg)=="true") debug = true;
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
    cout << "_____ntMultiPut starting_______\n";
    if(debug) PvaClient::setDebug(true);
    try {
        int nPvs = argc - optind;       /* Remaining arg list are PV names */
        if (nPvs!=0)
        {
            channelNames.clear();
            while(optind < argc) {
                channelNames.push_back(argv[optind]);
                optind++;
            }
        }
        cout << " channelNames " <<  channelNames << endl;
        PvaClientPtr pva= PvaClient::get(provider);
        shared_vector<const string> names(freeze(channelNames));
        MyPutPtr clientPut(MyPut::create(pva,provider,names));
        while(true) {
            cout << "Type exit or enter to put\n";
            string str;
            getline(cin,str);
            if(str.compare("exit")==0) break;
            cout  << "enter value\n";
            getline(cin,str);
            clientPut->put(str);
        }
        cout << "___ntMultiPut done_______\n";
    } catch (std::exception& e) {
        cout << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
