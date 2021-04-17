TOP=..

include $(TOP)/configure/CONFIG

controlRecordSRC = $(TOP)/src

EPICS_BASE_PVA_CORE_LIBS = pvDatabase pvAccess pvAccessCA pvData  Com

INC += pvcontrol/controlRecord.h

DBD += controlRecord.dbd

LIBRARY = controlRecord
LIBSRCS += controlRecord.cpp

controlRecord_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

# needed for Windows
LIB_SYS_LIBS_WIN32 += netapi32 ws2_32

include $(TOP)/configure/RULES
