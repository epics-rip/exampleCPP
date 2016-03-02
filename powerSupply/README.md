# examplePowerSupply


This is an example of creating a service that requires a somewhat complicated top level PVStructure.

## Building

If a proper RELEASE.local file exists two directory levels above **examplePowerSupply**,
or in **../configure/RELEASE.local** then just type:

    make

It can also be built by:

    cp configure/ExampleRELEASE.local configure/RELEASE.local
    edit file configure/RELEASE.local
    make

In **configure/RELEASE.local** it may only be necessary to change the definitions
of **EPICS4_DIR** and **EPICS_BASE**.


## To start the examplePowerSupply as part of a V3 IOC

    mrk> pwd
    /home/epicsv4/master/exampleCPP/examplePowerSupply/iocBoot/examplePowerSupply
    mrk> ../../bin/$EPICS_HOST_ARCH/examplePowerSupply st.cmd 

## To start the examplePowerSupply as a standalone main

    mrk> pwd
    /home/epicsv4/master/exampleCPP/examplePowerSupply
    mrk> bin/$EPICS_HOST_ARCH/examplePowerSupplyMain

## After starting examplePowerSupply

    mrk> pwd
    /home/epicsv4/master/exampleCPP/examplePowerSupply
    mrk> bin/$EPICS_HOST_ARCH/examplePowerSupplyClient
