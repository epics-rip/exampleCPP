/*
 * Copyright information and license terms for this software can be
 * found in the file LICENSE that is included with the distribution
 */

/**
 * @author mrk
 */

/* Author: Marty Kraimer */
#include <iostream>
#include <epicsGetopt.h>
#include <pv/pvaClient.h>


using namespace std;
using namespace epics::pvData;

int main(int argc,char *argv[])
{
    if(argc!=2) {
       cout << "enter a string to pass to createRequest\n";
       return 1;
    }
    CreateRequest::shared_pointer createRequest(CreateRequest::create());
    string request(argv[1]);
    PVStructurePtr pvRequest(createRequest->createRequest(request));
    if(pvRequest) {
        cout << "pvRequest\n" << pvRequest << endl;
    } else {
        cout << "error " << createRequest->getMessage() << endl;
    }
    return 0;
}
