/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <memory>
#include <vector>
#include <iostream>

#include <pv/standardField.h>
#include <pv/standardPVField.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>
#include <pv/alarm.h>
#include <pv/pvAlarm.h>
#include <pv/pvDatabase.h>
#include <pv/pvaClient.h>
#include <pv/controlSupport.h>
#include <pv/scalarAlarmSupport.h>

#include <pv/pvAccess.h>
#include <pv/serverContext.h>
#include <pv/rpcService.h>
#include <pv/channelProviderLocal.h>
#include <pv/serverContext.h>
#include <pv/pvdbcrScalarRecord.h>
#include <pv/pvdbcrScalarArrayRecord.h>
#include <pv/pvdbcrAddRecord.h>
#include <pv/pvdbcrRemoveRecord.h>
#include <pv/pvdbcrProcessRecord.h>
#include <pv/pvdbcrTraceRecord.h>
#include <pvsupport/supportRecord.h>
#include <powerSupply/powerSupplyRecord.h>
#include <linkRecord/getLinkScalarRecord.h>
#include <linkRecord/getLinkScalarArrayRecord.h>
#include <linkRecord/putLinkScalarRecord.h>
#include <linkRecord/putLinkScalarArrayRecord.h>
// The following must be the last include for code database uses
#include <epicsExport.h>
#define epicsExportSharedSymbols
//#include "linkservice"

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;


int main(int argc,char *argv[])
{
    PVDatabasePtr master = PVDatabase::getMaster();
    ChannelProviderLocalPtr channelProvider = getChannelProviderLocal();
 
    string recordName("PVRdouble");
    PVRecordPtr scalar = PvdbcrScalarRecord::create(recordName,"double");
    bool result = master->addRecord(scalar);      
    if(!result) {
         cerr << "record " << recordName << " not added to master\n";
    }

    recordName = string("PVRdoubleArray");
    PVRecordPtr doubleArray = PvdbcrScalarArrayRecord::create(recordName,"double");
    result = master->addRecord(doubleArray);      
    if(!result) {
         cerr << "record " << recordName << " not added to master\n";
    }

    recordName = string("PVRaddRecord");
    PVRecordPtr addRecord = PvdbcrAddRecord::create(recordName);
    result = master->addRecord(addRecord);      
    if(!result) {
         cerr << "record " << recordName << " not added to master\n";
    }
    
    
    recordName = string("PVRremoveRecord");
    PVRecordPtr removeRecord = PvdbcrRemoveRecord::create(recordName);
    result = master->addRecord(removeRecord);      
    if(!result) {
         cerr << "record " << recordName << " not added to master\n";
    }
    
    
    recordName = string("PVRprocessRecord");
    PvdbcrProcessRecordPtr processRecord = PvdbcrProcessRecord::create(recordName);
    result = master->addRecord(processRecord);      
    if(!result) {
         cerr << "record " << recordName << " not added to master\n";
    }
    
    recordName = string("PVRtraceRecord");
    PVRecordPtr traceRecord = PvdbcrTraceRecord::create(recordName);
    result = master->addRecord(traceRecord);      
    if(!result) {
         cerr << "record " << recordName << " not added to master\n";
    }
    
    epics::example::support::SupportRecordPtr supportRecordDouble
       = epics::example::support::SupportRecord::create("PVRsupportDouble","double");
    master->addRecord(supportRecordDouble);
    epics::example::support::SupportRecordPtr supportRecordUByte
       = epics::example::support::SupportRecord::create("PVRsupportUByte","ubyte");
    master->addRecord(supportRecordUByte);
        
    epics::example::powerSupply::PowerSupplyRecordPtr powerSupply
       = epics::example::powerSupply::PowerSupplyRecord::create("PVRpowerSupply");
    master->addRecord(powerSupply);
         
    epics::example::linkRecord::GetLinkScalarRecordPtr getLinkScalar
       = epics::example::linkRecord::GetLinkScalarRecord::create("PVRgetLinkScalar");
    master->addRecord(getLinkScalar);
         
    epics::example::linkRecord::GetLinkScalarArrayRecordPtr getLinkScalarArray
       = epics::example::linkRecord::GetLinkScalarArrayRecord::create("PVRgetLinkScalarArray");
    master->addRecord(getLinkScalarArray);

    epics::example::linkRecord::PutLinkScalarRecordPtr putLinkScalar
       = epics::example::linkRecord::PutLinkScalarRecord::create("PVRputLinkScalar");
    master->addRecord(putLinkScalar);
         
    epics::example::linkRecord::PutLinkScalarArrayRecordPtr putLinkScalarArray
       = epics::example::linkRecord::PutLinkScalarArrayRecord::create("PVRputLinkScalarArray");
    master->addRecord(putLinkScalarArray);    
   

    ServerContext::shared_pointer ctx =
        startPVAServer("local",0,true,true);

    string str;
    while(true) {
        cout << "enter: pvdbl or exit \n";
        getline(cin,str);
        if(str.compare("exit")==0) break;
        if(str.compare("pvdbl")==0) {
            PVStringArrayPtr pvNames = master->getRecordNames();
            PVStringArray::const_svector xxx = pvNames->view();
            for(size_t i=0; i<xxx.size(); ++i) cout<< xxx[i] << endl;
        }
    }
    return 0;
}

