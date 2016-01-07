TOP=..

include $(TOP)/configure/CONFIG

INSTALL_INCLUDE = $(INSTALL_LOCATION)/include/pv
USR_INCLUDES += -I$(INSTALL_LOCATION)/include

powerSupplySRC = $(TOP)/src

include $(powerSupplySRC)/pv/Makefile
include $(powerSupplySRC)/code/Makefile

LIBRARY = powerSupply
powerSupply_LIBS += pvDatabase pvAccess nt pvData Com
powerSupply_LIBS += $(EPICS_BASE_IOC_LIBS)


include $(TOP)/configure/RULES
\
