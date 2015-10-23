exampleCPP
==========

This project provides:

* An EPICS IOC that has both EPICS V3 DBRecords, V4 PVRecords, and pvaSrv for accessing the DBRecords.
* PvaClient examples.
* A test that can be run via "make runtests"

The EPICS IOC is also available for running the examples in pvaClientCPP and pvaClientJava.

Building
--------

If a proper RELEASE.local file exists one directory level above pvExampleTestCPP
then just type:

    make

If RELEASE.local does not exist then look at <b>configure/RELEASE</b>
for directions for how to build.

To run the test
---------------

make runtests

To start the database
------------

    mrk> pwd
    mrk> ../../bin/linux-x86_64/exampleDatabase st.cmd 

Status
------

* Readyfor release 4.5.0

HelloWorld
----------

HelloWorld is the classic hello world idea using channelRPC, implemented as a server and a client.
The client sends an argument "your name" to the server, which replies just 
saying "hello <that name>". The server is built on top of the RPCSever framework 
in pvAccessCPP.

Start with its HELLOWORLD_README.txt. 

ChannelArchiver Service
----------------------- 

The ChannelArchiver Service is an EPICS V4 client and server which can be used
to query the data archived by an EPICS Channel Archiver. It includes a bash
script, gethist, which can parse a wide range of dates and times and process
the corresponding data. However the service can be queried by any client
issuing a pvAccess RPC request, such as eget. Requests and responses use EPICS V4
normative types. Requests are in the form of an NTURI normative type and the results are returned as an NTTable. 

Start with its CHANNEL_ARCHIVER_SERVICE__README.txt


