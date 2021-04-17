TOP=../..

include $(TOP)/configure/CONFIG

DBD += exampleRPC.dbd

PROD_IOC += exampleRPC


# <name>_registerRecordDeviceDriver.cpp will be created from <name>.dbd
exampleRPC_SRCS += exampleRPC_registerRecordDeviceDriver.cpp
exampleRPC_SRCS_DEFAULT += exampleRPCMain.cpp

exampleRPC_LIBS += exampleRPC
exampleRPC_LIBS += pvDatabase qsrv pvAccessIOC pvAccess pvAccessCA nt pvData
exampleRPC_LIBS += $(EPICS_BASE_IOC_LIBS)

# needed for Windows
LIB_SYS_LIBS_WIN32 += netapi32 ws2_32

include $(TOP)/configure/RULES
