< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase("dbd/plugin.dbd")
plugin_registerRecordDeviceDriver(pdbbase)

## Load record instances
dbLoadRecords("db/ao.db","name=DBRsupportAo");

cd ${TOP}/iocBoot/${IOC}
iocInit()
traceRecordCreate PVRpluginTraceRecord
removeRecordCreate PVRpluginRemoveRecord
processRecordCreate PVRpluginProcessRecord .5
pluginRecordCreate PVRpluginDouble
pluginRecordCreate PVRpluginUByte pvUByte
scalarArrayRecordCreate PVRscalarArrayDouble
scalarArrayRecordCreate PVRscalarArrayUByte pvUByte
scalarArrayRecordCreate PVRscalarArrayString pvString
scalarArrayRecordCreate PVRscalarArrayBoolean pvBoolean


