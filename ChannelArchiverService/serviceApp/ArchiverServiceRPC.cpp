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
#include <float.h>

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

/*
structure NTStatistic
  typeIdentifier_t typeIdentifier
alarm_t alarm              // May be worst alarm from the N samples, or alarm of the aggregation. Which is not defined.
timeStamp_t timeStamp      // Nominally, the time of aggregation.
  double firstValue
  timeStamp_t firstValueTimeStamp
  double lastValue
  timeStamp_t lastValueTimeStamp
  double highValue
  double lowValue
  double meanValue  
  double stdDev
  long N
*/

const char * schema = \
    "structure ArchiverValues\n"                \
    "    string index\n"                        \
    "    string[] names\n"                      \
    "    structure t0\n"                        \
    "        long secPastEpoch\n"               \
    "        int nsec\n"                        \
    "    structure t1\n"                        \
    "        long secPastEpoch\n"               \
    "        int nsec\n"                        \
    "    long how\n"                            \
    "    long count";

using namespace epics::pvData;
using namespace epics::pvAccess;

#include "types.hpp"

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

/**
 * get left edge of bucket aligned to this time delta
 */
epicsTimeStamp calcBucketLeft(epicsTimeStamp t, int delta)
{
    epicsTimeStamp leftEdge;
    leftEdge.nsec = 0;
    leftEdge.secPastEpoch = (t.secPastEpoch / delta) * delta;
    return leftEdge;
}

void ExampleServiceRPC::request(
    ChannelRPCRequester::shared_pointer const & channelRPCRequester,
    epics::pvData::PVStructure::shared_pointer const & pvArgument)
{
    
    StructureConstPtr archiverStructure = ArchiverStructure(*getFieldCreate());
    PVStructure::shared_pointer pvResult(getPVDataCreate()->createPVStructure(NULL, archiverStructure));
    
    String builder;    
    pvArgument->toString(&builder);
    std::cout << builder << std::endl;
    
    // type checking (no pattern matching yet)
    std::string typeString;
    pvArgument->getStructure()->toString(&typeString);

    if(typeString != schema)
    {
        std::cout << "Type checking, wanted:" << std::endl << typeString << std::endl;
        std::cout << "Type checking, got:" << std::endl << schema << std::endl;
        channelRPCRequester->requestDone(Status(Status::STATUSTYPE_ERROR, "type mismatch"), pvResult);
    }
    
    // here's where an interface compiler would be useful...
    PVValueArray<std::string> * pvNames = (PVValueArray<std::string> *)
        pvArgument->getScalarArrayField("names", pvString);
    std::string indexName = pvArgument->getStringField("index")->get();
    epicsTimeStamp t0;
    epicsTimeStamp t1;
    t0.secPastEpoch = pvArgument->getStructureField("t0")->getLongField("secPastEpoch")->get();
    t0.nsec = pvArgument->getStructureField("t0")->getIntField("nsec")->get();
    t1.secPastEpoch = pvArgument->getStructureField("t1")->getLongField("secPastEpoch")->get();
    t1.nsec = pvArgument->getStructureField("t1")->getIntField("nsec")->get();
    int count = pvArgument->getLongField("count")->get();
    
    // UNPACK

    StringArrayData namesData;
    pvNames->get(0, pvNames->getLength(), &namesData);

    AutoPtr<Index> index(new AutoIndex());
    
    std::vector<double> values;
    std::vector<int64_t> secPastEpoch;
    std::vector<int> nsec;

    PVValueArray<double> * pvValues = (PVValueArray<double> * )
        pvResult->getScalarArrayField("values", pvDouble);
    
    PVValueArray<int64_t> * pvSecPastEpoch = (PVValueArray<int64_t> * )
        pvResult->getScalarArrayField("secPastEpoch", pvLong);

    PVValueArray<int> * pvNsec = (PVValueArray<int> * )
        pvResult->getScalarArrayField("nsec", pvInt);

    index->open(indexName.c_str(), true);

    const epicsTime start = t0;

    AutoPtr<DataReader> reader(new RawDataReader(*index));
    
    for(int n = 0; n < pvNames->getLength(); n++)
    {
        printf("name = %s\n", namesData.data[n].c_str());

        const stdString name = namesData.data[n].c_str();

        // QUERY
        const RawValue::Data *data = reader->find(name, &start);

        double minValue = DBL_MAX;
        double maxValue = DBL_MIN;
        double sum;
        int N = 0;
        int first = 1;

        // start of aligned buckets
        epicsTime bucketLeft;
        epicsTime bucketRight;
        int delta = 5;
        
        for(int c = 0; c < count; c++)
        {
            if(data == 0)
            {
                break;
            }
            double value;
            RawValue::getDouble(reader->getType(), reader->getCount(), data, value, 0);
            epicsTimeStamp t = RawValue::getTime(data);

            /*
            if(first)
            {
                bucketLeft = calcBucketLeft(t, delta);
                bucketRight = bucketLeft + delta;
            }

            if(t < bucketRight)
            {
                
            }
            else
            {
                while(t >= bucketRight)
                {
                    bucketLeft = bucketRight;
                    bucketRight = BucketLeft + delta;
                }
            }
            */
            
            values.push_back(value);
            secPastEpoch.push_back(t.secPastEpoch);
            nsec.push_back(t.nsec);
            data = reader->next();

            minValue = std::min(value, minValue);
            maxValue = std::max(value, maxValue);
            sum += value;
            N += 1;

        }
        
        // PACK
        pvValues->put(0, values.size(), &values[0], 0);
        pvSecPastEpoch->put(0, secPastEpoch.size(), &secPastEpoch[0], 0);
        pvNsec->put(0, nsec.size(), &nsec[0], 0);
        
        // so only do one name for now
        channelRPCRequester->requestDone(Status::OK, pvResult);
        break;

    }
}

}}

