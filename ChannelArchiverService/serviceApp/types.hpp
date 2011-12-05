template<typename T> T * copyToArray(std::vector<T> & fields)
{
    T * fields2 = new T[fields.size()];
    std::copy(fields.begin(), fields.end(), fields2);
    return fields2;
}

StructureConstPtr timeStampStructure(const char * name, FieldCreate & factory)
{
    std::vector<FieldConstPtr> fields;
    fields.push_back(factory.createScalar("secPastEpoch", epics::pvData::pvLong));
    fields.push_back(factory.createScalar("nsec", epics::pvData::pvInt));
    return factory.createStructure(
        name, fields.size(), copyToArray(fields));
}

StructureConstPtr MyNTDouble(FieldCreate & factory)
{
    std::vector<FieldConstPtr> fields;
    fields.push_back(factory.createScalarArray("values", epics::pvData::pvDouble));
    fields.push_back(factory.createScalarArray("secPastEpoch", epics::pvData::pvLong));
    fields.push_back(factory.createScalarArray("nsec", epics::pvData::pvDouble));
    return factory.createStructure(
        "ArchiveResult", fields.size(), copyToArray(fields));
}

StructureConstPtr ArchiverStructure(FieldCreate & factory)
{
    std::vector<FieldConstPtr> fields;
    fields.push_back(factory.createScalarArray("values", epics::pvData::pvDouble));
    fields.push_back(factory.createScalarArray("secPastEpoch", epics::pvData::pvLong));
    fields.push_back(factory.createScalarArray("nsec", epics::pvData::pvInt));
    return factory.createStructure(
        "ArchiveResult", fields.size(), copyToArray(fields));
}

StructureConstPtr ArchiverClientStructure(FieldCreate & factory)
{
    std::vector<FieldConstPtr> fields;
    fields.push_back(factory.createScalar("index", epics::pvData::pvString));
    fields.push_back(factory.createScalarArray("names", epics::pvData::pvString));
    
    fields.push_back(timeStampStructure("t0", factory));
    fields.push_back(timeStampStructure("t1", factory));

    fields.push_back(factory.createScalar("how", epics::pvData::pvLong));
    fields.push_back(factory.createScalar("count", epics::pvData::pvLong));
    return factory.createStructure(
        "ArchiverValues", fields.size(), copyToArray(fields));
}
