TOP=..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

EPICS_BASE_PVA_CORE_LIBS = pvaClient pvAccess pvAccessCA nt pvData ca Com

PROD_HOST += multiPutGetDouble
multiPutGetDouble_SRCS += multiPutGetDouble.cpp
multiPutGetDouble_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += multiMonitorDouble
multiMonitorDouble_SRCS += multiMonitorDouble.cpp
multiMonitorDouble_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += ntMultiPutGet
ntMultiPutGet_SRCS += ntMultiPutGet.cpp
ntMultiPutGet_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += ntMultiMonitor
ntMultiMonitor_SRCS += ntMultiMonitor.cpp
ntMultiMonitor_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)


#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

