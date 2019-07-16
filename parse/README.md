# parse

Provides a set of examples for put and putGet that use JASON syntax.

In order to run the examples, **database** must also be built and then the IOC database must be started as follows:

    mrk> pwd
    /home/epicsv4/master/exampleCPP/database/iocBoot/exampleDatabase
    mrk> ../../bin/$EPICS_HOST_ARCH/exampleDatabase st.cmd



## Building

Read exampleCPP/documentation/exampleCPP.html.

## scripts

directory client/scripts has a number of examples.

### client/scripts/exampleScalarArray

shows JSON syntax for scalar arrays.


### client/scripts/exampleUnion

shows JSON syntax for restricted union field

### client/scripts/exampleBigRecord

shows JSON syntax for accessing subfields.

