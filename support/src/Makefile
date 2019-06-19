TOP=..

include $(TOP)/configure/CONFIG

supportSRC = $(TOP)/src

EPICS_BASE_PVA_CORE_LIBS = pvDatabase pvAccess pvAccessCA pvData ca Com

INC += pv/supportRecord.h
INC += pv/scalarArrayRecord.h
INC += pv/scalarRecord.h

DBD += supportRecordRegister.dbd
DBD += scalarArrayRecordRegister.dbd
DBD += scalarRecordRegister.dbd

LIBRARY = support
LIBSRCS += supportRecord.cpp
LIBSRCS += supportRecordRegister.cpp
LIBSRCS += scalarArrayRecord.cpp
LIBSRCS += scalarArrayRecordRegister.cpp
LIBSRCS += scalarRecord.cpp
LIBSRCS += scalarRecordRegister.cpp
support_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

# shared library ABI version.
SHRLIB_VERSION ?= 4.3.0

PROD_HOST += supportMain
supportMain_SRCS += supportMain.cpp
supportMain_LIBS += support
supportMain_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_SYS_LIBS_WIN32 += ws2_32


include $(TOP)/configure/RULES


