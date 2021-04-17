TOP=..

include $(TOP)/configure/CONFIG

supportSRC = $(TOP)/src

EPICS_BASE_PVA_CORE_LIBS = pvDatabase pvAccess pvAccessCA pvData  pvaClient Com


PROD_HOST += serviceMain
serviceMain_SRCS += serviceMain.cpp
serviceMain_LIBS += controlRecord
serviceMain_LIBS += powerSupplyRecord
serviceMain_LIBS += linkRecord
serviceMain_LIBS +=  $(EPICS_BASE_PVA_CORE_LIBS)

PROD_SYS_LIBS_WIN32 += ws2_32

include $(TOP)/configure/RULES
