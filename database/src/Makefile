TOP=..

include $(TOP)/configure/CONFIG

exampleDatabaseSRC = $(TOP)/src

INC += pv/exampleHello.h
INC += pv/exampleHelloRPC.h
INC += pv/exampleDatabase.h

DBD += exampleDatabaseRegister.dbd

LIBRARY = exampleDatabase
LIBSRCS += exampleHello.cpp
LIBSRCS += exampleHelloRPC.cpp
LIBSRCS += exampleDatabase.cpp
LIBSRCS += exampleDatabaseRegister.cpp
exampleDatabase_LIBS += pvDatabase pvAccess nt pvData Com


PROD_HOST += exampleDatabaseMain
exampleDatabaseMain_SRCS += exampleDatabaseMain.cpp
exampleDatabaseMain_LIBS += exampleDatabase
exampleDatabaseMain_LIBS += pvDatabase pvAccess nt pvData Com


# shared library ABI version.
SHRLIB_VERSION ?= 1.0


include $(TOP)/configure/RULES

