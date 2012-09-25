/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS exampleCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>


#include <epicsGetopt.h>
#include <epicsStdlib.h>
#include <epicsTime.h>

#include <RawValue.h>

#include <pv/pvData.h>


#include "types.h"




#include "ArchiverClientResponseHandler.h"
#include "ServiceClient.h"

#define VERBOSE_DEBUG 0

using namespace std::tr1;
using namespace epics::pvData;
using std::string;

namespace epics
{

namespace channelArchiverService
{

/**
 * Creates the request to be sent to the archiver service for data between
 * start and end times for channel name.
 *
 * @param  channel          The name of the channel to query for.
 * @param  t0secPastEpoch   The seconds past epoch of the start time.
 * @return t1secPastEpoch   The seconds past epoch of the end time.
 */
PVStructure::shared_pointer createArchiverQueryRequest(string channel, int64_t t0secPastEpoch, int64_t t1secPastEpoch)
{
    StructureConstPtr archiverStructure = ArchiverQuery(*getFieldCreate());
    PVStructure::shared_pointer queryRequest(getPVDataCreate()->createPVStructure(archiverStructure));

    // Set request.
    queryRequest->getStringField("name")->put(channel);
    queryRequest->getLongField("t0secPastEpoch")->put(t0secPastEpoch);
    queryRequest->getLongField("t1secPastEpoch")->put(t1secPastEpoch); 
    return queryRequest;
}

/**
 * Converts the input string encoding the fields to be outpuuted into a
 * vector output field enums.
 *
 * @param  inString         The input string encoding the fields to be displayed.
 * @param  fields           The result containing fields to be displayed.
 */
void makeOutputtedFields(string inString, std::vector<OutputField> & fields)
{
    for (size_t i = 0; i < inString.length();++i)
    {
        char fieldChar = inString[i];
        switch(fieldChar) 
        {
        case 't': 
            fields.push_back(REAL_TIME);            
            break;

        case 'v':
            fields.push_back(VALUE);   
            break;

        case 'D':
            fields.push_back(DATE); 
            break;

        case 'A':
           fields.push_back(ALARM); 
           break;

        case 's':
            fields.push_back(SECONDS_PAST_EPOCH); 
            break;

        case 'n':
            fields.push_back(NANO_SECONDS); 
            break;

        case 'S':
            fields.push_back(STATUS);
            break;

        case 'V':
            fields.push_back(SEVERITY); 
            break;

        default:
            break; 
        }
    }
}

}

}


int main (int argc, char *argv[])
{
    using namespace  epics::channelArchiverService;
  
    int opt;

    string serviceName;
    int64_t t0     = 0;
    int64_t t1     = std::numeric_limits<int64_t>::max();

    FormatParameters parameters; 
    bool printChannelName = false;
    string outputtedFields;

    while ((opt = getopt(argc, argv, ":S:s:e:f:ao:p:dxnt")) != -1)
    {
        switch (opt)
        {
            case 'S':
                serviceName = optarg;
                break;

            case 's':
                t0 = atoi(optarg);
                break;

            case 'e':
                t1 = atoi(optarg);
                break;

            case 'f':
                parameters.filename = optarg;
                break;

            case 'a':
                parameters.appendToFile = true;
                break;

            case 'o':
                outputtedFields = optarg;
                break;

            case 'p':
                parameters.precision = atoi(optarg);
                break;

            case 'd':
                parameters.format = FormatParameters::FIXED_POINT;
                break;

            case 'x':
                parameters.format = FormatParameters::SCIENTIFIC; 
                break;

            case 'n':
                printChannelName = true; 
                break;

            case 't':
                parameters.printColumnTitles = true; 
                break;

            case '?':
                std::cerr << "illegal option" << std::endl;
                break;
        }
    }

#if VERBOSE_DEBUG
    std::cout << "service:" << serviceName << std::endl;
    std::cout << "start:"    << t0 << std::endl;
    std::cout << "end:"      << t1 << std::endl;
#endif

    makeOutputtedFields(outputtedFields, parameters.outputtedFields);

    if (optind >= argc)
    {
        std::cerr << "Error: Too few arguments to ArchiverClient" << std::endl;
        return 1;    
    }

    for (int i = optind; i < argc; ++i)
    {
        string channel = argv[optind];

#if VERBOSE_DEBUG
        std::cout << "channel:"  << channel << std::endl;
#endif

        std::cout << channel << std::endl;
        if (printChannelName)
        {
            parameters.title = channel;
        }

        //  Create query and send to archiver service.
        PVStructure::shared_pointer queryRequest = createArchiverQueryRequest(channel, t0, t1);

        std::cout << "Query:" << std::endl;        
        std::cout << toString(queryRequest) << std::endl;

        double timeOut = 3.0;
    
        PVStructure::shared_pointer queryResponse
             = epics::serviceClient::SendRequest(serviceName, queryRequest, timeOut);

        if (queryResponse == NULL)
        {
            std::cout << "Error: Request failed." << std::endl;
        }
        else
        {
            handleResponse(queryResponse, parameters);
        }

        parameters.appendToFile = true; 
    }
 
    return 0;
}
