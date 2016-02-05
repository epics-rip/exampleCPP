TOP=..

include $(TOP)/configure/CONFIG

exampleDatabaseSRC = $(TOP)/src

INC += pv/exampleHello.h
INC += pv/exampleHelloRPC.h
INC += pv/exampleDatabase.h

include $(exampleDatabaseSRC)/code/Makefile


# shared library ABI version.
SHRLIB_VERSION ?= 1.0


include $(TOP)/configure/RULES

