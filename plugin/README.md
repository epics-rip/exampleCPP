# plugin

Creates a PVRecord that calls support code that is implemented by pvDatabaseCPP

## Building

Read exampleCPP/documentation/exampleCPP.html.


## To start the server as part of a V3 IOC

    mrk> pwd
    /home/epicsv4/masterCPP/exampleCPP/plugin/iocBoot/support
    mrk> ../../bin/$EPICS_HOST_ARCH/support st.cmd

## To start testing

    mrk> pwd
    /home/epicsv4/masterCPP/exampleCPP/plugin/testClient/scripts
    mrk> ./configAll
    mrk>./exampleDeadband
    recordName?
    PVRpluginDouble
    deadband?
    2




    

