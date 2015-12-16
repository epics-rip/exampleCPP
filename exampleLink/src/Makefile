TOP=..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# Build an IOC support library
#

DBD += exampleLinkRegister.dbd

INC += exampleLink.h

LIBRARY_IOC += exampleLink
exampleLink_SRCS += exampleLink.cpp
exampleLink_SRCS += exampleLinkRegister.cpp
exampleLink_LIBS += pvDatabase
exampleLink_LIBS += pvAccess
exampleLink_LIBS += pvData
exampleLink_LIBS += nt
exampleLink_LIBS += Com
exampleLink_LIBS += $(EPICS_BASE_IOC_LIBS)

PROD_HOST += exampleLinkMain
exampleLinkMain_SRCS += exampleLinkMain.cpp
exampleLinkMain_LIBS += exampleLink
exampleLinkMain_LIBS += pvDatabase
exampleLinkMain_LIBS += pvAccess
exampleLinkMain_LIBS += pvData
exampleLinkMain_LIBS += nt
exampleLinkMain_LIBS += Com


PROD_HOST += exampleLinkClient
exampleLinkClient_SRCS += exampleLinkClient.cpp
exampleLinkClient_LIBS += pvaClient
exampleLinkClient_LIBS += pvAccess
exampleLinkClient_LIBS += pvData
exampleLinkClient_LIBS += Com




#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

