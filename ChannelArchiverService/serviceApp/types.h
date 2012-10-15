#ifndef CHANNELARCHIVERSERVICE_TYPES_H
#define CHANNELARCHIVERSERVICE_TYPES_H

/* types.h - template helpers for pvData */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS exampleCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

/* Map Type to pvData Type Identifier through specialization */

#include "pv/pvIntrospect.h"
#include "pv/pvData.h"

namespace {

template<class T> epics::pvData::ScalarType getScalarType()
{
    void Error_getScalarType_UsedWithNonPvDataType();
    Error_getScalarType_UsedWithNonPvDataType();
    return epics::pvData::pvString;
}

template<> epics::pvData::ScalarType getScalarType<int8_t>()  { return epics::pvData::pvByte; }
template<> epics::pvData::ScalarType getScalarType<int16_t>() { return epics::pvData::pvShort; }
template<> epics::pvData::ScalarType getScalarType<int32_t>() { return epics::pvData::pvInt; }
template<> epics::pvData::ScalarType getScalarType<int64_t>() { return epics::pvData::pvLong; }
template<> epics::pvData::ScalarType getScalarType<float>()   { return epics::pvData::pvFloat; }
template<> epics::pvData::ScalarType getScalarType<double>()  { return epics::pvData::pvDouble; }
template<> epics::pvData::ScalarType getScalarType<std::string>()  { return epics::pvData::pvString; }

/* copy from STL vector to pvData */

template<typename T> void copyToScalarArray(
    std::vector<T> & x, epics::pvData::PVStructure::shared_pointer & pvArgument, const char * name)
{
    epics::pvData::ScalarType st = getScalarType<T>();
    std::tr1::static_pointer_cast<epics::pvData::PVValueArray<T> >(pvArgument->getScalarArrayField(name, st))->put(0, x.size(), &x[0], 0);
}

template<typename T> void copyFromScalarArray(
    std::vector<T> & x, const epics::pvData::PVStructure::shared_pointer & pvArgument, const char * name)
{
    epics::pvData::ScalarType st = getScalarType<T>();
    epics::pvData::PVArrayData<T> arrayData;
    epics::pvData::PVValueArray<T> * pvValues = (epics::pvData::PVValueArray<T> *)pvArgument->getScalarArrayField(name, st);
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

const std::string ntTableStr = "uri:ev4:nt/2012/pwd:NTTable";
const std::string ntURIStr = "uri:ev4:nt/2012/pwd:NTURI";

const std::string nameStr  = "name";
const std::string startStr = "start";
const std::string endStr   = "end";
const std::string countStr = "count";


epics::pvData::StructureConstPtr ArchiverQuery(epics::pvData::FieldCreate & factory)
{
    using namespace epics::pvData;

    FieldConstPtrArray fields;
    StringArray names;

    fields.push_back(factory.createScalar(epics::pvData::pvString));

    fields.push_back(factory.createScalar(epics::pvData::pvString));
    fields.push_back(factory.createScalar(epics::pvData::pvString));

    names.push_back(nameStr);
    names.push_back(startStr);
    names.push_back(endStr);

    return factory.createStructure(names, fields);
}

epics::pvData::StructureConstPtr ArchiverTableValues(epics::pvData::FieldCreate & factory)
{
    using namespace epics::pvData;
    
    FieldConstPtrArray fields;
    StringArray names;

    names.push_back("value");
    names.push_back("secPastEpoch");
    names.push_back("nsec");
    names.push_back("status");
    names.push_back("severity");

    fields.push_back(factory.createScalarArray(epics::pvData::pvDouble));
    fields.push_back(factory.createScalarArray(epics::pvData::pvLong));
    fields.push_back(factory.createScalarArray(epics::pvData::pvInt));
    fields.push_back(factory.createScalarArray(epics::pvData::pvInt));
    fields.push_back(factory.createScalarArray(epics::pvData::pvInt));

    return factory.createStructure(names, fields);
}

epics::pvData::StructureConstPtr ArchiverTable(epics::pvData::FieldCreate & factory)
{
    using namespace epics::pvData;
    
    FieldConstPtrArray fields;
    StringArray names;

    names.push_back("labels");
    names.push_back("value");

    fields.push_back(factory.createScalarArray(epics::pvData::pvString));
    fields.push_back(ArchiverTableValues(factory));

    return factory.createStructure("uri:ev4:nt/2012/pwd:NTTable", names, fields);
}



}

#endif
