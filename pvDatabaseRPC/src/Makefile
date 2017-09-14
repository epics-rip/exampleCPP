TOP=..

include $(TOP)/configure/CONFIG

exampleRPCSRC = $(TOP)/src

INC += pv/exampleRPC.h
//INC += pv/device.h
INC += pv/point.h

DBD += exampleRPCRegister.dbd

LIBRARY = exampleRPC
LIBSRCS += device.cpp
LIBSRCS += exampleRPC.cpp
LIBSRCS += exampleRPCRegister.cpp
exampleRPC_LIBS += pvDatabase pvAccess pvData Com

# shared library ABI version.
SHRLIB_VERSION ?= 4.3.0

PROD_HOST += exampleRPCMain
exampleRPCMain_SRCS += exampleRPCMain.cpp
exampleRPCMain_LIBS += exampleRPC
exampleRPCMain_LIBS += pvDatabase pvAccess pvAccessCA pvData Com

PROD_HOST += control
control_SRCS += positionClient.cpp
control_LIBS += pvAccess pvAccessCA pvData Com


include $(TOP)/configure/RULES


