TOP=..

include $(TOP)/configure/CONFIG

pluginSRC = $(TOP)/src

EPICS_BASE_PVA_CORE_LIBS = pvDatabase pvAccess pvAccessCA pvData ca Com

INC += pv/pluginRecord.h

DBD += pluginRecordRegister.dbd
DBD += scalarArrayRecordRegister.dbd

LIBRARY = plugin
LIBSRCS += pluginRecord.cpp
LIBSRCS += pluginRecordRegister.cpp
LIBSRCS += scalarArrayRecord.cpp
LIBSRCS += scalarArrayRecordRegister.cpp
plugin_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

# shared library ABI version.
SHRLIB_VERSION ?= 4.3.0

PROD_HOST += pluginMain
pluginMain_SRCS += pluginMain.cpp
pluginMain_LIBS += plugin
pluginMain_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_SYS_LIBS_WIN32 += ws2_32


include $(TOP)/configure/RULES


