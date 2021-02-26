TOP=..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#EPICS_BASE_PVA_CORE_LIBS = pvaClient pvAccess pva2ca pvData ca Com
EPICS_BASE_PVA_CORE_LIBS = pvaClient pvAccess pvAccessCA nt pvData ca Com

PROD_HOST += pvaPutGetSleep
pvaPutGetSleep_SRCS += pvaPutGetSleep.cpp
pvaPutGetSleep_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)
pvaPutGetSleep_LIBS += $(EPICS_BASE_IOC_LIBS)

PROD_HOST += pvaPutGetMutex
pvaPutGetMutex_SRCS += pvaPutGetMutex.cpp
pvaPutGetMutex_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)
pvaPutGetMutex_LIBS += $(EPICS_BASE_IOC_LIBS)

PROD_HOST += pvaClientPutGet
pvaClientPutGet_SRCS += pvaClientPutGet.cpp
pvaClientPutGet_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += pvaClientPutClientGet
pvaClientPutClientGet_SRCS += pvaClientPutClientGet.cpp
pvaClientPutClientGet_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += pvaClientProcess
pvaClientProcess_SRCS += pvaClientProcess.cpp
pvaClientProcess_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

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

