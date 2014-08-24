/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS exampleCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */


#include "common.h"


namespace epics
{

namespace channelArchiverService
{

const std::string ntTableStr = "uri:ev4:nt/2012/pwd:NTTable";
const std::string ntURIStr = "uri:ev4:nt/2012/pwd:NTURI";

const std::string nameStr  = "entity";
const std::string startStr = "starttime";
const std::string endStr   = "endtime";
const std::string countStr = "maxrecords";



epics::pvData::StructureConstPtr makeQueryStructure(epics::pvData::FieldCreate & factory,
    const std::vector<std::string> & queryFields)
{
    using namespace epics::pvData;

    FieldConstPtrArray fields;
    StringArray names;

    for (std::vector<std::string>::const_iterator it = queryFields.begin();
         it!= queryFields.end();
         ++it)
    {
        fields.push_back(factory.createScalar(epics::pvData::pvString));
        names.push_back(*it);
    }

    return factory.createStructure(names, fields);
}

epics::pvData::StructureConstPtr makeRequestStructure(epics::pvData::FieldCreate & factory,
    const std::vector<std::string> & queryFields)
{
    using namespace epics::pvData;

    FieldConstPtrArray fields;
    StringArray names;

    fields.push_back(factory.createScalar(epics::pvData::pvString));
    fields.push_back(makeQueryStructure(factory, queryFields));

    names.push_back("path");
    names.push_back("query");

    return factory.createStructure(ntURIStr, names, fields);
}


epics::pvData::StructureConstPtr makeValuesStructure(epics::pvData::FieldCreate & factory)
{
    using namespace epics::pvData;
    
    FieldConstPtrArray fields;
    StringArray names;

    names.push_back("value");
    names.push_back("secondsPastEpoch");
    names.push_back("nanoseconds");
    names.push_back("status");
    names.push_back("severity");

    fields.push_back(factory.createScalarArray(epics::pvData::pvDouble));
    fields.push_back(factory.createScalarArray(epics::pvData::pvLong));
    fields.push_back(factory.createScalarArray(epics::pvData::pvInt));
    fields.push_back(factory.createScalarArray(epics::pvData::pvInt));
    fields.push_back(factory.createScalarArray(epics::pvData::pvInt));

    return factory.createStructure(names, fields);
}

epics::pvData::StructureConstPtr makeArchiverResponseStructure(epics::pvData::FieldCreate & factory)
{
    using namespace epics::pvData;
    
    FieldConstPtrArray fields;
    StringArray names;

    names.push_back("labels");
    names.push_back("value");

    fields.push_back(factory.createScalarArray(epics::pvData::pvString));
    fields.push_back(makeValuesStructure(factory));

    return factory.createStructure(ntTableStr, names, fields);
}

}

}
