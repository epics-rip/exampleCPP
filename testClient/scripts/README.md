# exampleCPP/testClient/scripts

This contains tests for pvaClient methods:
**getDouble**, **putDouble**, **getString**, **putString**,
**getDoubleArray**, **putDoubleArray**, **getStringArray**, and **putStringArray**.

Before the test can be run the following database must be started:

    mrk> pwd
    /home/epicsv4/masterCPP/exampleCPP/database/iocBoot/exampleDatabase
    mrk> ../../bin/linux-x86_64/exampleDatabase st.cmd

To run the tests do the following in this directory:

    ./testAll &> temp
    diff temp testSuccess
    rm temp

You should not see any differences.




