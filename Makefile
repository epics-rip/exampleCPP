# Makefile for the EPICS V4 exampleCPP module

TOP = ./database
include $(TOP)/configure/CONFIG

# No configure directory here

EMBEDDED_TOPS += $(TOP)/../database
EMBEDDED_TOPS += $(TOP)/../exampleClient
EMBEDDED_TOPS += $(TOP)/../exampleLink
EMBEDDED_TOPS += $(TOP)/../powerSupply
EMBEDDED_TOPS += $(TOP)/../helloPutGet
EMBEDDED_TOPS += $(TOP)/../helloRPC
EMBEDDED_TOPS += $(TOP)/../exampleRPCService
EMBEDDED_TOPS += $(TOP)/../arrayPerformance

DIRS += $(TOP)/../test
DIRS += $(EMBEDDED_TOPS)

# No inter-module dependencies

include $(TOP)/configure/RULES_TOP
