/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author Sinisa Veseli and Marty Kraimer
 * @date 2021.02
 */

#include <iostream>
#include <time.h>
#include <unistd.h>
#include <epicsGetopt.h>
#include <pv/pvaClientMultiChannel.h>
#include <pv/convert.h>

using std::tr1::static_pointer_cast;
using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaClient;

static PVDataCreatePtr pvDataCreate = getPVDataCreate();
static ConvertPtr convert = getConvert();

static void setValue(PVUnionPtr const &pvUnion, string value)
{
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
    throw std::runtime_error("only scalar is supported");
}

static void example(
     PvaClientPtr const &pva,
     string provider,
     shared_vector<const string> const &channelNames)
{
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
    PvaClientNTMultiPutPtr multiPut(multiChannel->createNTPut());
    shared_vector<epics::pvData::PVUnionPtr> data = multiPut->getValues();
    int successCount = 0;
    int failedCount = 0;
    clock_t startTime = clock();
    int numiter = 10000;
    int value = 0;
    for(int i = 0; i< numiter; i+= 1) {
        bool correctData = true;
        value++;
        if(value>127) value = 0;
        string strValue = to_string(value);
        for(size_t j=0; j<num ; ++j) {
             PVUnionPtr pvUnion = data[j];
             setValue(pvUnion,strValue);
        }
        try {
            multiPut->put();
        } catch (std::exception& e) {
            cout << "i=" << i << " put exception " << e.what() << endl;
            correctData = false;
        }
        try { 
             multiGet->get();
        } catch (std::exception& e) {
            cout << "i=" << i << " get exception " << e.what() << endl;
            correctData = false;
        }
        PvaClientNTMultiDataPtr multiData = multiGet->getData();
        PVStructurePtr received = multiData->getNTMultiChannel()->getPVStructure();
        PVUnionArrayPtr unionArray = received->getSubField<PVUnionArray>("value");
        shared_vector<const PVUnionPtr> pvUnions = unionArray->view();
        for(size_t j=0; j<num ; ++j) {
            PVUnionPtr pvUnion = pvUnions[j];
            PVFieldPtr pvField = pvUnion->get();
            StructureConstPtr unionStructure = 
                getFieldCreate()->createFieldBuilder()->add("value", pvField->getField())->createStructure();
            PVStructurePtr unionPvStructure = getPVDataCreate()->createPVStructure(unionStructure);
            unionPvStructure->getSubField("value")->copy(*pvField);
            PVScalarPtr fieldPtr = unionPvStructure->getSubField<PVScalar>("value");
            string getValue = convert->toString(fieldPtr);
            if(strValue!=getValue){
                 cout << "i=" << i << " channelName=" << channelNames[j]
                     << " expected=" << strValue << " got=" << getValue << "\n";
                 correctData = false;
            } 
        }
        if(correctData) {
            successCount++;
        } else {
            failedCount++;
        }
    }
    double seconds = (double)(clock() - startTime)/CLOCKS_PER_SEC;
    cout << "time=" << seconds << " per interation=" << seconds/numiter << "\n";
    cout << "SUCCESS COUNT: " << successCount << endl;
    cout << "FAILED COUNT: " << failedCount << endl;
}

int main(int argc,char *argv[])
{
    string provider("pva");
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
    int opt;
    while((opt = getopt(argc, argv, "hp:")) != -1) {
        switch(opt) {
            case 'p':
                provider = optarg;
                break;
            case 'h':
             cout << " -h -p provider channelNames " << endl;
             cout << "default" << endl;
             cout << "-p " << provider 
                  << " " <<  channelNames
                  << endl;           
                return 0;
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
    cout << "_____pvaClientNTMultiPutGet starting_______\n";
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
        example(pva,provider,names);
        cout << "_____pvaClientNTMultiPutGet done_______\n";
    } catch (std::exception& e) {
        cout << "exception " << e.what() << endl;
        return 1;
    }
    return 0;
}
