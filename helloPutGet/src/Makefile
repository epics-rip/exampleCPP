TOP=..

include $(TOP)/configure/CONFIG

helloPutGetSRC = $(TOP)/src

EPICS_BASE_PVA_CORE_LIBS = pvDatabase pvAccess pvAccessCA pvData ca Com

INC += pv/helloPutGetRecord.h

DBD += helloPutGetRegister.dbd

LIBRARY = helloPutGet
LIBSRCS += helloPutGetRecord.cpp
LIBSRCS += helloPutGetRegister.cpp
helloPutGet_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

# shared library ABI version.
SHRLIB_VERSION ?= 4.3.0

PROD_HOST += helloPutGetMain
helloPutGetMain_SRCS += helloPutGetMain.cpp
helloPutGetMain_LIBS += helloPutGet
helloPutGetMain_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += helloPutGetClient
helloPutGetClient_SRCS += helloPutGetClient.cpp
helloPutGetClient_LIBS += pvaClient
helloPutGetClient_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += helloNoWaitPutGetClient
helloNoWaitPutGetClient_SRCS += helloNoWaitPutGetClient.cpp
helloNoWaitPutGetClient_LIBS += pvaClient
helloNoWaitPutGetClient_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)


PROD_SYS_LIBS_WIN32 += ws2_32


include $(TOP)/configure/RULES


