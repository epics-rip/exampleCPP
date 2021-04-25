TOP=..
include $(TOP)/configure/CONFIG
# NOTE: The order of the following is important
EPICS_BASE_PVA_CORE_LIBS = pvaClient  pvDatabase pvAccess pvAccessCA nt pvData ca Com
INC += pv/exampleDatabase.h
DBD += exampleDatabaseRegister.dbd
LIBRARY = exampleDatabase
LIBSRCS += exampleDatabase.cpp
LIBSRCS += exampleDatabaseRegister.cpp
exampleDatabase_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)
# needed for Windows
LIB_SYS_LIBS_WIN32 += netapi32 ws2_32
include $(TOP)/configure/RULES

