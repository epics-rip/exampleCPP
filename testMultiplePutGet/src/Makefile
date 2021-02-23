TOP=..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#EPICS_BASE_PVA_CORE_LIBS = pvaClient pvAccess pva2ca pvData ca Com
EPICS_BASE_PVA_CORE_LIBS = pvaClient pvAccess pvAccessCA nt pvData ca Com

PROD_HOST += pvaPutGet
pvaPutGet_SRCS += pvaPutGet.cpp
pvaPutGet_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)
pvaPutGet_LIBS += $(EPICS_BASE_IOC_LIBS)

PROD_HOST += pvaClientPutGet
pvaClientPutGet_SRCS += pvaClientPutGet.cpp
pvaClientPutGet_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += pvaClientPutputPutget
pvaClientPutputPutget_SRCS += pvaClientPutputPutget.cpp
pvaClientPutputPutget_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += pvaClientNTMultiPutGet
pvaClientNTMultiPutGet_SRCS += pvaClientNTMultiPutGet.cpp
pvaClientNTMultiPutGet_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

