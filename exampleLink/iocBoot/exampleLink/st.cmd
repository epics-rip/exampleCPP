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
pvdbcrScalar double double
pvdbcrScalar string string
pvdbcrScalarArray doubleArray double
pvdbcrScalarArray stringArray string

putLinkScalar putLinkScalar
putLinkScalarArray putLinkScalarArray

getLinkScalar getLinkScalar
getLinkScalarArray getLinkScalarArray

