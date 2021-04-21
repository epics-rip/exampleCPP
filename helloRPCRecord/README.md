# powerSupply


This is an example of creating a service that requires a somewhat complicated top level PVStructure.

The example record has the structure:

    powerSupply
    structure 
        alarm_t alarm
            int severity
            int status
            string message
        time_t timeStamp
            long secondsPastEpoch
            int nanoseconds
            int userTag
        structure power
            double value
        structure voltage
            double value
        structure current
            double value

The record is meant to be accessed via a channelPutGet request.
The client can provide values for power.value and voltage.value.
The process routine computes current.value.

## Building

Read exampleCPP/documentation/exampleCPP.html.


## To start the powerSupply as part of a V3 IOC

    mrk> pwd
    /home/epicsv4/master/exampleCPP/powerSupply/iocBoot/powerSupply
    mrk> ../../bin/$EPICS_HOST_ARCH/powerSupply st.cmd 

## To start the powerSupply as a standalone main

    mrk> pwd
    /home/epicsv4/master/exampleCPP/powerSupply
    mrk> bin/$EPICS_HOST_ARCH/powerSupplyMain

## After starting powerSupply

    mrk> pwd
    /home/epicsv4/master/exampleCPP/powerSupply
    mrk> bin/$EPICS_HOST_ARCH/powerSupplyClient

This client calls channelPutGet several times specifying different value for power and voltage.

The last call sets the voltage to 0, which results in process throwing an exception.

## powerSupplyMonitor

There is also an example to monitor changes in the powerSupply.

    mrk> pwd
    /home/epicsv4/master/exampleCPP/powerSupply
    mrk> bin/$EPICS_HOST_ARCH/powerSupplyMonitor
