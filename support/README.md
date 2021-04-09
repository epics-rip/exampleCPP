# support

Creates a PVRecord that calls support code that is implemented by pvDatabaseCPP

## Building

Read exampleCPP/documentation/exampleCPP.html.


## To start the server as part of a V3 IOC

    mrk> pwd
    /home/epicsv4/masterCPP/exampleCPP
    mrk> bin/$EPICS_HOST_ARCH/supportMain

## monitor changed
    mrk> pvget -m -r "" -v  PVRsupportDouble PVRsupportUByte

## To start testing

    mrk> pwd
    /home/epicsv4/masterCPP/exampleCPP/support/testClient/scripts
    mrk> ./configAll

## Try various tests

    mrk> pvput PVRsupportUByte 14
    mrk> pvput PVRsupportDouble 22

    

