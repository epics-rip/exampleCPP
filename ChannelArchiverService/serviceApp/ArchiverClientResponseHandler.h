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

struct FormatParameters
{
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
 * Handles the response from the archive service, according to supplied parameters.
 *
 * @param  response         The response sent by service.
 * @param  parameters       Parameters for the handling the request.
 * @return Status of the call, 0 indicates success, non-zero inidicates failure.
 */
int handleResponse(epics::pvData::PVStructure::shared_pointer response,
    const FormatParameters & parameters);


}

}

#endif
