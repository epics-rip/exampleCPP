TOP=..

include $(TOP)/configure/CONFIG

exampleLinkSRC = $(TOP)/src

EPICS_BASE_PVA_CORE_LIBS = pvDatabase pvaClient pvAccess pvAccessCA nt pvData ca Com

INC += pv/exampleMonitorLinkRecord.h
INC += pv/exampleGetLinkRecord.h
INC += pv/examplePutLinkRecord.h
INC += pv/doubleArrayRecord.h

DBD += exampleMonitorLinkRegister.dbd
DBD += exampleGetLinkRegister.dbd
DBD += examplePutLinkRegister.dbd
DBD += doubleArrayRegister.dbd

LIBRARY = exampleLink

# shared library ABI version.
SHRLIB_VERSION ?= 4.3.0

LIBSRCS += exampleMonitorLinkRecord.cpp
LIBSRCS += exampleGetLinkRecord.cpp
LIBSRCS += examplePutLinkRecord.cpp
LIBSRCS += doubleArrayRecord.cpp
LIBSRCS += exampleMonitorLinkRegister.cpp
LIBSRCS += exampleGetLinkRegister.cpp
LIBSRCS += examplePutLinkRegister.cpp
LIBSRCS += doubleArrayRegister.cpp
exampleLink_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += exampleLinkMain
exampleLinkMain_SRCS += exampleLinkMain.cpp
exampleLinkMain_LIBS += exampleLink
exampleLinkMain_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_SYS_LIBS_WIN32 += ws2_32


include $(TOP)/configure/RULES


