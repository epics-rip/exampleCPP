# helloPutGet

The example implements a simple service that has a top level pvStructure:

    structure
        structure argument
            string value
        structure result
            string value
            time_t timeStamp
                long secondsPastEpoch
                int nanoseconds
                int userTag


It is designed to be accessed via a channelPutGet request.
The client sets argument.value
When the record processes it sets result.value to "Hello " 
concatenated with argument.value.
Thus if the client sets argument.value equal to "World"
result.value will be "Hello World".
In addition the timeStamp is set to the time when process is called.</p>

## Building

Read exampleCPP/documentation/exampleCPP.html.


## To start the helloPutGet as part of a V3 IOC

    mrk> pwd
    /home/epicsv4/master/helloPutGet/database/iocBoot/helloPutGet
    mrk> ../../bin/$EPICS_HOST_ARCH/helloPutGet st.cmd 

## To start the helloPutGet as a standalone main

    mrk> pwd
    /home/epicsv4/master/exampleCPP/helloPutGet
    mrk> bin/$EPICS_HOST_ARCH/helloPutGetMain

## After starting helloPutGet 

    mrk> pwd
    /home/epicsv4/master/exampleCPP/helloPutGet
    mrk> bin/$EPICS_HOST_ARCH/helloPutGetClient

