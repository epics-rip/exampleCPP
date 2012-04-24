#ifndef CHANNELARCHIVERSERVICE_TYPES_HPP
#define CHANNELARCHIVERSERVICE_TYPES_HPP

/* types.hpp - template helpers for pvData */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS exampleCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

/* Map Type to pvData Type Identifier through specialization */

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
    ((epics::pvData::PVValueArray<T> *)pvArgument->getScalarArrayField(name, st))->put(0, x.size(), &x[0], 0);
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



epics::pvData::StructureConstPtr ArchiverQuery(const char * name, epics::pvData::FieldCreate & factory)
{
    std::vector<epics::pvData::FieldConstPtr> fields;

    fields.push_back(factory.createScalar("name", epics::pvData::pvString));
    fields.push_back(factory.createScalar("t0secPastEpoch", epics::pvData::pvLong));
    fields.push_back(factory.createScalar("t0nsec", epics::pvData::pvInt));
    fields.push_back(factory.createScalar("t1secPastEpoch", epics::pvData::pvLong));
    fields.push_back(factory.createScalar("t1nsec", epics::pvData::pvInt));

    return factory.createStructure(
        name, fields.size(), copyToArray(fields));
}

epics::pvData::StructureConstPtr ArchiverTable(const char * name, epics::pvData::FieldCreate & factory)
{
    std::vector<epics::pvData::FieldConstPtr> fields;
    fields.push_back(factory.createScalarArray("labels", epics::pvData::pvString));
    fields.push_back(factory.createScalarArray("value", epics::pvData::pvDouble));
    fields.push_back(factory.createScalarArray("secPastEpoch", epics::pvData::pvLong));
    fields.push_back(factory.createScalarArray("nsec", epics::pvData::pvInt));
    fields.push_back(factory.createScalarArray("date", epics::pvData::pvString));
    fields.push_back(factory.createScalarArray("status", epics::pvData::pvInt));
    fields.push_back(factory.createScalarArray("severity", epics::pvData::pvInt));
    return factory.createStructure(name, fields.size(), copyToArray(fields));
}

}

#endif
