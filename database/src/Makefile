TOP=..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

DBD += exampleDatabaseRegister.dbd

INC += exampleHello.h
INC += exampleHelloRPC.h
INC += exampleDatabase.h

LIBRARY += exampleDatabase
exampleDatabase_SRCS += exampleHello.cpp
exampleDatabase_SRCS += exampleHelloRPC.cpp
exampleDatabase_SRCS += exampleDatabase.cpp
exampleDatabase_SRCS += exampleDatabaseRegister.cpp
exampleDatabase_LIBS += pvDatabase
exampleDatabase_LIBS += pvAccess
exampleDatabase_LIBS += pvData
exampleDatabase_LIBS += nt
exampleDatabase_LIBS += Com
exampleDatabase_LIBS += $(EPICS_BASE_IOC_LIBS)

PROD_HOST += exampleDatabaseMain
exampleDatabaseMain_SRCS += exampleDatabaseMain.cpp
exampleDatabaseMain_LIBS += exampleDatabase
exampleDatabaseMain_LIBS += pvDatabase
exampleDatabaseMain_LIBS += pvAccess
exampleDatabaseMain_LIBS += pvData
exampleDatabaseMain_LIBS += nt
exampleDatabaseMain_LIBS += Com

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

