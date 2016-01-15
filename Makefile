#Makefile at top of application tree
TOP = ./database
include $(TOP)/configure/CONFIG


DIRS := $(DIRS) $(filter-out $(DIRS), $(TOP)/../database)
DIRS := $(DIRS) $(filter-out $(DIRS), $(TOP)/../exampleClient)
DIRS := $(DIRS) $(filter-out $(DIRS), $(TOP)/../exampleLink)
DIRS := $(DIRS) $(filter-out $(DIRS), $(TOP)/../powerSupply)
DIRS := $(DIRS) $(filter-out $(DIRS), $(TOP)/../helloPutGet)
DIRS := $(DIRS) $(filter-out $(DIRS), $(TOP)/../helloRPC)
#DIRS := $(DIRS) $(filter-out $(DIRS), $(TOP)/../exampleRPCService)
DIRS := $(DIRS) $(filter-out $(DIRS), $(TOP)/../test)
DIRS := $(DIRS) $(filter-out $(DIRS), $(TOP)/../arrayPerformance)


$(foreach dir, $(filter-out configure,$(DIRS)),$(eval $(call $(dir))))

include $(TOP)/configure/RULES_TOP

