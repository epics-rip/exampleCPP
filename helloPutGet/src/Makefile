TOP=..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# Build an IOC support library
#


DBD += helloPutGet.dbd

INC += helloPutGet.h

LIBRARY_IOC += helloPutGet
helloPutGet_SRCS += helloPutGet.cpp
helloPutGet_SRCS += helloPutGetRegister.cpp
helloPutGet_LIBS += pvDatabase
helloPutGet_LIBS += pvAccess
helloPutGet_LIBS += pvData
helloPutGet_LIBS += Com
helloPutGet_LIBS += $(EPICS_BASE_IOC_LIBS)

PROD_HOST += helloPutGetMain
helloPutGetMain_SRCS += helloPutGetMain.cpp
helloPutGetMain_LIBS += helloPutGet
helloPutGetMain_LIBS += pvDatabase
helloPutGetMain_LIBS += pvAccess
helloPutGetMain_LIBS += pvData
helloPutGetMain_LIBS += Com

PROD_HOST += helloPutGetClient
helloPutGetClient_SRCS += helloPutGetClient.cpp
helloPutGetClient_LIBS += pvaClient
helloPutGetClient_LIBS += pvAccess
helloPutGetClient_LIBS += pvData
helloPutGetClient_LIBS += Com


#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

