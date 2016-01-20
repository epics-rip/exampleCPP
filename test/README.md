# test

A regression test for pvDatabaseCPP and pvaClientCPP.
This is activated via:

    make runtest


## Building

If a proper RELEASE.local file exists one or two directory levels above **test**
then just type:

    make

It can also be built by:

    cp configure/ExampleRELEASE.local configure/RELEASE.local
    edit configure/RELEASE.local
    make


