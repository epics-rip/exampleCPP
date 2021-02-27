# exampleCPP

This project provides a set of examples of client and server code for V4.

## obtaining and building examples.

The examples can be cloned from github and built as follows:

    git clone  https://github.com/epics-base/exampleCPP.git
    cd exampleCPP
    cp ExampleRELEASE.local configure/RELEASE.local
    gedit configure/RELEASE.local  // change EPICS7_DIR
    make

The definition of **EPICS7_DIR** must be changed to the location where You 
installed and built a version of **EPICS7**.

There is also a file **MasterRELEASE.local** that can be used if the V4 modules were
obtained from github.

## Description of examples

To see a brief description of each example see documentation/exampleCPP.html.

