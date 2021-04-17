TOP=..
include $(TOP)/configure/CONFIG

EPICS_BASE_PVA_CORE_LIBS = pvDatabase pvAccess pvAccessCA pvData ca Com

exampleRPCSRC = $(TOP)/src

INC += pv/exampleRPC.h
INC += pv/point.h

DBD += exampleRPCRegister.dbd

LIBRARY = exampleRPC
LIBSRCS += device.cpp
LIBSRCS += exampleRPC.cpp
LIBSRCS += exampleRPCRegister.cpp
exampleRPC_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += exampleRPCMain
exampleRPCMain_SRCS += exampleRPCMain.cpp
exampleRPCMain_LIBS += exampleRPC
exampleRPCMain_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += control
control_SRCS += positionClient.cpp
control_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

# needed for Windows
LIB_SYS_LIBS_WIN32 += netapi32 ws2_32

include $(TOP)/configure/RULES
