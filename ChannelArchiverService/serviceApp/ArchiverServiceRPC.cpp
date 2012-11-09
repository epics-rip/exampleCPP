/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS exampleCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>
#include <sstream>
#include <string>
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

#include <pv/rpcServer.h>

#include "ArchiverServiceRPC.h"
#include "common.h"


using namespace epics::pvData;
using namespace epics::pvAccess;

namespace epics
{

namespace channelArchiverService
{

ArchiverServiceRPC::~ArchiverServiceRPC() {}

namespace
{

/**
 * Converts a string to a Long.
 * Throws an exception if conversion impossible.
 *
 * @param  str  the argument for the rpc
 * @return      the result of the conversion.
 */
int64_t toLong(const std::string & str)
{
    int64_t result = 0;
    std::stringstream ss(str);
    if (!(ss >> result))
    {
        throw RPCRequestException(Status::STATUSTYPE_ERROR,
            "Cannot convert string " + str + " to Long");
    }
    return result;
}

}

/**
 * Fills in the list of table column labels
 */
void LabelTable(PVStructure::shared_pointer pvResult)
{
    std::vector<std::string> labels;
    labels.push_back("value");
    labels.push_back("secPastEpoch");
    labels.push_back("nsec");
    labels.push_back("status");
    labels.push_back("severity");
    copyToScalarArray(labels, pvResult, "labels");
}

/**
 * Queries the EPICS R-Tree Channel Archiver, returning raw samples
 */
PVStructure::shared_pointer ArchiverServiceRPC::queryRaw(
    epics::pvData::PVStructure::shared_pointer const & pvArgument,
    std::string & name, 
    const epicsTimeStamp & t0,
    const epicsTimeStamp & t1,
    int64_t count)
{
    std::cout << "Begin Query" << std::endl;

    /* Create the result pvStructure */

    PVStructure::shared_pointer pvResult(
        getPVDataCreate()->createPVStructure(makeArchiverResponseStructure(*getFieldCreate())));

    LabelTable(pvResult);

    /* The result table is built up as one STL vector per column */
    
    std::vector<double> values;
    std::vector<int64_t> secPastEpoch;
    std::vector<int> nsec;
    std::vector<int> stats;
    std::vector<int> sevrs;

    /* Open the Index */

    AutoPtr<Index> index(new AutoIndex());
    
    try
    {
        index->open(indexFilename.c_str(), true);
    }
    catch(GenericException & e)
    {
        std::cout << e.what() << std::endl;
        throw RPCRequestException(Status::STATUSTYPE_ERROR, e.what());
    }

    const epicsTime start = t0;
    const epicsTime end = t1;

    /* Create a Database Cursor */

    AutoPtr<DataReader> reader(new RawDataReader(*index));
    
    /* Seek to the first sample at or before 'start' for the named channel */
    const RawValue::Data *data = 0;
    try
    {
        data = reader->find(stdString(name.c_str()), &start);
    }
    catch(...)
    {
        throw RPCRequestException(Status::STATUSTYPE_ERROR, "Error querying archive");
    }

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

    PVStructure::shared_pointer resultValues = pvResult->getStructureField("value");    

    copyToScalarArray(values, resultValues, "value");
    copyToScalarArray(secPastEpoch, resultValues, "secPastEpoch");
    copyToScalarArray(nsec, resultValues, "nsec");
    copyToScalarArray(stats, resultValues, "status");
    copyToScalarArray(sevrs, resultValues, "severity");

    std::cout << "End Query" << std::endl;
        
    return pvResult;
}


/**
 * Queries the EPICS R-Tree Channel Archiver, returning raw samples
 */
epics::pvData::PVStructure::shared_pointer ArchiverServiceRPC::request(
    epics::pvData::PVStructure::shared_pointer const & pvArgument
    ) throw (RPCRequestException)
{    
    std::cout << toString(pvArgument) << std::endl;

    /* Unpack the request type */
    std::string name;
    int64_t start = 0;
    int64_t end   = std::numeric_limits<int32_t>::max();
    int64_t count = 1000000000; // limit to 1e9 values unless another number is specified

    bool isNTQuery = false;

    if ( (pvArgument->getSubField("path") != NULL) && (pvArgument->getStringField("path") != NULL) 
      && (pvArgument->getSubField("query") != NULL) && (pvArgument->getStructureField("query") != NULL))
    {
        isNTQuery = true;
    }

    epics::pvData::PVStructure::shared_pointer const & query
        = isNTQuery ? pvArgument->getStructureField("query") : pvArgument;

    if (query->getStringField(nameStr) != NULL)
    {
        name = query->getStringField(nameStr)->get();
        if (name == "")
        {
            throw RPCRequestException(Status::STATUSTYPE_ERROR, "Empty channel name");
        }
    }
    else
    {
        throw RPCRequestException(Status::STATUSTYPE_ERROR, "No channel name");
    }
        
    if ((query->getSubField(startStr) != NULL) && (query->getStringField(startStr) != NULL))
    {
        start = toLong((query->getStringField(startStr)->get()));
    }

    if ((query->getSubField(endStr) != NULL) && (query->getStringField(endStr) != NULL))
    {
        end = toLong((query->getStringField(endStr)->get()));
    }

    if ((query->getSubField(countStr) != NULL) && (query->getStringField(countStr) != NULL))
    {
        count = toLong((query->getStringField(countStr)->get()));
    }

    epicsTimeStamp t0, t1;

    t0.secPastEpoch = start;
    t0.nsec = 0;
    t1.secPastEpoch  = end;
    t1.nsec = 0;

    return queryRaw(pvArgument, name, t0, t1, count);
}

}

}
