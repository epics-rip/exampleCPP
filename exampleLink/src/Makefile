TOP=..

include $(TOP)/configure/CONFIG

exampleLinkSRC = $(TOP)/src

EPICS_BASE_PVA_CORE_LIBS = pvDatabase pvaClient pvAccess pvAccessCA nt pvData ca Com

DBD += putLinkScalarRegister.dbd
DBD += putLinkScalarArrayRegister.dbd
DBD += getLinkScalarRegister.dbd
DBD += getLinkScalarArrayRegister.dbd

LIBRARY = exampleLink

LIBSRCS += putLinkScalarRegister.cpp
LIBSRCS += putLinkScalarArrayRegister.cpp
LIBSRCS += getLinkScalarRegister.cpp
LIBSRCS += getLinkScalarArrayRegister.cpp
exampleLink_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

# shared library ABI version.
SHRLIB_VERSION ?= 4.3.0
PROD_SYS_LIBS_WIN32 += ws2_32

include $(TOP)/configure/RULES
