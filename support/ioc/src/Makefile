TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# Build an IOC support library
#

DBD += support.dbd

#=============================
# build an ioc application

PROD_IOC += support


# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
support_SRCS += support_registerRecordDeviceDriver.cpp
support_SRCS_DEFAULT += supportMain.cpp
support_SRCS_vxWorks += -nil-


# The following adds support from base/src/vxWorks
support_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

support_LIBS += support
support_LIBS += pvDatabase qsrv pvAccessIOC pvAccess pvAccessCA nt pvData
support_LIBS += $(EPICS_BASE_IOC_LIBS)

PROD_SYS_LIBS_WIN32 += ws2_32

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

