TOP=..

include $(TOP)/configure/CONFIG

INSTALL_INCLUDE = $(INSTALL_LOCATION)/include/pv
USR_INCLUDES += -I$(INSTALL_LOCATION)/include

EXAMPLEDATABASESRC = $(TOP)/src

include $(EXAMPLEDATABASESRC)/pv/Makefile
include $(EXAMPLEDATABASESRC)/code/Makefile

LIBRARY = exampleDatabase
exampleDatabase_LIBS += pvDatabase pvAccess nt pvData Com
exampleDatabase_LIBS += $(EPICS_BASE_IOC_LIBS)

# shared library ABI version.
SHRLIB_VERSION ?= 1.0


include $(TOP)/configure/RULES

