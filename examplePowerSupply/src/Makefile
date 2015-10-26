TOP=..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#=============================

#==================================================
# Build an IOC support library
#

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

PROD_HOST += powerSupplyMain
powerSupplyMain_SRCS += powerSupplyMain.cpp

powerSupplyMain_LIBS += pvDatabase
powerSupplyMain_LIBS += pvAccess
powerSupplyMain_LIBS += pvData
powerSupplyMain_LIBS += Com
powerSupplyMain_LIBS += powerSupply

PROD_HOST += examplePutGet
examplePutGet_SRCS += examplePutGet.cpp
examplePutGet_LIBS += pvaClient
examplePutGet_LIBS += pvAccess
examplePutGet_LIBS += pvData
examplePutGet_LIBS += Com


#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

