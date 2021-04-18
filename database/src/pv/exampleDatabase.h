/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 * @date 2013.07.24
 */
#ifndef EXAMPLEDATABASE_H
#define EXAMPLEDATABASE_H

#include <pv/pvDatabase.h>
#include <pv/pvdbcrScalarRecord.h>
#include <pv/pvdbcrScalarArrayRecord.h>
#include <pv/pvdbcrAddRecord.h>
#include <pv/pvdbcrRemoveRecord.h>
#include <pv/pvdbcrProcessRecord.h>
#include <pv/pvdbcrTraceRecord.h>
#include <powerSupply/powerSupplyRecord.h>
#include <linkRecord/getLinkScalarRecord.h>
#include <linkRecord/getLinkScalarArrayRecord.h>
#include <linkRecord/putLinkScalarRecord.h>
#include <linkRecord/putLinkScalarArrayRecord.h>
#include <shareLib.h>

namespace epics { namespace exampleCPP { namespace database {

class epicsShareClass  ExampleDatabase{
public:
    static void create();
};


}}}

#endif  /* EXAMPLEDATABASE_H */
