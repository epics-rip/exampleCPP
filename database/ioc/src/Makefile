TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# Build an IOC support library
#
LIBRARY_IOC += example

DBD += exampleDatabase.dbd
DBDINC += simpleBusyRecord

example_SRCS += simpleBusyRecord.c
example_LIBS += $(EPICS_BASE_IOC_LIBS)

#=============================
# build an ioc application

PROD_IOC += exampleDatabase


# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
exampleDatabase_SRCS += exampleDatabase_registerRecordDeviceDriver.cpp
exampleDatabase_SRCS_DEFAULT += exampleDatabaseMain.cpp
exampleDatabase_SRCS_vxWorks += -nil-


# The following adds support from base/src/vxWorks
exampleDatabase_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

exampleDatabase_LIBS += exampleDatabase
exampleDatabase_LIBS += pvDatabase qsrv pvAccessIOC pvAccess pvAccessCA nt pvData
exampleDatabase_LIBS += example
exampleDatabase_LIBS += $(EPICS_BASE_IOC_LIBS)

PROD_SYS_LIBS_WIN32 += ws2_32


#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

