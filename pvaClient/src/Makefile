TOP=..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#EPICS_BASE_PVA_CORE_LIBS = pvaClient pvAccess pva2ca pvData ca Com
EPICS_BASE_PVA_CORE_LIBS = pvaClient pvAccess pvAccessCA nt pvData ca Com

PROD_HOST += shortestPutGetDouble
shortestPutGetDouble_SRCS += shortestPutGetDouble.cpp
shortestPutGetDouble_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += shortestPutGetDoubleArray
shortestPutGetDoubleArray_SRCS += shortestPutGetDoubleArray.cpp
shortestPutGetDoubleArray_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += longerPutGetDouble
longerPutGetDouble_SRCS += longerPutGetDouble.cpp
longerPutGetDouble_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += longerPutGetDoubleArray
longerPutGetDoubleArray_SRCS += longerPutGetDoubleArray.cpp
longerPutGetDoubleArray_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

