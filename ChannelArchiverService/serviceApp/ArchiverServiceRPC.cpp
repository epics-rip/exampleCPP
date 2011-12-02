/* ArchiverServiceRPC.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvDataCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/* Marty Kraimer 2011.03 */
/* This connects to a V3 record and presents the data as a PVStructure
 * It provides access to  value, alarm, display, and control.
 */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <stdexcept>
#include <memory>
#include <iostream>
#include <vector>

#include <epicsVersion.h>
#include <alarm.h>
#include <epicsMath.h> // math.h + isinf + isnan
// Tools
#include <AutoPtr.h>
#include <MsgLogger.h>
#include <RegularExpression.h>
#include <BinaryTree.h>
#include <AutoIndex.h>
// Storage
#include <SpreadsheetReader.h>
#include <LinearReader.h>
#include <PlotReader.h>

#include <ArchiverServiceRPC.h>

using namespace epics::pvData;
using namespace epics::pvAccess;

template<typename T> T * copyToArray(std::vector<T> & fields)
{
    T * fields2 = new T[fields.size()];
    std::copy(fields.begin(), fields.end(), fields2);
    return fields2;
}

StructureConstPtr ArchiverStructure(FieldCreate & factory)
{
    std::vector<FieldConstPtr> fields;
    fields.push_back(factory.createScalarArray("values", epics::pvData::pvDouble));
    return factory.createStructure(
        "TestStructure", fields.size(), copyToArray(fields));
}

namespace epics { namespace pvIOC { 

ExampleServiceRPC::ExampleServiceRPC()
{}

ExampleServiceRPC::~ExampleServiceRPC()
{
printf("ExampleServiceRPC::~ExampleServiceRPC()\n");
}

void ExampleServiceRPC::destroy()
{
printf("ExampleServiceRPC::destroy()\n");
}


void ExampleServiceRPC::request(
    ChannelRPCRequester::shared_pointer const & channelRPCRequester,
    epics::pvData::PVStructure::shared_pointer const & pvArgument)
{
    String builder;    
    pvArgument->toString(&builder);
    std::cout << builder << std::endl;

    AutoPtr<Index> index(new AutoIndex());
    
    StructureConstPtr archiverStructure = ArchiverStructure(*getFieldCreate());
    PVStructure::shared_pointer pvResult(getPVDataCreate()->createPVStructure(NULL, archiverStructure));
    
    std::vector<double> values;
    PVValueArray<double> * pvValues = (PVValueArray<double> * )
        pvResult->getScalarArrayField("values", pvDouble);
    
    char * indexName = "/extra2/archdata/11_30/index";
    
    index->open(indexName, true);
    double delta = 1.0;
    const epicsTime start;
    const stdString name = "SR-DI-DCCT-01:SIGNAL";
    AutoPtr<DataReader> reader(ReaderFactory::create(*index, ReaderFactory::Raw, delta));

    const RawValue::Data *data = reader->find(name, &start);

    while(true)
    {
        if(data == 0)
        {
            break;
        }
        double value;
        RawValue::getDouble(reader->getType(), reader->getCount(), data, value, 0);
        values.push_back(value);
        data = reader->next();
    }

    pvValues->put(0, values.size(), &values[0], 0);
    channelRPCRequester->requestDone(Status::OK, pvResult);



}

}}

