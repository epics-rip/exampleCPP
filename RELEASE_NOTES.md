exampleCPP Release 4.2
========================

This is the release for EPICS V4 release 4.6

exampleCPP has examples for pvaClient and pvDatabase.
The pvaClient examples require that a pvDatabase is running.
If the tests are in pvaClient then circular dependiciesb are created between github repositories.

It also has a test for

make runtests

exampleCPP Release 4.1
========================


HelloWord and ChannelArchiverService
------------------------------------

For release 4.5 of EPICS V4 these were the only components in exampleCPP.

The only significant change since release 4.0 is:

* Calls to deprecated functions PVStructure::getXXXField() have been replaced
  with call of template getSubField().

