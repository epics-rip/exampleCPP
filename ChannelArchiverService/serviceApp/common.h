#ifndef CHANNELARCHIVERSERVICE_COMMON_H
#define CHANNELARCHIVERSERVICE_COMMON_H

/* common.h - channel archiver service types and utilities common to the server and client */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS exampleCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <string>
#include <vector>

#include "pv/pvIntrospect.h"
#include "pv/pvData.h"

namespace epics
{

namespace channelArchiverService
{

/**
 * Returns a scalar array of Type T from PVStructure
 *
 * @param  pvStructure  the PVStructure
 * @param  name         the name of the field
 * @return              the scalar array
 */
template <typename T>
typename T::shared_pointer getScalarArrayField(const pvData::PVStructurePtr & pvStructure, std::string name)
{
    return std::tr1::static_pointer_cast<T>(pvStructure->getScalarArrayField(name, T::typeCode));
}

// non template versions of getScalarArrayField for each type used

inline pvData::PVDoubleArrayPtr getDoubleArrayField(const pvData::PVStructurePtr & pvStructure, std::string name)
{
    return getScalarArrayField<pvData::PVDoubleArray>(pvStructure, name);
}

inline pvData::PVLongArrayPtr getLongArrayField(const pvData::PVStructurePtr & pvStructure, std::string name)
{
    return getScalarArrayField<pvData::PVLongArray>(pvStructure, name);
}

inline pvData::PVIntArrayPtr getIntArrayField(const pvData::PVStructurePtr & pvStructure, std::string name)
{
    return getScalarArrayField<pvData::PVIntArray>(pvStructure, name);
}

inline pvData::PVStringArrayPtr getStringArrayField(const pvData::PVStructurePtr & pvStructure, std::string name)
{
    return getScalarArrayField<pvData::PVStringArray>(pvStructure, name);
}


/**
 * Calculates the maximum of the length for a collection of strings or other objects with
 * a length() function.
 *
 * @param  t  the object of type T to be converted
 * @return    the result of the string conversion
 */
template <typename T>
size_t maxWidth(const T & t)
{
    typedef typename T::const_iterator const_iterator;

    size_t maxWidth = 0;
    for (const_iterator it = t.begin(); it != t.end(); ++it)
    {
	    size_t width = it->length();
	    if (width > maxWidth)
	    {
			maxWidth = width;
		}
	}
	return maxWidth;
}


// IDs for normative types
extern const std::string ntTableStr;
extern const std::string ntURIStr;


// Fieldnames of the channel archiver serivce query fields
extern const std::string nameStr;
extern const std::string startStr;
extern const std::string endStr;
extern const std::string countStr;

/**
 * Creates a structure for a query to a service with the supplied fields.
 *
 * @param  factory      the factory used to create the structure
 * @param  queryFields  the fields which the query is to contain
 * @return              the query structure
 */
epics::pvData::StructureConstPtr makeQueryStructure(epics::pvData::FieldCreate & factory,
    const std::vector<std::string> & queryFields);

/**
 * Creates a request structure for a query to a service with the supplied fields/
 *
 * @param  factory      the factory used to create the structure
 * @param  queryFields  the fields which the request query part is to contain
 * @return              the request structure
 */
epics::pvData::StructureConstPtr makeRequestStructure(epics::pvData::FieldCreate & factory,
    const std::vector<std::string> & queryFields);

/**
 * Creates the structure for the response to an archiver query
 *
 * @param  factory  the factory used to create the structure
 * @return          the table structure.
 */
epics::pvData::StructureConstPtr makeArchiverResponseStructure(epics::pvData::FieldCreate & factory);

}

}

#endif
