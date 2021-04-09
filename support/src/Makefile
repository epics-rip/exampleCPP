TOP=..

include $(TOP)/configure/CONFIG

supportSRC = $(TOP)/src

EPICS_BASE_PVA_CORE_LIBS = pvDatabase pvAccess pvData  Com

PROD_HOST += supportMain
supportMain_SRCS += supportMain.cpp
supportMain_LIBS +=  $(EPICS_BASE_PVA_CORE_LIBS)

include $(TOP)/configure/RULES
