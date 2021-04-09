TOP=..

include $(TOP)/configure/CONFIG

exampleDatabaseSRC = $(TOP)/src

EPICS_BASE_PVA_CORE_LIBS = pvDatabase pvaClient pvAccess pvAccessCA nt pvData ca Com

INC += pv/exampleHelloRecord.h
INC += pv/exampleHelloRPCRecord.h
INC += pv/exampleDatabase.h

DBD += exampleDatabaseRegister.dbd
DBD += exampleSoftRecordRegister.dbd

LIBRARY = exampleDatabase
LIBSRCS += exampleHelloRecord.cpp
LIBSRCS += exampleHelloRPCRecord.cpp
LIBSRCS += exampleDatabase.cpp
LIBSRCS += exampleDatabaseRegister.cpp
LIBSRCS += exampleSoftRecordRegister.cpp
exampleDatabase_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

# shared library ABI version.
SHRLIB_VERSION ?= 4.3.0


PROD_HOST += exampleHelloRPCClient
exampleHelloRPCClient_SRCS += exampleHelloRPCClient.cpp
exampleHelloRPCClient_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)


PROD_SYS_LIBS_WIN32 += ws2_32


include $(TOP)/configure/RULES

