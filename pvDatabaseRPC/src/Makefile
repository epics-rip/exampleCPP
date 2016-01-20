TOP=..

include $(TOP)/configure/CONFIG

INSTALL_INCLUDE = $(INSTALL_LOCATION)/include/pv
USR_INCLUDES += -I$(INSTALL_LOCATION)/include

exampleRPCSRC = $(TOP)/src

include $(exampleRPCSRC)/pv/Makefile
include $(exampleRPCSRC)/code/Makefile

LIBRARY = exampleRPC
exampleRPC_LIBS += pvDatabase pvAccess nt pvData Com
exampleRPC_LIBS += $(EPICS_BASE_IOC_LIBS)


include $(TOP)/configure/RULES


