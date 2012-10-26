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
#include <pv/logger.h>

#include "types.h"
#include "ArchiverClientResponseHandler.h"
#include "ServiceClient.h"

using namespace std::tr1;
using namespace epics::pvData;

namespace epics
{

namespace channelArchiverService
{

enum DebugLevel
{
    QUIET,
    NORMAL,
    VERBOSE
};

/**
 * Creates a plain, strings only query request to be sent to the archiver service for data
 * between start and end times for channel name.
 *
 * @param  channel       The name of the channel to query for.
 * @param  start         The seconds past epoch of the start time.
 * @return end           The seconds past epoch of the end time.
 */
PVStructure::shared_pointer createArchiverQuery(const std::string & channel, const std::string & start, const std::string &  end)
{
    StructureConstPtr archiverStructure = ArchiverQuery(*getFieldCreate());
    PVStructure::shared_pointer query(getPVDataCreate()->createPVStructure(archiverStructure));

    // Set request.
    query->getStringField(nameStr)->put(channel);
    query->getStringField(startStr)->put(start);
    query->getStringField(endStr)->put(end); 
    return query;
}

/**
 * Creates an NTURI request to be sent to the archiver service for data between
 * start and end times for channel name.
 *
 * @param  service       The service the request.
 * @param  channel       The name of the channel to query for.
 * @param  start         The seconds past epoch of the start time.
 * @return end           The seconds past epoch of the end time.
 */
PVStructure::shared_pointer createArchiverRequest(const std::string & service,
    const std::string & channel, const std::string & start, const std::string &  end)
{    
    StructureConstPtr archiverStructure = ArchiverRequest(*getFieldCreate());
    PVStructure::shared_pointer request(getPVDataCreate()->createPVStructure(archiverStructure));

    // set path.
    request->getStringField("path")->put(service);

    // Set query.
    PVStructure::shared_pointer query = request->getStructureField("query");

    query->getStringField(nameStr)->put(channel);
    query->getStringField(startStr)->put(start);
    query->getStringField(endStr)->put(end);
 
    return request;
}


/**
 * Converts the input string encoding the fields to be outpuuted into a
 * vector output field enums.
 *
 * @param  inString         The input string encoding the fields to be displayed.
 * @param  fields           The result containing fields to be displayed.
 */
void makeOutputtedFields(const std::string & inString, std::vector<OutputField> & fields)
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



/**
 * Display help for ArchiverClient.
 *
 */
void showHelp()
{
    std::cout << "ArchiverClient [OPTIONS] channel...\n"
              << "ArchiverClient -h\n\n";

    std::cout << "Queries a Channel Archive Server for the data for the channel\n\n"; 

    std::cout << "Options:\n"
      << "-h              display help\n"
      << "-S=SERVICE      query SERVICE for results\n" 
      << "-s= START_TIME  query archive for results starting from START_TIME\n"
      << "                in seconds after EPICS epoch\n"
      << "-e=END_TIME     query archive for results up to END_TIME\n"
      << "                in seconds after EPICS epoch\n"
      << "-f= FILENAME    output results of archiver query to file named FILENAME\n"
      << "-n              print channel name, preceeded by #, before results\n"
      << "-t              transpose rows and columns\n"
      << "-T              print column headers, preceeded by #, before results\n"
      << "                after channel name, if printed.\n"
      << "-q              supress all output to standard out except for archive data.\n"
      << "-v              output verbose logging information.\n"  
      << "-x              results of archiver query request outputted using\n"
      << "                scientific (i.e. exponent/mantissa) format"
      << "-d              results of archiver query request outputted using\n"
      << "                scientific (i.e. exponent/mantissa) format\n"
      << "-p=PREC         results of archiver query request given to precision PREC\n"
      << "                Default value is 6.\n"
      << "-o              specifies which fields to output, in which order"
      << std::endl;
}

}

}

/**
 * Channel Archiver Service Client main.
 *
 */ 
int main (int argc, char *argv[])
{
    using namespace  epics::channelArchiverService;
  
    int opt;

    std::string serviceName;
    int64_t t0     = 0;
    int64_t t1     = std::numeric_limits<int64_t>::max();

    FormatParameters parameters; 
    bool printChannelName = false;
    std::string outputtedFields;
    DebugLevel debugLevel = NORMAL;

    std::string start;
    std::string end;

    while ((opt = getopt(argc, argv, ":hS:s:e:f:ao:p:dxntTqv")) != -1)
    {
        switch (opt)
        {
        case 'h':
            showHelp();
            return 0;

        case 'S':
            serviceName = optarg;
            break;

        case 's':
            t0 = atol(optarg);
            start = optarg;
            break;

        case 'e':
            t1 = atol(optarg);
            end = optarg;
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
            parameters.transpose = true; 
            break;

        case 'T':
            parameters.printColumnTitles = true; 
            break;

        case 'q':
            debugLevel = QUIET; 
            break;

        case 'v':
            debugLevel = VERBOSE; 
            break;

        case '?':
            std::cerr << "illegal option" << std::endl;
            break;
        }
    }

    epics::pvAccess::pvAccessSetLogLevel(
        (debugLevel == QUIET) ? epics::pvAccess::logLevelOff
                              : (debugLevel == NORMAL) ? epics::pvAccess::logLevelInfo
                                                       : epics::pvAccess::logLevelDebug);

    makeOutputtedFields(outputtedFields, parameters.outputtedFields);

    if (optind >= argc)
    {
        std::cerr << "Error: Too few arguments to ArchiverClient" << std::endl;
        return 1;    
    }

    for (int i = optind; i < argc; ++i)
    {
        std::string channel = argv[optind];

        if (debugLevel != QUIET)
        {
            std::cout << "Querying " << serviceName << " (channel: "  << channel
                      << "  start: " << t0 << "  end: " << t1  << ")..." << std::endl;
        }

        if (printChannelName)
        {
            parameters.title = channel;
        }

        //  Create query and send to archiver service.
        //PVStructure::shared_pointer queryRequest = createArchiverQuery(channel, start, end);
        PVStructure::shared_pointer queryRequest = createArchiverRequest(serviceName, channel, start, end);

        if (debugLevel == VERBOSE)
        {
            std::cout << "Query:" << std::endl;        
            std::cout << toString(queryRequest) << std::endl;
        }

        double timeOut = 3.0;

        RequestResponseHandler::shared_pointer handler(new RequestResponseHandler(parameters));
        bool ok = epics::serviceClient::SendRequest(serviceName, queryRequest, handler, timeOut);        

        if (!ok)
        {
            std::cerr << "Error: Request failed." << std::endl;
        }
        else if (!handler->isOk())
        {
            std::cerr << "Error: Response handling failed." << std::endl;
        }
        else
        {
            if (debugLevel != QUIET)
            {
                std::cout << "Request successful. Processing results..." << std::endl;
            }

            if (debugLevel == VERBOSE)
            {
                std::cout << "Processings with parameters" << std::endl;

                if (parameters.filename != "")
                {
                    std::cout << "out file: " << parameters.filename
                              << (parameters.appendToFile ? " (append)" : " (overwrite)")
                              << std::endl;
                }
                std::cout << "precision: "    << parameters.precision << std::endl;
                std::cout << "format: ";
                switch (parameters.format)
                {
                case FormatParameters::SCIENTIFIC:
                    std::cout << "scientific";
                    break;

                case FormatParameters::FIXED_POINT:
                    std::cout << "fixed point";

                case FormatParameters::DEFAULT:
                    std::cout << "default";
                    break;

                default:
                    std::cout << "?";          
                    break;
                }
                std::cout << std::endl;

                std::cout << "Output fields: "; 
                for (std::vector<OutputField>::const_iterator it = parameters.outputtedFields.begin();
                     it != parameters.outputtedFields.end(); ++it)
                {
                    if (it != parameters.outputtedFields.begin())
                    {
                        std::cout << ", ";
                    }
                    std::cout << outputFieldNames[*it];
                }
                std::cout  << std::endl;
            }

            handler->outputResults();

            if (debugLevel != QUIET)
            {
                if (handler->isOk())
                {
                    std::cout << "Done. ";
                    if (parameters.filename != "")
                    {
                        std::cout << "Results " << (parameters.appendToFile ? "appended" : "written")
                                  << " to " <<  parameters.filename << ".";
                    }
                    std::cout << std::endl;
                }
                else
                {
                    std::cout << "Processing unsuccessful." << std::endl;
                }
            }
        }

        parameters.appendToFile = true; 
    }
 
    return 0;
}
