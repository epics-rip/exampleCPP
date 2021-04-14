TOP=..

include $(TOP)/configure/CONFIG

supportSRC = $(TOP)/src

EPICS_BASE_PVA_CORE_LIBS = pvDatabase pvAccess pvData  Com

INC += pvsupport/supportRecord.h

DBD += supportRecord.dbd

LIBRARY = supportRecord
LIBSRCS += supportRecord.cpp

support_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

# shared library ABI version.
SHRLIB_VERSION ?= 4.3.0

#PROD_HOST += supportMain
#supportMain_SRCS += supportMain.cpp
#supportMain_LIBS += supportRecord
#supportMain_LIBS +=  $(EPICS_BASE_PVA_CORE_LIBS)

PROD_SYS_LIBS_WIN32 += ws2_32

include $(TOP)/configure/RULES
