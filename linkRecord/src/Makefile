TOP=..

include $(TOP)/configure/CONFIG

EPICS_BASE_PVA_CORE_LIBS = pvaClient  pvDatabase pvAccess pvAccessCA pvData ca Com

INC += linkRecord/getLinkScalarRecord.h
INC += linkRecord/getLinkScalarArrayRecord.h
INC += linkRecord/putLinkScalarRecord.h
INC += linkRecord/putLinkScalarArrayRecord.h

DBD += getLinkScalarRecord.dbd
DBD += getLinkScalarArrayRecord.dbd
DBD += putLinkScalarRecord.dbd
DBD += putLinkScalarArrayRecord.dbd

LIBRARY = linkRecord

LIBSRCS += getLinkScalarRecord.cpp
LIBSRCS += getLinkScalarArrayRecord.cpp
LIBSRCS += putLinkScalarRecord.cpp
LIBSRCS += putLinkScalarArrayRecord.cpp

linkRecord_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

# needed for Windows
LIB_SYS_LIBS_WIN32 += netapi32 ws2_32

include $(TOP)/configure/RULES
