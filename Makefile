#Makefile at top of application tree
TOP = ./database
include $(TOP)/configure/CONFIG

DIRS += database
DIRS += exampleClient
DIRS += exampleLink
DIRS += examplePowerSupply
DIRS += helloPutGet
DIRS += helloRPC
DIRS += test
DIRS += arrayPerformance

include $(TOP)/configure/RULES_TOP
