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
    std::cout << toString(MYArchiverTable("MYArchiverTable", *getFieldCreate())) << std::endl;
    std::cout << toString(MYArchiverQuery("MYArchiverQuery", *getFieldCreate())) << std::endl;
    return 0;
}
