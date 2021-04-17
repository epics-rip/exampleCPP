TOP=..

include $(TOP)/configure/CONFIG

EPICS_BASE_PVA_CORE_LIBS = pvaClient  pvDatabase pvAccess pvAccessCA pvData ca Com

powerSupplySRC = $(TOP)/src

INC += powerSupply/powerSupplyRecord.h

DBD += powerSupplyRecord.dbd

LIBRARY = powerSupplyRecord
LIBSRCS += powerSupplyRecord.cpp
powerSupply_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += powerSupplyClient
powerSupplyClient_SRCS += powerSupplyClient.cpp
powerSupplyClient_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)


PROD_HOST += powerSupplyMonitor
powerSupplyMonitor_SRCS += powerSupplyMonitor.cpp
powerSupplyMonitor_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

# needed for Windows
LIB_SYS_LIBS_WIN32 += netapi32 ws2_32

include $(TOP)/configure/RULES

