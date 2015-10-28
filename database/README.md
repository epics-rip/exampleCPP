# database

This example provides an EPICS IOC that has both EPICS V3 DBRecords, V4 PVRecords, and pvaSrv for accessing the DBRecords.

It can also create a standalone main that has a set of V4 PVRecords.


## Building

If a proper RELEASE.local file exists one or two directory levels above **database**
then just type:

    make

It can also be built by:

    cp configure/ExampleRELEASE.local configure/RELEASE.local
    edit configure/RELEASE.local
    make


## To start the database as part of a V3 IOC

    mrk> pwd
    /home/epicsv4/master/exampleCPP/database/iocBoot/exampleDatabase
    mrk> ../../bin/linux-x86_64/exampleDatabase st.cmd 

## To start the database as a standalone main

    mrk> pwd
    /home/epicsv4/master/exampleCPP/database
    mrk> bin/linux-x86_64/exampleDatabaseMain

## exampleCPP/src

This directory has the following files:

### exampleHello.h and exampleHello.cpp
   
Code for an example that is accessed via channelPutGet.

### exampleHelloRPC.h and exampleHelloRPC.cpp
   
Code for an example that is accesed via channelRPC.

### exampleDatabase.h and exampleDatabase.cpp 
  
Code that creates many PVRecords.    
Most are soft records but also exampleHello and exampleHelloRPC.

### exampleDatabaseInclude.dbd and exampleDatabaseRegister.cpp
 
Code that allows the PVRecords to be part of a V3 IOC.

### exampleDatabaseMain.cpp

Code that allows the PVRecords to be available via a standalone main program.

## exampleCPP/ioc

Code that allows the database to be part of a V3 IOC.

### exampleCPP/ioc/Db


Files for creating V3 DBRecords

### exampleCPP/ioc/src

* exampleDatabaseMain.cpp   
Standard file for a V3 IOC.
* exampleDatabaseInclude.dbd    
Standard V3 dbd file with additional includes for PVRecords and pvaSrv and local pvAccess.

### exampleCPP/ioc/iocBoot/exampleDatabase

Support for starting V3 IOC that also has support for pvaSrv and PVRecords.


