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
#include <pv/pvdbcrProcessRecord.h>

// The following must be the last include for code database uses
#include <epicsExport.h>
#define epicsExportSharedSymbols


using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvDatabase;

namespace epics { namespace exampleCPP { namespace support {
class SupportRecord;
typedef std::tr1::shared_ptr<SupportRecord> SupportRecordPtr;

class epicsShareClass SupportRecord :
    public PVRecord
{
public:
    POINTER_DEFINITIONS(SupportRecord);
    static SupportRecordPtr create(
        std::string const & recordName,epics::pvData::ScalarType scalarType);
    virtual bool init();
    virtual void process();
    ~SupportRecord();
private:
    SupportRecord(
        std::string const & recordName,
        epics::pvData::PVStructurePtr const & pvStructure);
    ControlSupportPtr controlSupport;
    ScalarAlarmSupportPtr scalarAlarmSupport;
    epics::pvData::PVBooleanPtr pvReset;
};

SupportRecord::~SupportRecord() {}

SupportRecordPtr SupportRecord::create(
    std::string const & recordName,epics::pvData::ScalarType scalarType)
{
    FieldCreatePtr fieldCreate = getFieldCreate();
    PVDataCreatePtr pvDataCreate = getPVDataCreate();
    StandardFieldPtr standardField = getStandardField();
    StructureConstPtr  topStructure = fieldCreate->createFieldBuilder()->
        add("value",scalarType) ->
        add("reset",pvBoolean) ->
        add("alarm",standardField->alarm()) ->
        add("timeStamp",standardField->timeStamp()) ->
        add("display",standardField->display()) ->
        add("control",ControlSupport::controlField(scalarType)) ->
        add("scalarAlarm",ScalarAlarmSupport::scalarAlarmField()) ->
        createStructure();
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(topStructure);
    SupportRecordPtr pvRecord(
        new SupportRecord(recordName,pvStructure));
    if(!pvRecord->init()) pvRecord.reset();
    return pvRecord;
}

SupportRecord::SupportRecord(
    std::string const & recordName,
    epics::pvData::PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure)
{
}

bool SupportRecord::init()
{
    initPVRecord();
    PVRecordPtr pvRecord = shared_from_this();
    PVStructurePtr pvStructure(getPVStructure());
    controlSupport = ControlSupport::create(pvRecord);
    bool result = controlSupport->init(
       pvStructure->getSubField("value"),pvStructure->getSubField("control"));
    if(!result) return false;
    scalarAlarmSupport = ScalarAlarmSupport::create(pvRecord);
    result = scalarAlarmSupport->init(
       pvStructure->getSubField("value"),
       pvStructure->getSubField<PVStructure>("alarm"),
       pvStructure->getSubField("scalarAlarm"));
    if(!result) return false;
    pvReset = getPVStructure()->getSubField<PVBoolean>("reset");
    return true;
}

void SupportRecord::process()
{
    bool wasChanged = false;
    if(pvReset->get()==true) {
        pvReset->put(false);
        controlSupport->reset();
        scalarAlarmSupport->reset();
    } else {
        if(controlSupport->process()) wasChanged = true;;
        if(scalarAlarmSupport->process()) wasChanged = true;
    }
    if(wasChanged) PVRecord::process();
}
}}}

int main(int argc,char *argv[])
{
    PVDatabasePtr master = PVDatabase::getMaster();
    ChannelProviderLocalPtr channelProvider = getChannelProviderLocal();
    
    PVRecordPtr record = PvdbcrProcessRecord::create("PVRsupportProcessRecord",.5);        
    master->addRecord(record);
    
    epics::pvData::ScalarType scalarType  = epics::pvData::pvDouble;
    epics::exampleCPP::support::SupportRecordPtr supportRecordDouble
       = epics::exampleCPP::support::SupportRecord::create("PVRsupportDouble",scalarType);
    master->addRecord(supportRecordDouble);
    scalarType = epics::pvData::pvUByte;
    epics::exampleCPP::support::SupportRecordPtr supportRecordUByte
       = epics::exampleCPP::support::SupportRecord::create("PVRsupportUByte",scalarType);
    master->addRecord(supportRecordUByte);

    ServerContext::shared_pointer ctx =
        startPVAServer("local",0,true,true);

    cout << "support\n";
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
