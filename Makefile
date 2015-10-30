#Makefile at top of application tree
TOP = ./database
include $(TOP)/configure/CONFIG

DIRS += database
DIRS += exampleClient
DIRS += arrayPerformance
DIRS += exampleLink
DIRS += examplePowerSupply
DIRS += exampleServer
DIRS += HelloWorld
DIRS += test

include $(TOP)/configure/RULES_TOP
