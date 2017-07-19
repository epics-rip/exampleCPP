< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase("dbd/exampleRPC.dbd")
exampleRPC_registerRecordDeviceDriver(pdbbase)

## Load record instance
dbLoadRecords("db/dbCounter.db","name=pvDatabaseRPCCounter");

cd ${TOP}/iocBoot/${IOC}
iocInit()
dbl
exampleRPCCreateRecord mydevice

