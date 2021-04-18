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
#include <pvcontrol/controlRecord.h>
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
    
    std::vector<std::string> recordName;
    std::vector<std::string> valueType;   
    recordName.push_back("PVRboolean"); valueType.push_back("boolean");
    recordName.push_back("PVRbyte"); valueType.push_back("byte"); 
    recordName.push_back("PVRshort"); valueType.push_back("short"); 
    recordName.push_back("PVRint"); valueType.push_back("int"); 
    recordName.push_back("PVRlong"); valueType.push_back("long");
    recordName.push_back("PVRubyte"); valueType.push_back("ubyte"); 
    recordName.push_back("PVRushort"); valueType.push_back("ushort"); 
    recordName.push_back("PVRuint"); valueType.push_back("uint"); 
    recordName.push_back("PVRulong"); valueType.push_back("ulong");
    recordName.push_back("PVRfloat"); valueType.push_back("float"); 
    recordName.push_back("PVRdouble"); valueType.push_back("double"); 
    recordName.push_back("PVRstring"); valueType.push_back("string"); 
    for(size_t i=0;i<recordName.size(); ++i)
    {
        if(!master->addRecord(PvdbcrScalarRecord::create(recordName[i],valueType[i]))) { 
            cerr << "record " << recordName[i] << " not added to master\n";
        }
    }
    
    recordName.clear();
    valueType.clear();
    recordName.push_back("PVRbooleanArray"); valueType.push_back("boolean");
    recordName.push_back("PVRbyteArray"); valueType.push_back("byte"); 
    recordName.push_back("PVRshortArray"); valueType.push_back("short"); 
    recordName.push_back("PVRintArray"); valueType.push_back("int"); 
    recordName.push_back("PVRlongArray"); valueType.push_back("long");
    recordName.push_back("PVRubyteArray"); valueType.push_back("ubyte"); 
    recordName.push_back("PVRushortArray"); valueType.push_back("ushort"); 
    recordName.push_back("PVRuintArray"); valueType.push_back("uint"); 
    recordName.push_back("PVRulongArray"); valueType.push_back("ulong");
    recordName.push_back("PVRfloatArray"); valueType.push_back("float"); 
    recordName.push_back("PVRdoubleArray"); valueType.push_back("double"); 
    recordName.push_back("PVRstringArray"); valueType.push_back("string"); 
    for(size_t i=0;i<recordName.size(); ++i)
    {
        if(!master->addRecord(PvdbcrScalarArrayRecord::create(recordName[i],valueType[i]))) { 
            cerr << "record " << recordName[i] << " not added to master\n";
        }
    }
    
    if(!master->addRecord(PvdbcrAddRecord::create("PVRaddRecord"))) { 
        cerr << "record " << "PVRaddRecord" << " not added to master\n";
    }
    if(!master->addRecord(PvdbcrRemoveRecord::create("PVRremoveRecord"))) { 
        cerr << "record " << "PVRaddRecord" << " not added to master\n";
    }
    if(!master->addRecord(PvdbcrProcessRecord::create("PVRprocessRecord"))) { 
        cerr << "record " << "PVRaddRecord" << " not added to master\n";
    }
    if(!master->addRecord(PvdbcrTraceRecord::create("PVRtraceRecord"))) { 
        cerr << "record " << "PVRaddRecord" << " not added to master\n";
    }

    epics::example::control::ControlRecordPtr controlRecordDouble
       = epics::example::control::ControlRecord::create("PVRcontrolDouble","double");
    master->addRecord(controlRecordDouble);
    epics::example::control::ControlRecordPtr controlRecordUByte
       = epics::example::control::ControlRecord::create("PVRcontrolUByte","ubyte");
    master->addRecord(controlRecordUByte);
        
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

