< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase("dbd/exampleLink.dbd")
exampleLink_registerRecordDeviceDriver(pdbbase)

## Load record instance
dbLoadRecords("db/ai.db","name=exampleLinkAI");
cd ${TOP}/iocBoot/${IOC}
##asSetFilename(asconfig)
iocInit()
scalar double double
scalar string string
scalarArray doubleArray double
scalarArray stringArray string

putLinkScalar putLinkScalar
putLinkScalarArray putLinkScalarArray

getLinkScalar getLinkScalar
getLinkScalarArray getLinkScalarArray

