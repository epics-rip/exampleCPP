# Makefile for the EPICS V4 exampleCPP module

TOP = .
include $(TOP)/configure/CONFIG
#EMBEDDED_TOPS += $(TOP)/ChannelArchiverService
# beginning of service support
EMBEDDED_TOPS += $(TOP)/linkRecord
EMBEDDED_TOPS += $(TOP)/powerSupplyRecord
EMBEDDED_TOPS += $(TOP)/controlRecord
EMBEDDED_TOPS += $(TOP)/helloPutGetRecord
EMBEDDED_TOPS += $(TOP)/helloRPCRecord
# beginning of non support
EMBEDDED_TOPS += $(TOP)/exampleClient
EMBEDDED_TOPS += $(TOP)/pvaClient
EMBEDDED_TOPS += $(TOP)/pvaClientMultiChannel
EMBEDDED_TOPS += $(TOP)/json
EMBEDDED_TOPS += $(TOP)/arrayPerformance
# beginning of users of service support
EMBEDDED_TOPS += $(TOP)/serviceMain
EMBEDDED_TOPS += $(TOP)/database

DIRS := configure
DIRS += $(EMBEDDED_TOPS)


include $(TOP)/configure/RULES_TOP
