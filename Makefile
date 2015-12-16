#Makefile at top of application tree
TOP = ./database
include $(TOP)/configure/CONFIG

DIRS += database
DIRS += exampleClient
DIRS += exampleLink
DIRS += powerSupply
DIRS += helloPutGet
DIRS += helloRPC
DIRS += exampleRPCService
DIRS += test
DIRS += arrayPerformance

include $(TOP)/configure/RULES_TOP
