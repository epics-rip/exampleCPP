TOP=..

include $(TOP)/configure/CONFIG

exampleRPCSRC = $(TOP)/src

EPICS_BASE_PVA_CORE_LIBS = pvDatabase pvAccess pvAccessCA pvData Com

INC += pv/exampleRPC.h
//INC += pv/device.h
INC += pv/point.h

DBD += exampleRPCRegister.dbd

LIBRARY = exampleRPC
LIBSRCS += device.cpp
LIBSRCS += exampleRPC.cpp
LIBSRCS += exampleRPCRegister.cpp
exampleRPC_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

# shared library ABI version.
SHRLIB_VERSION ?= 4.3.0

PROD_HOST += exampleRPCMain
exampleRPCMain_SRCS += exampleRPCMain.cpp
exampleRPCMain_LIBS += exampleRPC
exampleRPCMain_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += control
control_SRCS += positionClient.cpp
control_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_SYS_LIBS_WIN32 += ws2_32


include $(TOP)/configure/RULES


