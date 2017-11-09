/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.04.02
 */

#include <pv/standardField.h>
#include <pv/ntscalar.h>

#define epicsExportSharedSymbols
#include <pv/exampleSoftRecord.h>


using namespace epics::pvData;
using namespace epics::pvDatabase;
using namespace epics::nt;
using std::tr1::static_pointer_cast;
using std::string;

namespace epics { namespace exampleCPP { namespace database {


ExampleSoftRecordPtr ExampleSoftRecord::create(
    string const & recordName)
{
    NTScalarBuilderPtr ntScalarBuilder = NTScalar::createBuilder();
    PVStructurePtr pvStructure = ntScalarBuilder->
        value(pvDouble)->
        addAlarm()->
        addTimeStamp()->
        addDisplay()->
        addControl()->
        createPVStructure();
    ExampleSoftRecordPtr pvRecord(
        new ExampleSoftRecord(recordName,pvStructure));
    pvRecord->initPVRecord();
    return pvRecord;
}

ExampleSoftRecord::ExampleSoftRecord(
    string const & recordName,
    PVStructurePtr const & pvStructure)
: PVRecord(recordName,pvStructure)
{
    PVFieldPtr pvField;
}

void ExampleSoftRecord::process()
{

}

}}}
