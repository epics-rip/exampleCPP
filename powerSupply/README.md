# examplePowerSupply


This is an example of creating a service that requires a somewhat complicated top level PVStructure.

## Building

If a proper RELEASE.local file exists one or two directory levels above **examplePowerSupply**
then just type:

    make

It can also be built by:

    cp configure/ExampleRELEASE.local configure/RELEASE.local
    edit configure/RELEASE.local
    make


## To start the examplePowerSupply as part of a V3 IOC

    mrk> pwd
    /home/epicsv4/master/exampleCPP/examplePowerSupply/iocBoot/examplePowerSupply
    mrk> ../../bin/linux-x86_64/examplePowerSupply st.cmd 

## To start the examplePowerSupply as a standalone main

    mrk> pwd
    /home/epicsv4/master/exampleCPP/examplePowerSupply
    mrk> bin/linux-x86_64/examplePowerSupplyMain

## After starting examplePowerSupply

    mrk> pwd
    /home/epicsv4/master/exampleCPP/examplePowerSupply
    mrk> bin/linux-x86_64/examplePowerSupplyClient
