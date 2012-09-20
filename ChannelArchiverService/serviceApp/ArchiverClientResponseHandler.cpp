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

#include <alarm.h>
#include <alarmString.h>
#include <ArrayTools.h>

#include <epicsGetopt.h>
#include <epicsStdlib.h>
#include <epicsTime.h>

#include <RawValue.h>

#include <pv/pvData.h>


using namespace std;
using namespace std::tr1;
using namespace epics::pvData;


#include "types.h"

#include "ArchiverClientResponseHandler.h"

namespace epics
{

namespace channelArchiverService
{

/**
 * Converts an epics alarm status and severity to a string, including
 * archiver special severities.
 *
 * @param  status       Alarm status.
 * @param  severity     Alarm severity. 
 */
std::string MakeAlarmString(short status, short severity)
{
    std::string result;
    char buf[200];

    severity &= 0xfff;
    switch (severity)
    {
    case NO_ALARM:
        result = "NO ALARM";
        return result;
    //  Archiver specials:
    case ARCH_EST_REPEAT:
        sprintf(buf, "Est_Repeat %d", (int)status);
        result = buf;
        return result;
    case ARCH_REPEAT:
        sprintf(buf, "Repeat %d", (int)status);
        result = buf;
        return result;
    case ARCH_DISCONNECT:
        result = "Disconnected";
        return result;
    case ARCH_STOPPED:
        result = "Archive_Off";
        return result;
    case ARCH_DISABLED:
        result = "Archive_Disabled";
        return result;
    }

    if (severity < static_cast<short>(SIZEOF_ARRAY(alarmSeverityString))  &&
        status < static_cast<short>(SIZEOF_ARRAY(alarmStatusString)))
    {
        result = alarmSeverityString[severity];
        result += " ";
        result += alarmStatusString[status];
    }
    else
    {
        sprintf(buf, "%d %d", severity, status);
        result = buf;
    }
    return result;
}


/**
 * Converts an epicsTime to a date string
 *
 * @param  t  the time to convert. 
 * @return The date string.
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
 * Converts secs past epoch and nsecs to a date string
 *
 * @param  secsPastEpoch seconds past EPICS epoch. 
 * @param  nsecs         nanseconds after second.
 * @return The date string.
 */
std::string getDate(int64_t secsPastEpoch, int32_t nsecs)
{
    epicsTimeStamp ts = { static_cast<epicsUInt32>(secsPastEpoch), static_cast<epicsUInt32>(nsecs)};
    epicsTime t(ts);
    return getDate(t);
}

/**
 * Converts data from array data object to strings according to format parameters 
 * and adds to vector of strings 
 *
 * @param  strings    Array of strings to add to. 
 * @param  arrayData  The array data to add.
 * @param  format     Format used to convert data to string. 
 * @param  precision  Precision used in formating when converting data to string. 
 */
template <typename A>
void dataArrayToVectorOfStrings(vector<string> & strings, const A & arrayData, int length,
                               const FormatParameters::Format format = FormatParameters::DEFAULT, int precision = 6)
{
    strings.reserve(strings.size() + length);
    ostringstream oss;

    switch(format)
    {
    case FormatParameters::SCIENTIFIC:
        oss << showpoint << scientific << setprecision(precision);
        break;

    case FormatParameters::FIXED_POINT:
        oss << showpoint << fixed << setprecision(precision);        
        break;

    case FormatParameters::DEFAULT:
        oss << showpoint  << setprecision(precision);
        break;

    case FormatParameters::HEX:
        oss << hex;
        break;
    }
    for (size_t i = arrayData.offset; i < arrayData.offset + length; ++i)
    {
        oss << arrayData.data[i];
        strings.push_back(oss.str());
        oss.str("");    
    }
}


/**
 * Class to perform the handling of the response from the archive service.
 */
class RequestResponseHandler
{
public:
/**
 * Constructor.
 *
 * @param  parameters       Parameters for the handling the request.
 */
RequestResponseHandler(const FormatParameters & parameters)
: m_parameters(parameters)
{
}

/**
 * Handles the response from the archive service, according to supplied parameters.
 *
 * @param  response         The response sent by service.
 * @return Status of the call, 0 indicates success, non-zero indicates failure.
 */
int handle(shared_ptr<epics::pvData::PVStructure> response)
{
    vector<string> outputFieldValues[NUMBER_OF_FIELDS];

    //  Handle each of the fields in the archiver query response in turn.

    //  Values.
    PVDoubleArrayPtr values = std::tr1::static_pointer_cast<epics::pvData::PVDoubleArray>(response->getScalarArrayField("value", pvDouble));
    DoubleArrayData valuesArrayData;
    int valuesLength = values->get(0, values->getLength(), valuesArrayData);

    dataArrayToVectorOfStrings(outputFieldValues[VALUE], valuesArrayData, valuesLength, m_parameters.format, m_parameters.precision);


    //  Seconds.
    PVLongArrayPtr secPastEpochs = std::tr1::static_pointer_cast<epics::pvData::PVLongArray>(response->getScalarArrayField("secPastEpoch", pvLong));
    LongArrayData secPastEpochsArrayData;

    int secPastEpochsLength = secPastEpochs->get(0, secPastEpochs->getLength(), secPastEpochsArrayData);
    if (secPastEpochsLength != valuesLength)
    {
        cerr << "Data invalid: Secs past epoch and Value lengths don't match." << endl;
        return 1;  
    }

    dataArrayToVectorOfStrings(outputFieldValues[SECONDS_PAST_EPOCH], secPastEpochsArrayData, secPastEpochsLength);


    //  Nanoseconds.
    PVIntArrayPtr nsecs = std::tr1::static_pointer_cast<epics::pvData::PVIntArray>(response->getScalarArrayField("nsec", pvInt));
    IntArrayData nsecsArrayData;
    int nsecsLength = nsecs->get(0, nsecs->getLength(), nsecsArrayData);
    if (nsecsLength != valuesLength)
    {
        cerr << "Data invalid: nsecs past epoch and Value lengths don't match." << endl;
        return 1;  
    }

    dataArrayToVectorOfStrings(outputFieldValues[NANO_SECONDS], nsecsArrayData, nsecsLength);


    //  Real time in seconds.
    int realTimeLength = min(secPastEpochsLength, nsecsLength);
    vector<string> & realTimeStrings = outputFieldValues[REAL_TIME];
    realTimeStrings.reserve(realTimeLength);

    {
        ostringstream oss;
        for (int i = 0; i < realTimeLength; ++i)
        {
            oss << secPastEpochsArrayData.data[i]  << ".";
            oss << setfill('0') << setw(9) << nsecsArrayData.data[i];
            realTimeStrings.push_back(oss.str());
            oss.str("");
        }
    }

    //  Dates.
    vector<string> & dateStrings = outputFieldValues[DATE];
    int dateLength = min(secPastEpochsLength, nsecsLength);
    dateStrings.reserve(dateLength);

    for (int i = 0; i < dateLength; ++i)
    {     
        string dateString = getDate(secPastEpochsArrayData.data[i], nsecsArrayData.data[i]);
        dateStrings.push_back(dateString);
    }


    //  Alarm status.
    PVIntArrayPtr statuses = std::tr1::static_pointer_cast<epics::pvData::PVIntArray>(response->getScalarArrayField("status", pvInt));
    IntArrayData statusesArrayData;
    int statusesLength = statuses->get(0, statuses->getLength(), statusesArrayData);
    if (statusesLength != valuesLength)
    {
        cerr << "Data invalid: Alarm Status and Value lengths don't match." << endl;
        return 1;  
    }

    dataArrayToVectorOfStrings(outputFieldValues[STATUS], statusesArrayData, statusesLength, FormatParameters::HEX);


    //  Alarm severity.
    PVIntArrayPtr severities = std::tr1::static_pointer_cast<epics::pvData::PVIntArray>(response->getScalarArrayField("severity", pvInt));
    IntArrayData severitiesArrayData;
    int severitiesLength = severities->get(0, severities->getLength(), severitiesArrayData);
    if (severitiesLength != valuesLength)
    {
        cerr << "Data invalid: Alarm Severity and Value lengths don't match." << endl;
        return 1;  
    }

    dataArrayToVectorOfStrings(outputFieldValues[SEVERITY], severitiesArrayData, severitiesLength, FormatParameters::HEX);


    //  Alarm string.
    int alarmStringsLength = std::min(secPastEpochsLength, nsecsLength);
    vector<string> & alarmStrings = outputFieldValues[ALARM];
    alarmStrings.reserve(alarmStringsLength);

    for (int i = 0; i < valuesLength; ++i)
    {     
        string alarmString = MakeAlarmString(statusesArrayData.data[i], severitiesArrayData.data[i]);
        alarmStrings.push_back(alarmString);
    }


    //  Now output archive data.
    bool outputToFile = m_parameters.filename.compare(string(""));
    std::ofstream outfile;

    if (outputToFile)
    {
        ios_base::openmode openMode = m_parameters.appendToFile ? (ios_base::out | ios_base::app) : ios_base::out;
        outfile.open(m_parameters.filename.c_str(), openMode);
    }

    ostream & out = outputToFile ? outfile : std::cout; 

    //  Print title.
    bool printTitle = m_parameters.title.compare(string(""));
    if (printTitle)
    {
        out << m_parameters.prefix << m_parameters.title << std::endl;
    }


    size_t maxWidths[NUMBER_OF_FIELDS];
    for (int i = 0; i < NUMBER_OF_FIELDS; ++i)
    {
        maxWidths[i] = maxWidth(outputFieldValues[i]);
    }

    string columnSpace = "  ";


    //  Print column headers if required.    
    const string columnTitles[] = {
        "timePastEpoch(s)",
        "value",
        "Date",
        "Alarm",
        "secsPastEpoch",
        "nsecs",
        "Status",
        "Severity"
    };

    if (m_parameters.printColumnTitles)
    {
        for (size_t i = 0; i < m_parameters.outputtedFields.size(); ++i)
        {
            OutputField field = m_parameters.outputtedFields[i];
            string columnTitle = m_parameters.prefix;
            columnTitle += columnTitles[field];
            maxWidths[field] = std::max(maxWidths[field], columnTitle.length());
            out << setw(maxWidths[field]) << left << columnTitle << columnSpace; 
        }
        out << "\n";
    }


    //  Output archive data values. 
    typedef ios_base & (*align_t)(ios_base &);
    const align_t alignments[] = {
        right,
        right,
        left,
        left,
        right,
        right, 
        right,
        right
    }; 

    for (int j = 0; j < valuesLength; ++j) 
    {
        for (size_t i = 0; i < m_parameters.outputtedFields.size(); ++i)
        {
            OutputField field = m_parameters.outputtedFields[i];

            out << setw(maxWidths[field])      << alignments[field]
                << outputFieldValues[field][j]   << columnSpace;   
        }
        out << "\n";
    }
    out.flush();


    if (outputToFile)
    {
        outfile.close();
    }  

    return 0;
}


private:
    FormatParameters m_parameters;
};


/*
  Handle response
*/
int handleResponse(PVStructure::shared_pointer response, const FormatParameters & parameters)
{
   RequestResponseHandler handler(parameters);
   return handler.handle(response);
}

}

}
