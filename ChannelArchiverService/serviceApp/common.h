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
/* Map Type to pvData Type Identifier through specialization */

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

/* copy from STL vector to pvData */

template<typename T> void copyToScalarArray(
    std::vector<T> & x, epics::pvData::PVStructure::shared_pointer & pvArgument, const char * name)
{
    epics::pvData::ScalarType st = getScalarType<T>();
    std::tr1::static_pointer_cast<epics::pvData::PVValueArray<T> >(
        pvArgument->getScalarArrayField(name, st))->put(0, x.size(), &x[0], 0);
}

template<typename T> void copyFromScalarArray(
    std::vector<T> & x, const epics::pvData::PVStructure::shared_pointer & pvArgument, const char * name)
{
    epics::pvData::ScalarType st = getScalarType<T>();
    epics::pvData::PVArrayData<T> arrayData;
    epics::pvData::PVValueArray<T> * pvValues =
        (epics::pvData::PVValueArray<T> *)pvArgument->getScalarArrayField(name, st);
    pvValues->get(0, pvValues->getLength(), &arrayData);
    x.resize(pvValues->getLength());
    std::copy(arrayData.data, arrayData.data + pvValues->getLength(), x.begin());
}

template<typename T> T * copyToArray(std::vector<T> & fields)
{
    T * fields2 = new T[fields.size()];
    std::copy(fields.begin(), fields.end(), fields2);
    return fields2;
}

template<class T> std::string toString(T st)
{
    std::string s;
    st->toString(&s);
    return s;
}

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


epics::pvData::StructureConstPtr ArchiverQuery(epics::pvData::FieldCreate & factory,
    const std::vector<std::string> & queryFields);


epics::pvData::StructureConstPtr ArchiverRequest(epics::pvData::FieldCreate & factory,
    const std::vector<std::string> & queryFields);


epics::pvData::StructureConstPtr ArchiverTableValues(epics::pvData::FieldCreate & factory);


epics::pvData::StructureConstPtr ArchiverTable(epics::pvData::FieldCreate & factory);

}

}

#endif
