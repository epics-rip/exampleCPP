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

/* Map type T to pvData Type Identifier through specialisation */

template<class T> epics::pvData::ScalarType getScalarType()
{
    void Error_getScalarType_UsedWithNonPvDataType();
    Error_getScalarType_UsedWithNonPvDataType();
    return epics::pvData::pvString;
}

template<> epics::pvData::ScalarType getScalarType<int8_t>();
template<> epics::pvData::ScalarType getScalarType<int16_t>();
template<> epics::pvData::ScalarType getScalarType<int32_t>();
template<> epics::pvData::ScalarType getScalarType<int64_t>();
template<> epics::pvData::ScalarType getScalarType<float>();
template<> epics::pvData::ScalarType getScalarType<double>();
template<> epics::pvData::ScalarType getScalarType<std::string>();


/**
 * Copies from a pvData PVStructure to an STL vector.
 *
 * @param  scalarArray   the vector of scalars to copy to
 * @param  pvStructure   the  PVStructure to copy from
 */
template<typename T> void copyToScalarArray(std::vector<T> & scalarArray,
    epics::pvData::PVStructure::shared_pointer & pvArgument, const char * name)
{
    epics::pvData::ScalarType st = getScalarType<T>();
    std::tr1::static_pointer_cast<epics::pvData::PVValueArray<T> >(
        pvArgument->getScalarArrayField(name, st))->put(0, scalarArray.size(), &scalarArray[0], 0);
}

/**
 * Copies from a pvData PVStructure to an STL vector.
 *
 * @param  scalarArray   the vector of scalars to copy from
 * @param  pvStructure   the  PVStructure to copy to
 */
template<typename T> void copyFromScalarArray(std::vector<T> & scalarArray,
    const epics::pvData::PVStructure::shared_pointer & pvArgument, const char * name)
{
    epics::pvData::ScalarType st = getScalarType<T>();
    epics::pvData::PVArrayData<T> arrayData;
    epics::pvData::PVValueArray<T> * pvValues =
        (epics::pvData::PVValueArray<T> *)pvArgument->getScalarArrayField(name, st);
    pvValues->get(0, pvValues->getLength(), &arrayData);
    scalarArray.resize(pvValues->getLength());
    std::copy(arrayData.data, arrayData.data + pvValues->getLength(), scalarArray.begin());
}

/**
 * Converts an object of type T to string, where T should be a pointer or smart pointer
 * to an object which has an apropriate toString function.
 *
 * @param  t  the object of type T to be converted
 * @return    the result of the string conversion
 */
template<class T> std::string toString(T st)
{
    std::string s;
    st->toString(&s);
    return s;
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
