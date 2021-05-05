TOP=..

include $(TOP)/configure/CONFIG

EPICS_BASE_PVA_CORE_LIBS = pvDatabase pvAccess pvAccessCA nt pvData ca Com

INC += scalarLimit/scalarLimitRecord.h

DBD += scalarLimitRecord.dbd

LIBRARY = scalarLimitRecord
LIBSRCS += scalarLimitRecord.cpp

scalarLimitRecord_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

# needed for Windows
LIB_SYS_LIBS_WIN32 += netapi32 ws2_32

include $(TOP)/configure/RULES
