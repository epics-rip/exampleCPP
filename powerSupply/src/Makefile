TOP=..

include $(TOP)/configure/CONFIG


powerSupplySRC = $(TOP)/src

INC += pv/powerSupply.h

DBD += powerSupplyRegister.dbd

LIBRARY = powerSupply
LIBSRCS += powerSupply.cpp
LIBSRCS += powerSupplyRegister.cpp
powerSupply_LIBS += pvDatabase pvAccess nt pvData Com

PROD_HOST += powerSupplyMain
powerSupplyMain_SRCS += powerSupplyMain.cpp
powerSupplyMain_LIBS += powerSupply
powerSupplyMain_LIBS += pvDatabase
powerSupplyMain_LIBS += pvAccess pvData Com

PROD_HOST += powerSupplyClient
powerSupplyClient_SRCS += powerSupplyClient.cpp
powerSupplyClient_LIBS += pvaClient pvAccess pvData ca Com

# shared library ABI version.
SHRLIB_VERSION ?= 1.0


include $(TOP)/configure/RULES

