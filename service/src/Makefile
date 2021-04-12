TOP=..

include $(TOP)/configure/CONFIG

supportSRC = $(TOP)/src

EPICS_BASE_PVA_CORE_LIBS = pvDatabase pvAccess pvData  Com


PROD_HOST += linkServiceMain
linkServiceMain_SRCS += linkServiceMain.cpp
linkServiceMain_LIBS += supportRecord
linkServiceMain_LIBS +=  $(EPICS_BASE_PVA_CORE_LIBS)

PROD_SYS_LIBS_WIN32 += ws2_32

include $(TOP)/configure/RULES
