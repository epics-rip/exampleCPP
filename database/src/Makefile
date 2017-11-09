TOP=..

include $(TOP)/configure/CONFIG

exampleDatabaseSRC = $(TOP)/src

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
exampleDatabase_LIBS += pvDatabase pvAccess nt pvData Com

# shared library ABI version.
SHRLIB_VERSION ?= 4.3.0


PROD_HOST += exampleDatabaseMain
exampleDatabaseMain_SRCS += exampleDatabaseMain.cpp
exampleDatabaseMain_LIBS += exampleDatabase
exampleDatabaseMain_LIBS += pvDatabase pvAccess pvAccessCA nt pvData Com

PROD_HOST += exampleHelloRPCClient
exampleHelloRPCClient_SRCS += exampleHelloRPCClient.cpp
exampleHelloRPCClient_LIBS += pvaClient pvAccess pvAccessCA nt pvData ca Com


PROD_SYS_LIBS_WIN32 += ws2_32


include $(TOP)/configure/RULES

