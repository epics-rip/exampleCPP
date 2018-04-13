
GITHUB Main
===========

* Code has been added to database to test all DBF types.
* testRequest has been added to exampleClient.

EPICS 7 release 4.7
====================

* exampleClient: getNoBlock, putNoBlock, monitorNoBlock, and getFieldNoBlock no longer exist.
  These are replaced by get, put, putGet, monitor, and getField.
  All work even if channel is not connected when server is started.
* exampleLink: This now has examples for a get, put, and monitor link.
  Each works even if linked channel does not exist when server is started.


exampleCPP release 4.3
----------------------

Summary of changes since release 4.2:

* exampleClient
  * new example using the non-blocking features of pvaClient are implemented.

* exampleLink
  * major changes to use the non-blocking features of pvaClient.
  * examples for monitor, get, and put links.
  * now works if linked channel is not available when ioc is started.

* minor changes to other examples.





EPICS V4 release 4.6
====================

exampleCPP release 4.2
----------------------

Summary of changes since release 4.1:

* HelloWorld has been renamed helloRPC.

* The following examples have been moved from pvDatabaseCPP:
  * arrayPerformance
  * database
  * exampleLink
  * helloPutGet
  * powerSupply
  * pvDatabaseRPC

* The following have been moved from pvaClientCPP:
  * exampleClient
  * test


EPICS V4 release 4.5
========================

The only significant change since release 4.0 is:

* Calls to deprecated functions PVStructure::getXXXField() have been replaced
  with call of template getSubField().


Release 4.0
===========

This was the starting point for RELEASE_NOTES

This release contains two example services: HelloWorld and 
ChannelArchiverService


