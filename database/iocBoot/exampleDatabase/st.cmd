< envPaths

cd ${TOP}

## Register all support components
dbLoadDatabase("dbd/exampleDatabase.dbd")
exampleDatabase_registerRecordDeviceDriver(pdbbase)

## Load record instances
dbLoadRecords("db/dbStringArray.db","name=DBRstringArray01")
dbLoadRecords("db/dbEnum.db","name=DBRenum01")
dbLoadRecords("db/dbCounter.db","name=DBRcounter01")
dbLoadRecords("db/dbArray.db","name=DBRdoubleArray,type=DOUBLE")
dbLoadRecords("db/test.db")
dbLoadRecords("db/testtypes.db")
dbLoadRecords("db/dbSimpleBusy.db","name=DBRbusy")

cd ${TOP}/iocBoot/${IOC}
asSetFilename(asconfig)
pvdbcrScalarRecord PVRboolean boolean
pvdbcrScalarRecord PVRbyte byte
pvdbcrScalarRecord PVRshort short
pvdbcrScalarRecord PVRint int
pvdbcrScalarRecord PVRlong long
pvdbcrScalarRecord PVRubyte ubyte
pvdbcrScalarRecord PVRushort ushort 
pvdbcrScalarRecord PVRuint uint
pvdbcrScalarRecord PVRulong ulong
pvdbcrScalarRecord PVRfloat float
pvdbcrScalarRecord PVRdouble double
pvdbcrScalarRecord PVRstring string
pvdbcrScalarRecord PVRdouble01 double
pvdbcrScalarRecord PVRdouble02 double
pvdbcrScalarRecord PVRdouble03 double
pvdbcrScalarRecord PVRdouble04 double
pvdbcrScalarRecord PVRdouble05 double
pvdbcrScalarArrayRecord PVRbooleanArray boolean
pvdbcrScalarArrayRecord PVRbyteArray byte
pvdbcrScalarArrayRecord PVRshortArray short
pvdbcrScalarArrayRecord PVRintArray int
pvdbcrScalarArrayRecord PVRlongArray long
pvdbcrScalarArrayRecord PVRubyteArray ubyte
pvdbcrScalarArrayRecord PVRushortArray ushort 
pvdbcrScalarArrayRecord PVRuintArray uint
pvdbcrScalarArrayRecord PVRulongArray ulong
pvdbcrScalarArrayRecord PVRfloatArray float
pvdbcrScalarArrayRecord PVRdoubleArray double
pvdbcrScalarArrayRecord PVRstringArray string
pvdbcrScalarArrayRecord PVRdouble01Array double
pvdbcrScalarArrayRecord PVRdouble02Array double
pvdbcrScalarArrayRecord PVRdouble03Array double
pvdbcrScalarArrayRecord PVRdouble04Array double
pvdbcrScalarArrayRecord PVRdouble05Array double
pvdbcrTraceRecord PVRtraceRecord
pvdbcrRemoveRecord PVRremoveRecord
pvdbcrAddRecord PVRaddRecord
pvdbcrProcessRecord PVRprocessRecord .5
powerSupplyRecord PVRpowerSupply
controlRecord PVRcontrolDouble double
controlRecord PVRcontrolUbyte ubyte
getLinkScalar PVRgetLinkScalar
getLinkScalarArray PVRgetLinkScalarArray
putLinkScalar PVRputLinkScalar
putLinkScalarArray PVRputLinkScalarArray
helloPutGetRecord PVRhelloPutGet
helloRPCRecord PVRhelloRPC
exampleDatabase
iocInit()
refshow
