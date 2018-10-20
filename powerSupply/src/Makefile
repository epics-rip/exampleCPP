TOP=..

include $(TOP)/configure/CONFIG

powerSupplySRC = $(TOP)/src

EPICS_BASE_PVA_CORE_LIBS = pvaClient pvDatabase pvAccess pvAccessCA pvData ca Com

INC += pv/powerSupplyRecord.h

DBD += powerSupplyRegister.dbd

LIBRARY = powerSupply
LIBSRCS += powerSupplyRecord.cpp
LIBSRCS += powerSupplyRegister.cpp
powerSupply_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

# shared library ABI version.
SHRLIB_VERSION ?= 4.3.0

PROD_HOST += powerSupplyMain
powerSupplyMain_SRCS += powerSupplyMain.cpp
powerSupplyMain_LIBS += powerSupply
powerSupplyMain_LIBS += pvDatabase
powerSupplyMain_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += powerSupplyClient
powerSupplyClient_SRCS += powerSupplyClient.cpp
powerSupplyClient_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)


PROD_HOST += powerSupplyMonitor
powerSupplyMonitor_SRCS += powerSupplyMonitor.cpp
powerSupplyMonitor_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)


PROD_SYS_LIBS_WIN32 += ws2_32


include $(TOP)/configure/RULES

