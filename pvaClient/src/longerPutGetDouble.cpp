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
   string channelName("PVRdouble");
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
       PVScalarPtr pvValue(pvStructure->getSubField<PVScalar>("value"));
       if(!pvValue) {
          cerr << "value is not a scalar\n";
          continue;
       }
       ScalarType scalarType = pvValue->getScalar()->getScalarType();
       if(!ScalarTypeFunc::isNumeric(scalarType)) {
            cerr << "value is not a numeric scalar\n";
          continue;
       }
       try {
          double getvalue = getData->getDouble();
          cout << getvalue << "\n";
          putData->putDouble(value);
          pvaClientPut->put();
          value = value + 1;
       } catch (std::exception& e) {
           cerr << "exception " << e.what() << endl;
       }
    }
    return 0;
}
