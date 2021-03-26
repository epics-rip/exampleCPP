TOP=..

include $(TOP)/configure/CONFIG

supportSRC = $(TOP)/src

EPICS_BASE_PVA_CORE_LIBS = pvDatabase pvAccess pvAccessCA pvData ca Com

DBD += supportRecordRegister.dbd
DBD += scalarArrayRecordRegister.dbd
DBD += scalarRecordRegister.dbd

LIBRARY = support
LIBSRCS += supportRecordRegister.cpp
LIBSRCS += scalarArrayRecordRegister.cpp
LIBSRCS += scalarRecordRegister.cpp
support_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

# shared library ABI version.
SHRLIB_VERSION ?= 4.3.0
PROD_SYS_LIBS_WIN32 += ws2_32

include $(TOP)/configure/RULES
