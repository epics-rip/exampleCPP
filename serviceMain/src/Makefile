TOP=..

include $(TOP)/configure/CONFIG

supportSRC = $(TOP)/src

EPICS_BASE_PVA_CORE_LIBS = pvaClient  pvDatabase pvAccess pvAccessCA pvData ca Com


PROD_HOST += serviceMain
serviceMain_SRCS += serviceMain.cpp
serviceMain_LIBS += controlRecord
serviceMain_LIBS += powerSupplyRecord
serviceMain_LIBS += linkRecord
serviceMain_LIBS +=  $(EPICS_BASE_PVA_CORE_LIBS)

include $(TOP)/configure/RULES
