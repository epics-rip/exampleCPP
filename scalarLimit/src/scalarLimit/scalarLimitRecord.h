/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2021.05.03
 */
#ifndef SCALARLIMITRECORDCREATE_H
#define SCALARLIMITRECORDCREATE_H

#include <shareLib.h>

namespace epics { namespace scalarLimit {

class ScalarLimitRecord;
typedef std::tr1::shared_ptr<ScalarLimitRecord> ScalarLimitRecordPtr;

class epicsShareClass ScalarLimitRecordCreate
{
public:
    /**
     * @brief Create a record.
     *
     * @param recordName The record name.
     * @param scalarType The type for the value field
     * @param asLevel  The access security level.
     * @param asGroup  The access security group.
     */
     static void create(
        std::string const & recordName,std::string const &  scalarType,
        int asLevel=0,std::string const & asGroup = std::string("DEFAULT"));
};

}}

#endif  /* SCALARLIMITRECORDCREATE_H */
