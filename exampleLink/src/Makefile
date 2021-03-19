TOP=..

include $(TOP)/configure/CONFIG

exampleLinkSRC = $(TOP)/src

EPICS_BASE_PVA_CORE_LIBS = pvDatabase pvaClient pvAccess pvAccessCA nt pvData ca Com

INC += pv/putLinkScalarRecord.h
INC += pv/putLinkScalarArrayRecord.h
INC += pv/getLinkScalarRecord.h
INC += pv/getLinkScalarArrayRecord.h

DBD += putLinkScalarRegister.dbd
DBD += putLinkScalarArrayRegister.dbd
DBD += getLinkScalarRegister.dbd
DBD += getLinkScalarArrayRegister.dbd
DBD += scalarRegister.dbd
DBD += scalarArrayRegister.dbd

LIBRARY = exampleLink

# shared library ABI version.
SHRLIB_VERSION ?= 4.3.0

LIBSRCS += putLinkScalarRecord.cpp
LIBSRCS += putLinkScalarArrayRecord.cpp
LIBSRCS += putLinkScalarRegister.cpp
LIBSRCS += putLinkScalarArrayRegister.cpp
LIBSRCS += getLinkScalarRecord.cpp
LIBSRCS += getLinkScalarArrayRecord.cpp
LIBSRCS += getLinkScalarRegister.cpp
LIBSRCS += getLinkScalarArrayRegister.cpp
LIBSRCS += scalarRegister.cpp
LIBSRCS += scalarArrayRegister.cpp
exampleLink_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_SYS_LIBS_WIN32 += ws2_32


include $(TOP)/configure/RULES


