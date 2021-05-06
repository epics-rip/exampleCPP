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
#include <scalarLimit/scalarLimitRecord.h>
#include <powerSupply/powerSupplyRecord.h>
#include <linkRecord/getLinkScalarRecord.h>
#include <linkRecord/getLinkScalarArrayRecord.h>
#include <linkRecord/putLinkScalarRecord.h>
#include <linkRecord/putLinkScalarArrayRecord.h>
#include <helloPutGet/helloPutGetRecord.h>
#include <helloRPC/helloRPCRecord.h>
#include <pvcontrol/controlRecord.h>
// The following must be the last include for code database uses
#include <epicsExport.h>
#define epicsExportSharedSymbols

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;


int main(int argc,char *argv[])
{
    PVDatabasePtr master = PVDatabase::getMaster();
    ChannelProviderLocalPtr channelProvider = getChannelProviderLocal();
    
    std::vector<std::string> recordNames;
    std::vector<std::string> valueType;   
    recordNames.push_back("PVRboolean"); valueType.push_back("boolean");
    recordNames.push_back("PVRbyte"); valueType.push_back("byte"); 
    recordNames.push_back("PVRshort"); valueType.push_back("short"); 
    recordNames.push_back("PVRint"); valueType.push_back("int"); 
    recordNames.push_back("PVRlong"); valueType.push_back("long");
    recordNames.push_back("PVRubyte"); valueType.push_back("ubyte"); 
    recordNames.push_back("PVRushort"); valueType.push_back("ushort"); 
    recordNames.push_back("PVRuint"); valueType.push_back("uint"); 
    recordNames.push_back("PVRulong"); valueType.push_back("ulong");
    recordNames.push_back("PVRfloat"); valueType.push_back("float"); 
    recordNames.push_back("PVRdouble"); valueType.push_back("double");
    recordNames.push_back("PVRdouble01"); valueType.push_back("double");
    recordNames.push_back("PVRdouble02"); valueType.push_back("double");
    recordNames.push_back("PVRdouble03"); valueType.push_back("double");
    recordNames.push_back("PVRdouble04"); valueType.push_back("double");
    recordNames.push_back("PVRdouble05"); valueType.push_back("double");
    recordNames.push_back("PVRstring"); valueType.push_back("string"); 
    for(size_t i=0;i<recordNames.size(); ++i)
    {
        if(!master->addRecord(PvdbcrScalarRecord::create(recordNames[i],valueType[i]))) { 
            cerr << "record " << recordNames[i] << " not added to master\n";
        }
    }
    
    recordNames.clear();
    valueType.clear();
    recordNames.push_back("PVRbooleanArray"); valueType.push_back("boolean");
    recordNames.push_back("PVRbyteArray"); valueType.push_back("byte"); 
    recordNames.push_back("PVRshortArray"); valueType.push_back("short"); 
    recordNames.push_back("PVRintArray"); valueType.push_back("int"); 
    recordNames.push_back("PVRlongArray"); valueType.push_back("long");
    recordNames.push_back("PVRubyteArray"); valueType.push_back("ubyte"); 
    recordNames.push_back("PVRushortArray"); valueType.push_back("ushort"); 
    recordNames.push_back("PVRuintArray"); valueType.push_back("uint"); 
    recordNames.push_back("PVRulongArray"); valueType.push_back("ulong");
    recordNames.push_back("PVRfloatArray"); valueType.push_back("float"); 
    recordNames.push_back("PVRdoubleArray"); valueType.push_back("double");
    recordNames.push_back("PVRdouble01Array"); valueType.push_back("double");
    recordNames.push_back("PVRdouble02Array"); valueType.push_back("double");
    recordNames.push_back("PVRdouble03Array"); valueType.push_back("double");
    recordNames.push_back("PVRdouble04Array"); valueType.push_back("double");
    recordNames.push_back("PVRdouble05Array"); valueType.push_back("double");
    recordNames.push_back("PVRstringArray"); valueType.push_back("string"); 
    for(size_t i=0;i<recordNames.size(); ++i)
    {
        if(!master->addRecord(PvdbcrScalarArrayRecord::create(recordNames[i],valueType[i]))) { 
            cerr << "record " << recordNames[i] << " not added to master\n";
        }
    }
    
    std::string recordName;
    recordName = "PVRaddRecord";
    if(!master->addRecord(PvdbcrAddRecord::create(recordName))) { 
        cerr << "record " << recordName << " not added to master\n";
    }
    recordName = "PVRremoveRecord";
    if(!master->addRecord(PvdbcrRemoveRecord::create(recordName))) { 
        cerr << "record " << recordName << " not added to master\n";
    }
    recordName = "PVRprocessRecord";
    if(!master->addRecord(PvdbcrProcessRecord::create(recordName))) { 
        cerr << "record " << recordName << " not added to master\n";
    }
    recordName = "PVRtraceRecord";
    if(!master->addRecord(PvdbcrTraceRecord::create(recordName))) { 
        cerr << "record " << recordName << " not added to master\n";
    }

    recordName = "PVRcontrolDouble";
    epics::example::control::ControlRecordPtr controlRecordDouble
       = epics::example::control::ControlRecord::create(recordName,"double");
    if(!master->addRecord(controlRecordDouble)) {
        cerr << "record " << recordName << " not added to master\n";
    }
    recordName = "PVRcontrolUByte";
    epics::example::control::ControlRecordPtr controlRecordUByte
       = epics::example::control::ControlRecord::create(recordName,"ubyte");
    master->addRecord(controlRecordUByte);

    recordName = "PVRpowerSupply";
    epics::example::powerSupply::PowerSupplyRecordPtr powerSupply
       = epics::example::powerSupply::PowerSupplyRecord::create(recordName);
    if(!master->addRecord(powerSupply)) {
        cerr << "record " << recordName << " not added to master\n";
    }

    recordName = "PVRgetLinkScalar";  
    epics::example::linkRecord::GetLinkScalarRecordPtr getLinkScalar
       = epics::example::linkRecord::GetLinkScalarRecord::create(recordName);
    if(!master->addRecord(getLinkScalar)) {
        cerr << "record " << recordName << " not added to master\n";
    }

    recordName = "PVRgetLinkScalarArray";
    epics::example::linkRecord::GetLinkScalarArrayRecordPtr getLinkScalarArray
       = epics::example::linkRecord::GetLinkScalarArrayRecord::create(recordName);
    if(!master->addRecord(getLinkScalarArray)) {
        cerr << "record " << recordName << " not added to master\n";
    }

    recordName = "PVRputLinkScalar";
    epics::example::linkRecord::PutLinkScalarRecordPtr putLinkScalar
       = epics::example::linkRecord::PutLinkScalarRecord::create(recordName);
    if(!master->addRecord(putLinkScalar)) {
        cerr << "record " << recordName << " not added to master\n";
    }

    recordName = "PVRputLinkScalarArray";
    epics::example::linkRecord::PutLinkScalarArrayRecordPtr putLinkScalarArray
       = epics::example::linkRecord::PutLinkScalarArrayRecord::create(recordName);
    if(!master->addRecord(putLinkScalarArray)) {
        cerr << "record " << recordName << " not added to master\n";
    }

    recordName = "PVRhelloPutGet";
    epics::example::helloPutGet::HelloPutGetRecordPtr helloPutGet
       = epics::example::helloPutGet::HelloPutGetRecord::create(recordName);
    if(!master->addRecord(helloPutGet)) { 
        cerr << recordName << " not added to master\n";
    }

    recordName = "PVRhelloRPC";
    epics::example::helloRPC::HelloRPCRecordPtr helloRPC
       = epics::example::helloRPC::HelloRPCRecord::create(recordName);
    if(!master->addRecord(helloRPC)) { 
        cerr << recordName << " not added to master\n";
    }

    recordName = "PVRscalarLimitUbyte";  
    epics::scalarLimit::ScalarLimitRecordCreate::create(recordName,"ubyte");

    recordName = "PVRscalarLimitDouble";  
    epics::scalarLimit::ScalarLimitRecordCreate::create(recordName,"double");

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

