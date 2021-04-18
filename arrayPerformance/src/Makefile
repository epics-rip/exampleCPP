TOP = ..
include $(TOP)/configure/CONFIG

EPICS_BASE_PVA_CORE_LIBS = pvaClient  pvDatabase pvAccess pvAccessCA pvData ca Com

arrayPerformanceSRC = $(TOP)/src

INC+= pv/arrayPerformance.h
INC+= pv/longArrayMonitor.h
INC+= pv/longArrayGet.h
INC+= pv/longArrayPut.h


LIBRARY = arrayPerform
LIBSRCS += arrayPerformance.cpp
LIBSRCS += longArrayGet.cpp
LIBSRCS += longArrayMonitor.cpp
LIBSRCS += longArrayPut.cpp
arrayPerform_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += arrayPerformanceMain
arrayPerformanceMain_SRCS += arrayPerformanceMain.cpp
arrayPerformanceMain_LIBS += arrayPerform
arrayPerformanceMain_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += longArrayGetMain
longArrayGetMain_SRCS += longArrayGetMain.cpp
longArrayGetMain_LIBS += arrayPerform
longArrayGetMain_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += longArrayMonitorMain
longArrayMonitorMain_SRCS += longArrayMonitorMain.cpp
longArrayMonitorMain_LIBS += arrayPerform
longArrayMonitorMain_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)


PROD_HOST += longArrayPutMain
longArrayPutMain_SRCS += longArrayPutMain.cpp
longArrayPutMain_LIBS += arrayPerform
longArrayPutMain_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)


PROD_HOST += vectorPerformanceMain
vectorPerformanceMain_SRCS += vectorPerformanceMain.cpp
vectorPerformanceMain_LIBS +=  pvData Com

# needed for Windows
LIB_SYS_LIBS_WIN32 += netapi32 ws2_32

include $(TOP)/configure/RULES

