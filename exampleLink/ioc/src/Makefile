TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# Build an IOC support library
#

#=============================
# build an ioc application


DBD += exampleLink.dbd

PROD_IOC += exampleLink


# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
exampleLink_SRCS += exampleLink_registerRecordDeviceDriver.cpp
exampleLink_SRCS_DEFAULT += exampleLinkMain.cpp
exampleLink_SRCS_vxWorks += -nil-


# The following adds support from base/src/vxWorks
exampleLink_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

exampleLink_LIBS += exampleLink
exampleLink_LIBS += pvDatabase pvaClient qsrv pvAccessIOC pvAccess pvAccessCA nt pvData
exampleLink_LIBS += $(EPICS_BASE_IOC_LIBS)

PROD_SYS_LIBS_WIN32 += ws2_32

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

