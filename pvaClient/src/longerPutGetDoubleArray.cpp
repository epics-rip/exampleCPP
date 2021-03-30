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
   PvaClientPtr pvaClient(PvaClient::get("pva"));
   PvaClientChannelPtr pvaClientChannel(pvaClient->createChannel(channelName));
   pvaClientChannel->issueConnect();
   double value = 0;
   while(true) {
       cout << "Type exit to stop: \n";
       int c = std::cin.peek();  // peek character
       if ( c == EOF ) continue;
       string str;
       getline(cin,str);
       if(str.compare("exit")==0) break;
       
       Status status = pvaClientChannel->waitConnect();
       if(!status.isOK()) {
           cerr << status.getMessage() << "\n";
           continue;
       }
       PvaClientGetPtr pvaClientGet(pvaClientChannel->createGet());
       pvaClientGet->issueConnect();
       status = pvaClientGet->waitConnect();
       if(!status.isOK()) {
           cerr << status.getMessage() << "\n";
           continue;
       }
       pvaClientGet->issueGet();
       status = pvaClientGet->waitGet();
       if(!status.isOK()) {
           cerr << status.getMessage() << "\n";
           continue;
       }
       PvaClientGetDataPtr getData(pvaClientGet->getData());
       PvaClientPutPtr pvaClientPut(pvaClientChannel->createPut());
       pvaClientPut->issueConnect();
       status = pvaClientPut->waitConnect();
       if(!status.isOK()) {
           cerr << status.getMessage() << "\n";
           continue;
       }
       pvaClientPut->issuePut();
       status = pvaClientPut->waitPut();
       if(!status.isOK()) {
           cerr << status.getMessage() << "\n";
           continue;
       }
       PvaClientPutDataPtr putData(pvaClientPut->getData());
       PVStructurePtr pvStructure(putData->getPVStructure());
       PVScalarArrayPtr pvValue(pvStructure->getSubField<PVScalarArray>("value"));
       if(!pvValue) {
          cerr << "value is not a scalarArray\n";
          continue;
       }
       ScalarType scalarType = pvValue->getScalarArray()->getElementType();
       if(!ScalarTypeFunc::isNumeric(scalarType)) {
            cerr << "value is not a numeric scalarArray\n";
          continue;
       }
       shared_vector<const double> getvalue = getData->getDoubleArray();
       cout << getvalue << "\n";
       size_t num = 5;
       shared_vector<double> data(num,0);
       for(size_t i=0; i<num; ++i) data[i] = value + i;
       putData->putDoubleArray(freeze(data));
       pvaClientPut->put();
       value = value + 1;
    }
    return 0;
}
