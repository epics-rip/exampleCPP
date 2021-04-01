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
    shared_vector<const string> names(freeze(channelNames));
    PvaClientPtr pvaClient = PvaClient::get("pva");
    PvaClientMultiChannelPtr multiChannel = 
       PvaClientMultiChannel::create(pvaClient,names);
    multiChannel->connect();
    PvaClientNTMultiPutPtr multiPut = multiChannel->createNTPut();
    PvaClientNTMultiGetPtr multiGet = multiChannel->createNTGet();
    bool valueOnly(true);
    while(true) {
       cout << "Type exit to stop: \n";
       int c = std::cin.peek();  // peek character
       if ( c == EOF ) continue;
       string str;
       getline(cin,str);
       if(str.compare("exit")==0) break;
       string value;
       cout  << "enter value\n";
       getline(cin,value);
       multiGet->get(valueOnly);
       PvaClientNTMultiDataPtr multiData = multiGet->getData();
       PVStructurePtr pvStructure = multiData->getNTMultiChannel()->getPVStructure();
       PVUnionArrayPtr pvUnionArray = static_pointer_cast<PVUnionArray>(
             pvStructure->getSubField("value"));
       shared_vector<const PVUnionPtr> values = pvUnionArray->view();
       for(size_t ind=0; ind < values.size(); ++ind)
       {
            PVUnionPtr pvUnion = values[ind];
            if(pvUnion) {
                PVFieldPtr pvField = pvUnion->get();
                cout << names[ind] << " = " << pvField << "\n";
            } else {
                cout << names[ind] << " is null\n";
            }
       }
       shared_vector<epics::pvData::PVUnionPtr> data = multiPut->getValues();
       shared_vector<epics::pvData::boolean> isConnected = multiChannel->getIsConnected();
       for(size_t i=0; i<names.size(); ++i)
       {
           if(isConnected[i])
           {
                PVUnionPtr pvUnion = data[i];
                setValue(pvUnion,value);
            }
       }
       multiPut->put();
    }
    return 0;
}
