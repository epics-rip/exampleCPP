#Makefile at top of application tree
#Makefile at top of application tree
TOP = .
include $(TOP)/configure/CONFIG

DIRS := configure
DIRS += src
src_DEPEND_DIRS = configure

DIRS += database
EMBEDDED_TOPS := $(EMBEDDED_TOPS) $(filter-out $(EMBEDDED_TOPS), database)

DIRS += test
test_DEPEND_DIRS = configure database

DIRS += exampleClient
EMBEDDED_TOPS := $(EMBEDDED_TOPS) $(filter-out $(EMBEDDED_TOPS), exampleClient)

DIRS += arrayPerformance
EMBEDDED_TOPS := $(EMBEDDED_TOPS) $(filter-out $(EMBEDDED_TOPS), arrayPerformance)


DIRS += exampleLink
EMBEDDED_TOPS := $(EMBEDDED_TOPS) $(filter-out $(EMBEDDED_TOPS), exampleLink)


DIRS += examplePowerSupply
EMBEDDED_TOPS := $(EMBEDDED_TOPS) $(filter-out $(EMBEDDED_TOPS), examplePowerSupply)


DIRS += exampleServer
EMBEDDED_TOPS := $(EMBEDDED_TOPS) $(filter-out $(EMBEDDED_TOPS), exampleServer)

DIRS += HelloWorld
EMBEDDED_TOPS := $(EMBEDDED_TOPS) $(filter-out $(EMBEDDED_TOPS), HelloWorld)

include $(TOP)/configure/RULES_TOP

