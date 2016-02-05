TOP = ..
include $(TOP)/configure/CONFIG

arrayPerformanceSRC = $(TOP)/src

INC+= pv/arrayPerformance.h
INC+= pv/longArrayMonitor.h
INC+= pv/longArrayGet.h
INC+= pv/longArrayPut.h

include $(arrayPerformanceSRC)/code/Makefile


# shared library ABI version.
SHRLIB_VERSION ?= 1.0


include $(TOP)/configure/RULES

