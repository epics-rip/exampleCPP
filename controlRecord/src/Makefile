TOP=..

include $(TOP)/configure/CONFIG

controlRecordSRC = $(TOP)/src

EPICS_BASE_PVA_CORE_LIBS = pvDatabase pvAccess pvAccessCA pvData  Com

INC += pvcontrol/controlRecord.h

DBD += controlRecord.dbd

LIBRARY = controlRecord
LIBSRCS += controlRecord.cpp

controlRecord_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

# shared library ABI version.
SHRLIB_VERSION ?= 4.3.0

PROD_SYS_LIBS_WIN32 += ws2_32

include $(TOP)/configure/RULES
