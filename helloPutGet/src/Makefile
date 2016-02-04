TOP=..

include $(TOP)/configure/CONFIG

INSTALL_INCLUDE = $(INSTALL_LOCATION)/include/pv
USR_INCLUDES += -I$(INSTALL_LOCATION)/include

helloPutGetSRC = $(TOP)/src

include $(helloPutGetSRC)/pv/Makefile
include $(helloPutGetSRC)/code/Makefile

LIBRARY = helloPutGet
helloPutGet_LIBS += pvDatabase pvAccess nt pvData Com
helloPutGet_LIBS += $(EPICS_BASE_IOC_LIBS)

# shared library ABI version.
SHRLIB_VERSION ?= 1.0


include $(TOP)/configure/RULES


