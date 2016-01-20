SRC_DIRS += $(ARRAYPERFORMSRC)/code

LIBSRCS += arrayPerformance.cpp
LIBSRCS += longArrayMonitor.cpp
LIBSRCS += longArrayGet.cpp
LIBSRCS += longArrayPut.cpp

PROD_HOST += arrayPerformanceMain
arrayPerformanceMain_SRCS += arrayPerformanceMain.cpp
arrayPerformanceMain_LIBS += pvDatabase pvAccess pvData Com
arrayPerformanceMain_LIBS += arrayPerform

PROD_HOST += longArrayMonitorMain
longArrayMonitorMain_SRCS += longArrayMonitorMain.cpp
longArrayMonitorMain_LIBS += pvDatabase pvAccess pvData Com
longArrayMonitorMain_LIBS += arrayPerform

PROD_HOST += longArrayGetMain
longArrayGetMain_SRCS += longArrayGetMain.cpp
longArrayGetMain_LIBS += pvDatabase pvAccess pvData Com
longArrayGetMain_LIBS += arrayPerform


PROD_HOST += longArrayPutMain
longArrayPutMain_SRCS += longArrayPutMain.cpp
longArrayPutMain_LIBS += pvDatabase pvAccess pvData Com
longArrayPutMain_LIBS += arrayPerform


PROD_HOST += vectorPerformanceMain
vectorPerformanceMain_SRCS += vectorPerformanceMain.cpp
vectorPerformanceMain_LIBS +=  pvData Com



