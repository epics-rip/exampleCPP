TOP=..

include $(TOP)/configure/CONFIG

INSTALL_INCLUDE = $(INSTALL_LOCATION)/include/pv
USR_INCLUDES += -I$(INSTALL_LOCATION)/include

exampleLinkSRC = $(TOP)/src

include $(exampleLinkSRC)/pv/Makefile
include $(exampleLinkSRC)/code/Makefile

LIBRARY = exampleLink
exampleLink_LIBS += pvDatabase pvAccess nt pvData Com
exampleLink_LIBS += $(EPICS_BASE_IOC_LIBS)

# shared library ABI version.
SHRLIB_VERSION ?= 1.0


include $(TOP)/configure/RULES


