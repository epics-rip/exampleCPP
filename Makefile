# Makefile for the EPICS V4 exampleCPP module

TOP = .
include $(TOP)/configure/CONFIG
#EMBEDDED_TOPS += $(TOP)/ChannelArchiverService
EMBEDDED_TOPS += $(TOP)/exampleClient
EMBEDDED_TOPS += $(TOP)/pvaClient
EMBEDDED_TOPS += $(TOP)/pvaClientMultiChannel
EMBEDDED_TOPS += $(TOP)/helloPutGet
EMBEDDED_TOPS += $(TOP)/helloRPC
EMBEDDED_TOPS += $(TOP)/pvDatabaseRPC
EMBEDDED_TOPS += $(TOP)/json
EMBEDDED_TOPS += $(TOP)/arrayPerformance
# beginning of service support
EMBEDDED_TOPS += $(TOP)/linkRecord
EMBEDDED_TOPS += $(TOP)/powerSupply
EMBEDDED_TOPS += $(TOP)/support
# beginning of users of service support
EMBEDDED_TOPS += $(TOP)/database
EMBEDDED_TOPS += $(TOP)/service

DIRS := configure
DIRS += $(EMBEDDED_TOPS)


include $(TOP)/configure/RULES_TOP
