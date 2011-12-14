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
    /*
    std::cout << toString(MYScalarTransposedStatisticsArray("MYScalarTransposedStatisticsArray", pvDouble, *getFieldCreate())) << std::endl;
    std::cout << toString(MYScalarTransposedArray("MYScalarTransposedArray", pvDouble, *getFieldCreate())) << std::endl;
    */
    std::cout << toString(MYArchiverTable("MYArchiverTable", *getFieldCreate(), 0)) << std::endl;
    std::cout << toString(MYArchiverTable("MYArchiverTableStats", *getFieldCreate(), 1)) << std::endl;
    std::cout << toString(ArchiverClientStructure("ArchiverQuery", *getFieldCreate())) << std::endl;
    return 0;
}
