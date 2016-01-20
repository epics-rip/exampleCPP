# exampleLink

This example implements a PVRecord that accesses another PVRecord.

## Building

If a proper RELEASE.local file exists one or two directory levels above **exampleLink**
then just type:

    make

It can also be built by:

    cp configure/ExampleRELEASE.local configure/RELEASE.local
    edit configure/RELEASE.local
    make

## To start the exampleLink as part of a V3 IOC

    mrk> pwd
    /home/epicsv4/master/exampleLink/exampleLink/iocBoot/exampleLink
    mrk> ../../bin/linux-x86_64/exampleLink st.local

or
 
mrk> pwd
    /home/epicsv4/master/exampleLink/exampleLink/iocBoot/exampleLink
    mrk> ../../bin/linux-x86_64/exampleLink st.remote


