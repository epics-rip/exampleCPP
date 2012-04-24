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
#include <sstream>
#include <string>
#include <vector>


#include <epicsGetopt.h>
#include <epicsStdlib.h>
#include <epicsTime.h>

#include <RawValue.h>

#include <pv/pvData.h>


#include "types.hpp"




#include "ArchiverClientResponseHandler.h"
#include "ServiceClient.h"


using namespace std::tr1;
using namespace epics::pvData;
using std::string;

namespace epics
{

namespace channelArchiverService
{

PVStructure::shared_pointer createArchiverQueryRequest(string channel, int64_t t0secPastEpoch, int64_t t1secPastEpoch)
{
    StructureConstPtr archiverStructure = ArchiverQuery("ArchiverQuery", *getFieldCreate());
    PVStructure::shared_pointer queryRequest(getPVDataCreate()->createPVStructure(NULL, archiverStructure));

    // Set request.
    queryRequest->getStringField("name")->put(channel);
    queryRequest->getLongField("t0secPastEpoch")->put(t0secPastEpoch);
    queryRequest->getLongField("t1secPastEpoch")->put(t1secPastEpoch); 
    return queryRequest;
}

FormatParameters makeFormatParameters(string displayedFields, string filename, int precision, string options, string title)
{
    FormatParameters parameters;

    for (size_t i = 0; i < displayedFields.length();++i)
    {
        char fieldChar = displayedFields[i];
        switch(fieldChar) 
        {
        case 't': 
            parameters.displayedFields.push_back(REAL_TIME);            
            break;

        case 'v':
            parameters.displayedFields.push_back(VALUE);   
            break;

        case 'D':
            parameters.displayedFields.push_back(DATE); 
            break;

        case 'A':
           parameters.displayedFields.push_back(ALARM); 
           break;

        case 's':
            parameters.displayedFields.push_back(SECONDS_PAST_EPOCH); 
            break;

        case 'n':
            parameters.displayedFields.push_back(NANO_SECONDS); 
            break;

        case 'S':
            parameters.displayedFields.push_back(STATUS);
            break;

        case 'V':
            parameters.displayedFields.push_back(SEVERITY); 
            break;

        default:
            break; 
        }
    }

    parameters.appendToFile = (options.find("a") != string::npos);
    parameters.format  = FormatParameters::DEFAULT;
    parameters.prefix = "#";

    if (options.find("n") != string::npos)
    {
        parameters.title = title;
    }

    parameters.printColumnTitles = (options.find("t") != string::npos);

    if (options.find("x") != string::npos)
    {
        parameters.format = FormatParameters::SCIENTIFIC;     
    }
    else if (options.find("d") != string::npos)
    {
        parameters.format = FormatParameters::FIXED_POINT;     
    }

    parameters.precision = precision;
    parameters.filename  = filename;

    return parameters;
}

}

}

int main (int argc, char *argv[])
{
    using namespace  epics::channelArchiverService;

    const int minArgs = 8;
    if (argc < minArgs)
    {
        std::cerr << "argc:"     << argc << std::endl;
        std::cerr << "Error: Too few arguments to ArchiverClient (" 
                  << argc-1 << "). " << minArgs-1 << "+ expected." << std::endl;
        return 1;
    }

    //  Get supplied service name for archiver service.
    string serviceName = argv[1];
    std::cout << "service:" << serviceName << std::endl;


    //  Get parameters for the archiver query.
    string channel = argv[2];
    int64_t t0     = atol(argv[3]);
    int64_t t1     = atol(argv[4]);

    std::cout << "channel:"  << channel << std::endl;
    std::cout << "start:"    << t0 << std::endl;
    std::cout << "end:"      << t1 << std::endl;


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
        return 1;
    }
    else
    {
        //  Get format parameters for the archiver response.
        string displayedFields = string(argv[5]);
        string options         = string(argv[6]);
        int    precision       = atoi(argv[7]);
        string filename        = (argc < 9) ? "" : argv[8];

        std::cout << "displayed fields: " << displayedFields << std::endl;
        std::cout << "output file: "      << filename        << std::endl; 
        std::cout << "precision: "        << precision       << std::endl;
        std::cout << "options: "          << options         << std::endl;  

        FormatParameters parameters = makeFormatParameters(displayedFields, filename, precision, options, channel);


        handleResponse(queryResponse, parameters);
    }
    
    return 0;
}

