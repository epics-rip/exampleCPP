TOP=..
include $(TOP)/configure/CONFIG
# NOTE: The order of the following is important
EPICS_BASE_PVA_CORE_LIBS = pvaClient  pvDatabase pvAccess pvAccessCA nt pvData ca Com

PROD_HOST += serviceMain
# NOTE: The order of the following is important
serviceMain_SRCS += serviceMain.cpp
serviceMain_LIBS += scalarLimitRecord
serviceMain_LIBS += powerSupplyRecord
serviceMain_LIBS += linkRecord
serviceMain_LIBS += helloPutGetRecord
serviceMain_LIBS += helloRPCRecord
serviceMain_LIBS += controlRecord
serviceMain_LIBS +=  $(EPICS_BASE_PVA_CORE_LIBS)

include $(TOP)/configure/RULES
