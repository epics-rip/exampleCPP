TOP=..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

DBD += powerSupplyRegister.dbd

INC += powerSupply.h

LIBRARY += powerSupply
powerSupply_SRCS += powerSupply.cpp
powerSupply_SRCS += powerSupplyRegister.cpp
powerSupply_LIBS += pvDatabase
powerSupply_LIBS += pvAccess
powerSupply_LIBS += pvData
powerSupply_LIBS += Com
powerSupply_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

