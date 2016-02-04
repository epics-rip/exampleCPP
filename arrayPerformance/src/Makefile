TOP = ..
include $(TOP)/configure/CONFIG

INSTALL_INCLUDE = $(INSTALL_LOCATION)/include/pv
USR_INCLUDES += -I$(INSTALL_LOCATION)/include

ARRAYPERFORMSRC = $(TOP)/src

include $(ARRAYPERFORMSRC)/pv/Makefile
include $(ARRAYPERFORMSRC)/code/Makefile

LIBRARY = arrayPerform
arrayPerform_LIBS += pvDatabase pvAccess pvData Com

# shared library ABI version.
SHRLIB_VERSION ?= 1.0


include $(TOP)/configure/RULES

