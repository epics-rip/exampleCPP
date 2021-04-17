TOP=..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

EPICS_BASE_PVA_CORE_LIBS = pvaClient pvAccess pvAccessCA nt pvData ca Com

PROD_HOST += jsonGet
jsonGet_SRCS += jsonGet.cpp
jsonGet_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += parsePut
parsePut_SRCS += parsePut.cpp
parsePut_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += parsePutGet
parsePutGet_SRCS += parsePutGet.cpp
parsePutGet_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

# needed for Windows
LIB_SYS_LIBS_WIN32 += netapi32 ws2_32

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

