/* types.hpp - template helpers for pvData */

/* Map Type to pvData Type Identifier through specialization */

template<class T> ScalarType getScalarType()
{
    void Error_getScalarType_UsedWithNonPvDataType();
    Error_getScalarType_UsedWithNonPvDataType();
    return pvString;
}

template<> ScalarType getScalarType<int8_t>()  { return pvByte; }
template<> ScalarType getScalarType<int16_t>() { return pvShort; }
template<> ScalarType getScalarType<int32_t>() { return pvInt; }
template<> ScalarType getScalarType<int64_t>() { return pvLong; }
template<> ScalarType getScalarType<float>()   { return pvFloat; }
template<> ScalarType getScalarType<double>()  { return pvDouble; }
template<> ScalarType getScalarType<std::string>()  { return pvString; }

/* copy from STL vector to pvData */

template<typename T> void copyToScalarArray(
    std::vector<T> & x, PVStructure::shared_pointer & pvArgument, const char * name)
{
    ScalarType st = getScalarType<T>();
    ((PVValueArray<T> *)pvArgument->getScalarArrayField(name, st))->put(0, x.size(), &x[0], 0);
}

template<typename T> void copyFromScalarArray(
    std::vector<T> & x, const PVStructure::shared_pointer & pvArgument, const char * name)
{
    ScalarType st = getScalarType<T>();
    PVArrayData<T> arrayData;
    PVValueArray<T> * pvValues = (PVValueArray<T> *)pvArgument->getScalarArrayField(name, st);
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

StructureConstPtr MYArchiverQuery(const char * name, FieldCreate & factory)
{
    std::vector<FieldConstPtr> fields;

    fields.push_back(factory.createScalar("index", pvString));
    fields.push_back(factory.createScalar("name", pvString));
    fields.push_back(factory.createScalar("t0secPastEpoch", pvLong));
    fields.push_back(factory.createScalar("t0nsec", pvInt));
    fields.push_back(factory.createScalar("count", pvLong));

    return factory.createStructure(
        name, fields.size(), copyToArray(fields));
}

StructureConstPtr MYArchiverTable(const char * name, FieldCreate & factory)
{
    std::vector<FieldConstPtr> fields;
    fields.push_back(factory.createScalarArray("labels", pvString));
    fields.push_back(factory.createScalarArray("value", pvDouble));
    fields.push_back(factory.createScalarArray("secPastEpoch", pvLong));
    fields.push_back(factory.createScalarArray("nsec", pvInt));
    fields.push_back(factory.createScalarArray("date", pvString));
    fields.push_back(factory.createScalarArray("status", pvInt));
    fields.push_back(factory.createScalarArray("severity", pvInt));
    return factory.createStructure(name, fields.size(), copyToArray(fields));
}
