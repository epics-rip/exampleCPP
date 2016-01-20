# exampleCPP

This project provides a set of examples of client and server code for pvAccess.
Each example can be built separately by building in the example subdirectory.
The complete set of examples, except for ChannelArchiverService can also be built by building in exampleCPP itself.

Each example can be used as a template for building other applications:

* Choose an example that is similar to the desired application.
* Copy the complete example to where the desired application should reside.
* Edit the new application.

## Building via exampleCPP

This is only a good idea if a proper **RELEASE.local** is present one directory level above **exampleCPP**.

Just type:

    make

An example of a proper *RELEASE.local* is:

    EPICS_BASE=/home/install/epics/base
    EPICS4_DIR=/home/epicsv4/master
    PVCOMMON=${EPICS4_DIR}/pvCommonCPP
    PVDATA=${EPICS4_DIR}/pvDataCPP
    PVACCESS=${EPICS4_DIR}/pvAccessCPP
    PVASRV=${EPICS4_DIR}/pvaSrv
    NORMATIVETYPES=${EPICS4_DIR}/normativeTypesCPP
    PVDATABASE=${EPICS4_DIR}/pvDatabaseCPP
    PVACLIENT=${EPICS4_DIR}/pvaClientCPP


## Building An Individual Example

If a proper **RELEASE.local** is present one directory level above **exampleCPP** then in the example directory just type:

    make

Each example has a **README.md** file. See it for how to build the example if a proper **RELEASE.local**
does not exist or if the example has special requirements.

## Brief summary of examples.

### database

Provides an EPICS IOC that has both EPICS V3 DBRecords, V4 PVRecords, and pvaSrv for accessing the DBRecords.

It can also create a standalone main that has a set of V4 PVRecords.


### exampleClient

Provides a set of examples that use pvaClientCPP.

In order to run the examples, **database** must also be built and then the IOC database must be started as follows:

    mrk> pwd
    /home/epicsv4/master/exampleCPP/database/iocBoot/exampleDatabase
    mrk> ../../bin/linux-x86_64/exampleDatabase st.cmd


### helloPutGet

Implements a PVRecord that is an example of a PVRecord intended to ba accessed via channelPutGet.
It is a simple HelloWorld example.

### helloRPC

This implements HelloWorld that is accessed via channelRPC.

### exampleRPCService

This is an example service that is accessed via PvAClientChannel::createChannelRPC.

### exampleLink

This implements a V3 IOC that has a PVRecord doubleArray and a PVRecord exampleLink that monitors changes to doubleArray. PVRecord exampleLink uses pvAccess client code to monitor for changes in doubleArray. It can use either provider local or pva to connect to doubleArray.


### powerSupply

This is an example of creating a PVRecord that uses a somewhat complicated top level PVStructure.
It simulates a power supply.

The example also has an example pvaClient for accessing the PVRecord.

### ChannelArchiverService

The ChannelArchiver Service is an EPICS V4 client and server which can be used
to query the data archived by an EPICS Channel Archiver. It includes a bash
script, gethist, which can parse a wide range of dates and times and process
the corresponding data. However the service can be queried by any client
issuing a pvAccess RPC request, such as eget. Requests and responses use EPICS V4
normative types. Requests are in the form of an NTURI normative type and the results are returned as an NTTable. 

### test

This is an example that tests pvDatabase and pvaClient.   
This example requires that **database** must also be built. 

It is meant to be used to execute:

    make runtests


When this is done it starts the example database and then executes various client tests.

###  arrayPerformance

This is an example that shows performance for an array of doubles.

arrayPerformanceMain implement a PVRecord that is a double array.
It has a process method with code that causes the array to be updated at selectable rates and sizes.!

It also has pvaClient examples that can get, put, and monitor the double array record.


