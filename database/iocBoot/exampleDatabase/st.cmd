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

cd ${TOP}/iocBoot/${IOC}
asSetFilename(asconfig)
pvdbcrScalar PVRboolean boolean
pvdbcrScalar PVRbyte byte
pvdbcrScalar PVRshort short
pvdbcrScalar PVRint int
pvdbcrScalar PVRlong long
pvdbcrScalar PVRubyte ubyte
pvdbcrScalar PVRushort ushort 
pvdbcrScalar PVRuint uint
pvdbcrScalar PVRulong ulong
pvdbcrScalar PVRfloat float
pvdbcrScalar PVRdouble double
pvdbcrScalar PVRstring string
pvdbcrScalar PVRdouble01 double
pvdbcrScalar PVRdouble02 double
pvdbcrScalar PVRdouble03 double
pvdbcrScalar PVRdouble04 double
pvdbcrScalar PVRdouble05 double
pvdbcrScalarArray PVRbooleanArray boolean
pvdbcrScalarArray PVRbyteArray byte
pvdbcrScalarArray PVRshortArray short
pvdbcrScalarArray PVRintArray int
pvdbcrScalarArray PVRlongArray long
pvdbcrScalarArray PVRubyteArray ubyte
pvdbcrScalarArray PVRushortArray ushort 
pvdbcrScalarArray PVRuintArray uint
pvdbcrScalarArray PVRulongArray ulong
pvdbcrScalarArray PVRfloatArray float
pvdbcrScalarArray PVRdoubleArray double
pvdbcrScalarArray PVRstringArray string
pvdbcrScalarArray PVRdouble01Array double
pvdbcrScalarArray PVRdouble02Array double
pvdbcrScalarArray PVRdouble03Array double
pvdbcrScalarArray PVRdouble04Array double
pvdbcrScalarArray PVRdouble05Array double
pvdbcrTraceRecord PVRtraceRecord
pvdbcrRemoveRecord PVRremoveRecord
pvdbcrAddRecord PVRaddRecord
pvdbcrProcessRecord PVRprocessRecord .5
exampleDatabase
exampleSoftRecordCreate PVRsoft
iocInit()
refshow
