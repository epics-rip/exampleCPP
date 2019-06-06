TOP=..

include $(TOP)/configure/CONFIG

supportSRC = $(TOP)/src

EPICS_BASE_PVA_CORE_LIBS = pvDatabase pvAccess pvAccessCA pvData ca Com

INC += pv/supportRecord.h

DBD += supportRecordRegister.dbd

LIBRARY = support
LIBSRCS += supportRecord.cpp
LIBSRCS += supportRecordRegister.cpp
support_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

# shared library ABI version.
SHRLIB_VERSION ?= 4.3.0

PROD_HOST += supportMain
supportMain_SRCS += supportMain.cpp
supportMain_LIBS += support
supportMain_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_SYS_LIBS_WIN32 += ws2_32


include $(TOP)/configure/RULES


