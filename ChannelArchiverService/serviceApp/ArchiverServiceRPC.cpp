/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS exampleCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
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

/* EPICS Archiver Includes */
#include <epicsVersion.h>
#include <alarm.h>
#include <epicsMath.h>
#include <AutoPtr.h>
#include <MsgLogger.h>
#include <RegularExpression.h>
#include <BinaryTree.h>
#include <AutoIndex.h>
#include <SpreadsheetReader.h>
#include <LinearReader.h>
#include <PlotReader.h>

#include <ArchiverServiceRPC.h>


#include "types.h"

using namespace epics::pvData;
using namespace epics::pvAccess;

namespace channelArchiverService
{

ArchiverServiceRPC::~ArchiverServiceRPC() {}
void ArchiverServiceRPC::destroy() {}

/**
 * Fills in the list of table column labels
 */
void LabelTable(PVStructure::shared_pointer pvResult)
{
    std::vector<std::string> labels;
    labels.push_back("value");
    labels.push_back("secPastEpoch");
    labels.push_back("nsec");
    labels.push_back("date");
    labels.push_back("status");
    labels.push_back("severity");
    copyToScalarArray(labels, pvResult, "labels");
}

/**
 * Converts an epicsTime to a date string
 */
std::string getDate(epicsTime t)
{
    char buf[1024];
    buf[0] = '\0';
    buf[sizeof(buf)-1] = '\0';
    t.strftime(buf, sizeof(buf)-1, "%c");
    return buf;
}

/**
 * Queries the EPICS R-Tree Channel Archiver, returning raw samples
 */
void ArchiverServiceRPC::QueryRaw(ChannelRPCRequester::shared_pointer const & channelRPCRequester,
              epics::pvData::PVStructure::shared_pointer const & pvArgument,
              std::string & name, 
              const epicsTimeStamp & t0, const epicsTimeStamp & t1, int64_t count)
{
    std::cout << "Begin Query" << std::endl;

    /* Create the result pvStructure */

    PVStructure::shared_pointer pvResult(
        getPVDataCreate()->createPVStructure(
            NULL, ArchiverTable(
                "ArchiverTable", *getFieldCreate())));

    LabelTable(pvResult);

    /* The result table is built up as one STL vector per column */
    
    std::vector<double> values;
    std::vector<int64_t> secPastEpoch;
    std::vector<int> nsec;
    std::vector<int> stats;
    std::vector<int> sevrs;
    std::vector<std::string> dates;

    /* Open the Index */

    AutoPtr<Index> index(new AutoIndex());
    
    try
    {
        index->open(indexFilename.c_str(), true);
    }
    catch(GenericException & e)
    {
        std::cout << e.what() << std::endl;
        channelRPCRequester->requestDone(Status(Status::STATUSTYPE_ERROR, e.what()), pvArgument);
        return;
    }

    const epicsTime start = t0;
    const epicsTime end = t1;

    /* Create a Database Cursor */

    AutoPtr<DataReader> reader(new RawDataReader(*index));
    
    /* Seek to the first sample at or before 'start' for the named channel */

    const RawValue::Data *data = reader->find(stdString(name.c_str()), &start);

    /* find returns the reading immediately before start, unless start date is
       before first reading in archive, so skip to next.*/
    if((data != 0) && (RawValue::getTime(data) < start))
    {
        data = reader->next();
    }
 

    /* Fill the table */

    for(int64_t c = 0; c < count; c++)
    {
        if(data == 0)
        {
            break;
        }
        double value;
        
        /* missing support for waveforms and strings */

        RawValue::getDouble(reader->getType(), reader->getCount(), data, value, 0);
        epicsTimeStamp t = RawValue::getTime(data);

        if(end < t)
        {
            break;
        }
            
        dates.push_back(getDate(t));
            
        int status = RawValue::getStat(data);
        int severity = RawValue::getSevr(data);

        values.push_back(value);
        secPastEpoch.push_back(t.secPastEpoch);
        nsec.push_back(t.nsec);
        stats.push_back(status);
        sevrs.push_back(severity);
            
        data = reader->next();

    }
        
    /* Pack the table into the pvStructure using some STL helper functions */

    copyToScalarArray(secPastEpoch, pvResult, "secPastEpoch");
    copyToScalarArray(nsec, pvResult, "nsec");
    copyToScalarArray(dates, pvResult, "date");
    copyToScalarArray(values, pvResult, "value");
    copyToScalarArray(stats, pvResult, "status");
    copyToScalarArray(sevrs, pvResult, "severity");

    std::cout << "End Query" << std::endl;
        
    channelRPCRequester->requestDone(Status::OK, pvResult);

}

void ArchiverServiceRPC::request(
    ChannelRPCRequester::shared_pointer const & channelRPCRequester,
    epics::pvData::PVStructure::shared_pointer const & pvArgument)
{
    
    std::cout << toString(pvArgument) << std::endl;

    /* Check the request type by comparing the schemas */
    
    std::string typeString = toString(pvArgument->getStructure());
    std::string schema = toString(ArchiverQuery("ArchiverQuery", *getFieldCreate()));

    if(typeString != schema)
    {
        std::cout << "Type checking, wanted:" << std::endl << typeString << std::endl;
        std::cout << "Type checking, got:" << std::endl << schema << std::endl;
        channelRPCRequester->requestDone(Status(Status::STATUSTYPE_ERROR, "type mismatch"), pvArgument);
        return;
    }

    /* Unpack the request type */

    epicsTimeStamp t0, t1;
    t0.secPastEpoch = pvArgument->getLongField("t0secPastEpoch")->get();
    t0.nsec = pvArgument->getIntField("t0nsec")->get();
    t1.secPastEpoch = pvArgument->getLongField("t1secPastEpoch")->get();
    t1.nsec = pvArgument->getIntField("t1nsec")->get();
    std::string name = pvArgument->getStringField("name")->get();
    //int64_t count = pvArgument->getLongField("count")->get();
    int64_t count = 1000000000; // limit to 1e9 results for now, count will become an optional parameter
    return QueryRaw(channelRPCRequester, pvArgument, name, t0, t1, count);

}

}
