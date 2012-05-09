/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS exampleCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
#ifndef ARCHIVERCLIENTRESPONSEHANDLER_H
#define ARCHIVERCLIENTRESPONSEHANDLER_H

#include <string>

namespace epics
{

namespace channelArchiverService
{
/**
 * Fields outputted by Channel Archiver client
 */
enum ArchiverField 
{
    REAL_TIME,
    VALUE,
    DATE,
    ALARM,
    SECONDS_PAST_EPOCH,
    NANO_SECONDS,
    STATUS,
    SEVERITY
};

const int NUMBER_OF_FIELDS = SEVERITY+1;

/**
 * Parameters according to which the results of the archiver query will be handled.
 */
struct FormatParameters
{
    /**
     * Options for formatting a field.
     */
    enum Format
    {
        DEFAULT,
        SCIENTIFIC,
        FIXED_POINT,
        HEX
    };

    std::vector<ArchiverField> displayedFields;
    std::string filename;
    std::string title;
    std::string prefix;
    Format format;
    int    precision;
    bool   appendToFile;
    bool   printColumnTitles;
};


/**
 * Handles the response from the archiver service, according to supplied parameters.
 *
 * @param  response         The response sent by service.
 * @param  parameters       Parameters for the handling the request.
 * @return Status of the call, 0 indicates success, non-zero indicates failure.
 */
int handleResponse(epics::pvData::PVStructure::shared_pointer response,
    const FormatParameters & parameters);


}

}

#endif
