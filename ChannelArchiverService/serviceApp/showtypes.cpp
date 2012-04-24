/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS exampleCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <iostream>
#include <pv/clientFactory.h>

#include <stdio.h>
#include <epicsStdlib.h>
#include <epicsGetopt.h>
#include <pv/logger.h>

#include <vector>
#include <string>

#include <pv/convert.h>
#include <pv/event.h>

#include <epicsTime.h>

using namespace std;
using namespace std::tr1;
using namespace epics::pvData;
using namespace epics::pvAccess;

#include "types.hpp"

int main()
{
    std::cout << toString(ArchiverTable("ArchiverTable", *getFieldCreate())) << std::endl;
    std::cout << toString(ArchiverQuery("ArchiverQuery", *getFieldCreate())) << std::endl;
    return 0;
}
