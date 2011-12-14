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

void LabelTable(PVStructure::shared_pointer pvResult)
{
    // fill in string[] labels
}

void QueryRaw(ChannelRPCRequester::shared_pointer const & channelRPCRequester, 
              std::string & indexName, std::vector<std::string> & names, 
              const epicsTimeStamp & t0, const epicsTimeStamp & t1, int how, int64_t count)
{

    PVStructure::shared_pointer pvResult(
        getPVDataCreate()->createPVStructure(
            NULL, MYArchiverTable(
                "MYArchiverTable", *getFieldCreate(), 0)));
    LabelTable(pvResult);
    
    std::vector<double> values;
    std::vector<int64_t> secPastEpoch;
    std::vector<int> nsec;
    std::vector<int> stats;
    std::vector<int> sevrs;
    std::vector<std::string> dates;

    AutoPtr<Index> index(new AutoIndex());
    index->open(indexName.c_str(), true);

    const epicsTime start = t0;

    AutoPtr<DataReader> reader(new RawDataReader(*index));
    
    for(size_t n = 0; n < names.size(); n++)
    {
        const stdString name = names[n].c_str();

        // QUERY
        const RawValue::Data *data = reader->find(name, &start);

        for(int c = 0; c < count; c++)
        {
            if(data == 0)
            {
                break;
            }
            double value;
            RawValue::getDouble(reader->getType(), reader->getCount(), data, value, 0);
            epicsTimeStamp t = RawValue::getTime(data);
            epicsTime tm(t);
            
            char buf[1024];
            buf[0] = '\0';
            buf[sizeof(buf)-1] = '\0';
            tm.strftime(buf, sizeof(buf)-1, "%c");
            dates.push_back(buf);
            
            int status = RawValue::getStat(data);
            int severity = RawValue::getSevr(data);

            values.push_back(value);
            secPastEpoch.push_back(t.secPastEpoch);
            nsec.push_back(t.nsec);
            stats.push_back(status);
            sevrs.push_back(severity);
            
            data = reader->next();

        }
        
        // PACK
        copyToScalarArray(secPastEpoch, pvResult, "secPastEpoch");
        copyToScalarArray(nsec, pvResult, "nsec");
        copyToScalarArray(dates, pvResult, "date");
        copyToScalarArray(values, pvResult, "value");
        copyToScalarArray(stats, pvResult, "status");
        copyToScalarArray(sevrs, pvResult, "severity");
        
        // so only do one name for now
        channelRPCRequester->requestDone(Status::OK, pvResult);
        break;
        
    }
}

void QueryStats(ChannelRPCRequester::shared_pointer const & channelRPCRequester, 
                std::string & indexName, std::vector<std::string> & names, 
                const epicsTimeStamp & t0, const epicsTimeStamp & t1, int how, int64_t count)
{
    PVStructure::shared_pointer pvResult(
        getPVDataCreate()->createPVStructure(
            NULL, MYScalarTransposedStatisticsArray(
                "MYScalarTransposedStatisticsArray", epics::pvData::pvDouble, *getFieldCreate())));
                                              
    std::vector<double> highValue;
    std::vector<double> lowValue;
    std::vector<double> meanValue;
    std::vector<double> stdDev;
    std::vector<int> Ns;
    std::vector<int> stat;
    std::vector<int> sevr;
    std::vector<int64_t> secPastEpoch;
    std::vector<int> nsec;

    AutoPtr<Index> index(new AutoIndex());
    index->open(indexName.c_str(), true);
    
    double delta = 60.0;

    // left edge of first bucket
    const epicsTime start = t0;

    AutoPtr<DataReader> reader(new RawDataReader(*index));

    for(size_t n = 0; n < names.size(); n++)
    {
        printf("name = %s\n", names[n].c_str());

        const stdString name = names[n].c_str();

        // QUERY
        const RawValue::Data *data = reader->find(name, &start);

        double minValue = DBL_MAX;
        double maxValue = DBL_MIN;
        int maxStatus = 0;
        int maxSeverity = 0;
        double sum = 0;
        double sumSq = 0;
        int N = 0;
        
        epicsTime left = t0;
        epicsTime right = left + delta;
        
        for(int c = 0; c < count; )
        {
            
            if(data == 0)
            {
                break;
            }

            double value;
            RawValue::getDouble(reader->getType(), reader->getCount(), data, value, 0);
            epicsTime t = RawValue::getTime(data);
            int status = RawValue::getStat(data);
            int severity = RawValue::getSevr(data);

            if(t < left)
            {
                // sample too early, skip
                data = reader->next();
                continue;
            }
            
            while(t >= right) // TODO could optimize for sparsity
            {
                // store bucket
                lowValue.push_back(minValue);
                highValue.push_back(maxValue);
                meanValue.push_back(N > 0 ? sum / N : 0);
                stdDev.push_back(N > 0 ? sqrt(N * sumSq - sum * sum) / N: 0);
                stat.push_back(maxStatus);
                sevr.push_back(maxSeverity);
                Ns.push_back(N);
                c += 1;
                
                epicsTimeStamp middle = left + delta / 2.0;

                secPastEpoch.push_back(middle.secPastEpoch);
                nsec.push_back(middle.nsec);

                // reset bucket
                minValue = DBL_MAX;
                maxValue = DBL_MIN;
                maxStatus = 0;
                maxSeverity = 0;
                sum = 0;
                sumSq = 0;
                N = 0;
                left = left + delta;
                right = right + delta;
            }
            
            // update current bucket
            maxStatus = std::max(status, maxStatus);
            maxSeverity = std::max(severity, maxSeverity);
            minValue = std::min(value, minValue);
            maxValue = std::max(value, maxValue);
            sum += value;
            sumSq += value * value;
            N += 1;
            data = reader->next();
        }
        
        // TODO copy final bucket

        // PACK
        
        // replaced with timestamp_t
        // copyToScalarArray(secPastEpoch, pvResult, "secPastEpoch");
        // copyToScalarArray(nsec, pvResult, "nsec");
        copyToScalarArray(highValue, pvResult, "highValue");
        copyToScalarArray(lowValue, pvResult, "lowValue");
        copyToScalarArray(meanValue, pvResult, "meanValue");
        copyToScalarArray(stdDev, pvResult, "stdDev");
        //copyToScalarArray(stat, pvResult, "status");
        //copyToScalarArray(sevr, pvResult, "severity");
        copyToScalarArray(Ns, pvResult, "N");
        
        // fill in the timestamp array
        {
            PVStructureArray * ts = pvResult->getStructureArrayField("timeStamp");
            ts->append(secPastEpoch.size());
            ts->setLength(secPastEpoch.size());
            StructureArrayData sa;
            ts->get(0, ts->getLength(), &sa);
            for(size_t n = 0; n < secPastEpoch.size(); n++)
            {
                sa.data[n]->getLongField("secPastEpoch")->put(secPastEpoch[n]);
                sa.data[n]->getIntField("nsec")->put(nsec[n]);
            }
        }
        
        // fill in the alarm array
        {
            PVStructureArray * ts = pvResult->getStructureArrayField("alarm");
            ts->append(stat.size());
            ts->setLength(stat.size());
            StructureArrayData sa;
            ts->get(0, ts->getLength(), &sa);
            for(size_t n = 0; n < stat.size(); n++)
            {
                sa.data[n]->getIntField("status")->put(stat[n]);
                sa.data[n]->getIntField("severity")->put(sevr[n]);
            }
        }
        
        // so only do one name for now
        channelRPCRequester->requestDone(Status::OK, pvResult);
        break;
        
    }
    
}

void ExampleServiceRPC::request(
    ChannelRPCRequester::shared_pointer const & channelRPCRequester,
    epics::pvData::PVStructure::shared_pointer const & pvArgument)
{
    
    std::cout << toString(pvArgument);

    // type checking by field names (no pattern matching yet)
    std::string typeString = toString(pvArgument->getStructure());
    std::string schema = toString(ArchiverClientStructure("ArchiverQuery", *getFieldCreate()));

    if(typeString != schema)
    {
        std::cout << "Type checking, wanted:" << std::endl << typeString << std::endl;
        std::cout << "Type checking, got:" << std::endl << schema << std::endl;
        channelRPCRequester->requestDone(Status(Status::STATUSTYPE_ERROR, "type mismatch"), pvArgument);
    }

    // UNPACK
    // here's where an interface compiler would be useful...
    std::string indexName = pvArgument->getStringField("index")->get();
    epicsTimeStamp t0;
    epicsTimeStamp t1;
    t0.secPastEpoch = pvArgument->getStructureField("t0")->getLongField("secPastEpoch")->get();
    t0.nsec = pvArgument->getStructureField("t0")->getIntField("nsec")->get();
    t1.secPastEpoch = pvArgument->getStructureField("t1")->getLongField("secPastEpoch")->get();
    t1.nsec = pvArgument->getStructureField("t1")->getIntField("nsec")->get();
    int64_t count = pvArgument->getLongField("count")->get();
    int how = pvArgument->getIntField("how")->get();
    std::vector<std::string> names;
    copyFromScalarArray(names, pvArgument, "names");
    
    if (how == ARCHIVER_HOW_STATISTICS)
    {
        return QueryStats(channelRPCRequester, indexName, names, t0, t1, how, count);
    }
    else
    {
        return QueryRaw(channelRPCRequester, indexName, names, t0, t1, how, count);
    }

} // request

} // namespace epics

} // namespace pvIOC

/*

How to fill in timestamp structures...?


*/
