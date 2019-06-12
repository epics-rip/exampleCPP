TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# Build an IOC plugin library
#

DBD += plugin.dbd

#=============================
# build an ioc application

PROD_IOC += plugin


# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
plugin_SRCS += plugin_registerRecordDeviceDriver.cpp
plugin_SRCS_DEFAULT += pluginMain.cpp
plugin_SRCS_vxWorks += -nil-


# The following adds plugin from base/src/vxWorks
plugin_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

plugin_LIBS += plugin
plugin_LIBS += pvDatabase qsrv pvAccessIOC pvAccess pvAccessCA nt pvData
plugin_LIBS += $(EPICS_BASE_IOC_LIBS)

PROD_SYS_LIBS_WIN32 += ws2_32

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

