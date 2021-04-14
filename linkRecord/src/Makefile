TOP=..

include $(TOP)/configure/CONFIG

EPICS_BASE_PVA_CORE_LIBS = pvDatabase pvaClient pvData Com

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

# shared library ABI version.
SHRLIB_VERSION ?= 4.3.0
PROD_SYS_LIBS_WIN32 += ws2_32

include $(TOP)/configure/RULES
