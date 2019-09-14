< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase("dbd/support.dbd")
support_registerRecordDeviceDriver(pdbbase)

## Load record instances
dbLoadRecords("db/ao.db","name=DBRsupportAo");

cd ${TOP}/iocBoot/${IOC}
iocInit()
traceRecordCreate PVRtraceRecord
removeRecordCreate PVRremoveRecord
addRecordCreate PVRaddRecord
processRecordCreate PVRprocessRecord .5
supportRecordCreate PVRsupportDouble
supportRecordCreate PVRsupportUByte pvUByte
scalarArrayRecordCreate PVRscalarArrayDouble pvDouble
scalarArrayRecordCreate PVRscalarArrayUByte pvUByte
scalarArrayRecordCreate PVRscalarArrayString pvString
scalarArrayRecordCreate PVRscalarArrayBoolean pvBoolean
scalarRecordCreate PVRscalarDouble pvDouble -10 10 .5
scalarRecordCreate PVRscalarUByte pvUByte 0 20 1

