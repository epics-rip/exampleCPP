TOP=..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

#EPICS_BASE_PVA_CORE_LIBS = pvaClient pvAccess pva2ca pvData ca Com
EPICS_BASE_PVA_CORE_LIBS = pvaClient pvAccess pvAccessCA nt pvData ca Com

PROD_HOST += testRequest
testRequest_SRCS += testRequest.cpp
testRequest_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += testPutDouble
testPutDouble_SRCS += testPutDouble.cpp
testPutDouble_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += testGetDouble
testGetDouble_SRCS += testGetDouble.cpp
testGetDouble_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += testPutDoubleArray
testPutDoubleArray_SRCS += testPutDoubleArray.cpp
testPutDoubleArray_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += testGetDoubleArray
testGetDoubleArray_SRCS += testGetDoubleArray.cpp
testGetDoubleArray_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += testPutString
testPutString_SRCS += testPutString.cpp
testPutString_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += testGetString
testGetString_SRCS += testGetString.cpp
testGetString_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += testPutStringArray
testPutStringArray_SRCS += testPutStringArray.cpp
testPutStringArray_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += testGetStringArray
testGetStringArray_SRCS += testGetStringArray.cpp
testGetStringArray_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)



#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

