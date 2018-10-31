TOP=..

include $(TOP)/configure/CONFIG

exampleDatabaseSRC = $(TOP)/src

EPICS_BASE_PVA_CORE_LIBS = pvDatabase pvaClient pvAccess pvAccessCA nt pvData Com

INC += pv/exampleHelloRecord.h
INC += pv/exampleHelloRPCRecord.h
INC += pv/exampleSoftRecord.h
INC += pv/exampleDatabase.h

DBD += exampleDatabaseRegister.dbd

LIBRARY = exampleDatabase
LIBSRCS += exampleHelloRecord.cpp
LIBSRCS += exampleSoftRecord.cpp
LIBSRCS += exampleHelloRPCRecord.cpp
LIBSRCS += exampleDatabase.cpp
LIBSRCS += exampleDatabaseRegister.cpp
exampleDatabase_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

# shared library ABI version.
SHRLIB_VERSION ?= 4.3.0


PROD_HOST += exampleDatabaseMain
exampleDatabaseMain_SRCS += exampleDatabaseMain.cpp
exampleDatabaseMain_LIBS += exampleDatabase
exampleDatabaseMain_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += exampleHelloRPCClient
exampleHelloRPCClient_SRCS += exampleHelloRPCClient.cpp
exampleHelloRPCClient_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)


PROD_SYS_LIBS_WIN32 += ws2_32


include $(TOP)/configure/RULES

