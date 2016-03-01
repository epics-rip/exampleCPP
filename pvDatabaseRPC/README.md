# pvDatabaseRPC

An example of a pvDatabase PVRecord which also supports RPC services.

## Building

If a proper RELEASE.local file exists one or two directory levels above
**pvDatabaseRPC**
then just type:

    make

It can also be built by:

    cp configure/ExampleRELEASE.local configure/RELEASE.local
    edit file configure/RELEASE.local
    make

In **configure/RELEASE.local** it may only be necessary to change the definitions
of **EPICS4_DIR** and **EPICS_BASE**.



