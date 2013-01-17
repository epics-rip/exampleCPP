HELLO_WORLD_SERVICE__README.txt

This is the README file of the HelloWorld service.

Auth: Dave Hickin, 17-Jan-2013 (david.hickin@diamond.ac.uk)


SUMMARY
-------

HelloWorld is a minimum client and server in the EPICS v4 services framework,
intended to show the basic programming required to implement a service that
just takes an argument and returns a string.


EXAMPLE
-------
The following is an example of running the client, giving the argument "Jessica"

          % ./helloClientRunner Jessica
          Hello Jessica  


FILES THAT COMPRISE THE HELLOWORLD EXAMPLE
------------------------------------------
HelloService*.*               C++ source code of the server side of the example
HelloClient.cpp               C++ source code of the client side of the example
helloClientRunner             A unix (bash) executable script to run the client 
helloServerRunner             A unix (bash) executable script to run the server 


PREREQUISITES
-------------

1. EPICS V3 Base          - for a large number of EPICS types
2. pvDataCPP              - for PVData 
3. pvAccessCPP            - for PVAccess


SETUP
-----

1. EPICS version 3and the EPICS V4 prerequisites (pvData and pvAccess) should
be built.

2. The HelloWorld service needs to know the location of these through the
following macros:

EPICS_BASE
PVDATA
PVACCESS

This can be achieved by creating a RELEASE.local file in the configure
directory of the ChannelArchiverService directory, e.g. if your EPICS 3 base
and extensions code is in /epics/R3.14.11 and your base directories of the
EPICS 4 prerequisites you wish to link to are in /epics4 then it could be of
the form 

EPICS_BASE=/epics/R3.14.11/base
PVDATA=/epics4/pvDataCPP
PVACCESS=/epics4/pvAccessCPP

3. The HelloWorld server and client are built by the command "make" issued from
the HelloWorld directory.


EXECUTION
---------
This section describes how you start server and client sides of the helloWorld
Service. Start the server side first.

To start the HelloWorld server
-------------------------------
 * cd to the directory containing HelloWorld

   E.g. % cd ~/Development/epicsV4/workspace_hg_beta2/exampleCPP/HelloWorld
  
 * Start the server in one terminal 

   E.g. % ./helloServerRunner
  
 * Terminate the server with a SIGTERM (like CTRL-C in its process window)
   - after you've tested it with the client below of course.
   
   
To run a HelloWorld Client
---------------------------
 * In another window from the server:
 
 * cd to the directory containing HelloWorld (both client and server are in the
   same directory for demo purposes)
   
   E.g. % cd ~/Development/epicsV4/workspace_hg_beta2/exampleCPP/HelloWorld
 
 * Execute the client side demo, optionally giving your name!
 
   E.g. % ./helloClientRunner Dave
        Hello Dave


Querying the service using eget
-------------------------------
The service can be queried using the eget utility with the -q flag.
With the server started, assuming eget is in your path:

 * Execute the eget utility:

   eget -q -s helloService -a "personsname=Dave"


Remark
------
This is the C++ version of the equivalent Java service (exampleJava) written by
Greg White. 
