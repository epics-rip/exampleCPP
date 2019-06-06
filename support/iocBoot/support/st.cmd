< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase("dbd/support.dbd")
support_registerRecordDeviceDriver(pdbbase)

## Load record instances
dbLoadRecords("db/ai.db","name=helloPutGetAI");

cd ${TOP}/iocBoot/${IOC}
iocInit()
traceRecordCreate PVRtraceRecord
removeRecordCreate PVRremoveRecord
supportRecordCreate PVRsupportDouble
supportRecordCreate PVRsupportUByte pvUByte


