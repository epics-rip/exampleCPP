CHANNEL_ARCHIVER_SERVICE__README.txt

This is the README file of the ChannelArchiver service.

Auth: Dave Hickin, 20-Mar-2012 (david.hickin@diamond.ac.uk)


SUMMARY
-------

The ChannelArchiver service is an EPICS V4 client and server which be used to query a 
the data archived by an EPICS Channel Archiver. It includes a bash script, gethist,
which can parse a wide range of dates and times and return the corresponding data.



EXAMPLES
--------

The server is run as follows

    % ./start_server

start_server calls the ArchiverService executable (which should be
bin/$EPICS_HOST_ARCH/ArchiverServiceRPC) supplying two arguments. The first is the
service name, the second the location of the index file. Should you wish to change
either then these arguments should be modified.   


The following are examples of running the client:

0) Use the -h or --help options to get help

    % ./gethist -h
    % ./gethist --help

1) Retrieve archived data for the channel "fred" with the default start and end times, which
are 1 hour ago and now.

    % ./gethist fred

2) Retrieve archived data for the channel "janet" between Jan 1 1990 and now.

    % ./gethist -s "Jan 1 1990" -e "now" -f out.txt -x janet

3) Long options are also supported.

    % ./gethist --start "Jan 1 1990" --end "now" --file out.txt --scientific janet

4) The options without argument can preceed another:

    % ./gethist -xn fred

5) A wide variety of date and time formats are supported (those supported by date -d). The
following are all valid:

    % ./gethist -s "31 Jan 1990" janet
    % ./gethist -s "Jan 31 1990" janet
    % ./gethist -s "January 31 1990" janet
    % ./gethist -s 1990-01-31 janet
    % ./gethist -s 1990-01-31Z janet

You can also supply a time, for example

    % ./gethist -s "30 Jun 2001 17:34" fred
    % ./gethist -s "30 Jun 2001 5:34 p.m." fred
    % ./gethist -s "30 Jun 2001 5:34 p.m." fred
    % ./gethist -s "30 Jun 2001 17:34:56" fred
    % ./gethist -s "30 Jun 2001 17:34 56 seconds" fred
    % ./gethist -s "30 Jun 2001 5:34 p.m. 56 seconds" fred

The following are also valid

    % ./gethist -s  yesterday      -e  now janet
    % ./gethist -s "last week"     -e  today janet
    % ./gethist -s "last year"     -e "last month" janet
    % ./gethist -s "5 minutes ago" -e "10 seconds ago" janet
    % ./gethist -s "2 weeks ago"   -e "1 hour ago" janet
    % ./gethist -s "4 years ago"   -e "3 months ago" janet



FILES THAT COMPRISE THE CHANNEL ARCHIVER EXAMPLE
------------------------------------------

ArchiverServiceRPC.h/cpp                  C++ source code of the server side of the example
ArchiverServiceRPCMain.cpp                C++ source code of the server side of the example
ArchiverClient.cpp                        C++ source code of the client side of the example
ArchiverClientResponseHandler.h/cpp       C++ source code of the client for handling the response from server 
ServiceClient.h/cpp                       C++ source code of the client for handling the response from server
showtypes.cpp                             C++ source code of application that shows request and response types
types.h                                   C++ source code of types common to client and server

start_server                              bash script to start server
gethist                                   bash script to query service

data                                      directory containing sample Channel Archiver data

PREREQUISITES
-------------

EPICS V4 components:
1. common  - for source/pvCommon_setup.bash
2. pvAccessJava - for PVAccess 
3. pvDataJava   - for PVData 
4. pvIOCJava    - for JavaIOC rpc
5. pvService    - for xmls that define a service
6. exampleJava      - for the example java classes and config files.

1. EPICS 3 Base          - for a large number of EPICS types
2. pvDataCPP             - for PVData 
3. pvAccessCPP           - for PVAccess
4. pvIOCCPP              - for CPP IOC RPC
5. The Channel Archiver  - for Channel Archiver access and types



EPICS_BASE=/dls_sw/epics/R3.14.11/base

PVDATA=$(TOP)/../../pvDataCPP
PVACCESS=$(TOP)/../../pvAccessCPP
PVIOC=$(TOP)/../../pvIOCCPP

ARCHIVER=/dls_sw/work/R3.14.11/support/ChannelArchiver


SETUP
-----

1. EPICS version 3, the Channel Archiver and the EPICS V4 prerequisites (pvData, pvAccess and pvIOC)
should be built.
2. The Channel Archiver service needs to know the location of these through the following macros:
EPICS_BASE
PVDATA
PVACCESS
PVIOC
ARCHIVER 

This can be achieved by creating a RELEASE.local file in the configure directory of the
ChannelArchiverService directory, e.g. if your EPICS 3 base and extensions code is in
/epics/R3.14.11 and your base directories of the EPICS 4 prerequisites you wish to link to
are in /epics4 then it could be of the form 

EPICS_BASE=/epics/R3.14.11/base
PVDATA=/epics4/pvDataCPP
PVACCESS=/epics4/pvAccessCPP
PVIOC=/epics4/pvIOCCPP
ARCHIVER=/epics/R3.14.11/extensions

ARCHIVER should point to the directory containing src/ChannelArchiver. This will depend on
the layout of your EPICS installation.

3. The Archiver server and client are built by the command "make" issued from the
ChannelArchiverService directory.

4. The start_server script starts the ChannelArchiverService server using the data supplied in the
directory "data". Should you wish to use other data you need have data in the ususal format created by
the ChannelArchiver application, including the index file, and will need to modify the start_server
script to point to this as described below (see the ChannelArchiver manual, for example, 
http://www.slac.stanford.edu/grp/lcls/controls/global/sw/epics/extensions/ChannelArchiver/manual.pdf).


EXECUTION
---------
This section describes how you start server and client sides of the Channel Archiver Service. 
Start the server side first.

To start the Channel Archiver server
-------------------------------
 * cd to the directory containing start_server

   E.g. % cd ~/Development/epicsV4/exampleCPP/ChannelArchiverService

*  start_server calls the ArchiverService executable (which should be
   bin/$EPICS_HOST_ARCH/ArchiverServiceRPC) supplying two arguments. The first is the
   service name, the second the location of the index file. Should you wish to change
   either then these arguments should be modified.  

 * Start the server in one terminal 

   E.g. % ./start_server
  
 * Terminate the server with a SIGTERM (like CTRL-C in its process window) 
   or by typing "exit" - after you've tested it with the client below of course.
   
   
To run a Channel Archiver Service Client
---------------------------

 * In another window from the server:
 
 * cd to the directory containing gethist (both client and server are in the same dir 
   for demo purposes)
   
   E.g. % cd ~/Development/epicsV4/exampleCPP/ChannelArchiverService
 
 * Execute the gethist as described above
 
   E.g. % ./gethist --start "Jan 1 1990" --end "now" --file out.txt --scientific janet 
 
By default the server queries the supplied data which has two channels ("fred" and "janet")
and which covers the period on the 21st September 2005 between 17:49:57 and 19:33:16.


