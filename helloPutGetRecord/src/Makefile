TOP=..
include $(TOP)/configure/CONFIG

EPICS_BASE_PVA_CORE_LIBS = pvaClient pvDatabase pvAccess pvAccessCA nt pvData ca Com

INC += helloPutGet/helloPutGetRecord.h

DBD += helloPutGetRecord.dbd

LIBRARY = helloPutGetRecord
LIBSRCS += helloPutGetRecord.cpp
helloPutGetRecord_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += helloPutGetClient
helloPutGetClient_SRCS += helloPutGetClient.cpp
helloPutGetClient_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += helloNoWaitPutGetClient
helloNoWaitPutGetClient_SRCS += helloNoWaitPutGetClient.cpp
helloNoWaitPutGetClient_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

# needed for Windows
LIB_SYS_LIBS_WIN32 += netapi32 ws2_32

include $(TOP)/configure/RULES


