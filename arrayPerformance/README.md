# arrayPerformance

This example implements main programs that demonstrate performance
of large arrays and can also be used to check for memory leaks.
Checking for memory leaks can be accomplished by running the programs with valgrind
or some other memory check program.

## Brief Summary

The programs are:

* arrayPerformanceMain   
    This is server and also a configurable number of longArrayMonitor clients.   
    The clients can use either the local or remote providers.     
    The monitor code is the same code that is used by longArrayMonitorMain.
* longArrayMonitorMain   
    Remote client that monitors the array served by arrayPerformanceMain.
* longArrayGetMain    
    Remote client that uses channelGet to access the array served by arrayPerformanceMain.
* longArrayPutMain    
     Remote client that uses channelPut to access the array served by arrayPerformanceMain.


## Building

If a proper RELEASE.local file exists one or two directory levels above **arrayPerformance**
then just type:

    make

It can also be built by:

    cp configure/ExampleRELEASE.local configure/RELEASE.local
    edit configure/RELEASE.local
    make

