# database

This example implements main programs that demonstrate performance
of large arrays and can also be used to check for memory leaks.
Checking for memory leaks can be accomplished by running the programs with valgrind
or some other memory check program.

## Brief Summary

Provides an IOC with a combination of V3 DBRecords, PVRecords, and also starts pvaSrv.


## Building

If a proper RELEASE.local file exists one or two directory levels above **database**
then just type:

    make

It can also be built by:

    cp configure/ExampleRELEASE.local configure/RELEASE.local
    edit configure/RELEASE.local
    make

