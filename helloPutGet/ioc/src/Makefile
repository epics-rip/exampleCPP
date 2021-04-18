TOP=../..

include $(TOP)/configure/CONFIG

DBD += helloPutGet.dbd

#=============================
# build an ioc application

PROD_IOC += helloPutGet

# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
helloPutGet_SRCS += helloPutGet_registerRecordDeviceDriver.cpp
helloPutGet_SRCS_DEFAULT += helloPutGetMain.cpp


helloPutGet_LIBS += helloPutGet
helloPutGet_LIBS += pvDatabase qsrv pvAccessIOC pvAccess pvAccessCA nt pvData
helloPutGet_LIBS += $(EPICS_BASE_IOC_LIBS)

# needed for Windows
LIB_SYS_LIBS_WIN32 += netapi32 ws2_32

include $(TOP)/configure/RULES
