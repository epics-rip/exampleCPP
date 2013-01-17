TOP=..

include $(TOP)/configure/CONFIG

USR_SYS_LIBS += expat

PROD_HOST += HelloService
HelloService_SRCS += HelloServiceMain.cpp
HelloService_SRCS += HelloService.cpp
HelloService_LIBS += dbIoc pvAccess pvData Com Storage Tools

PROD_HOST += HelloClient
HelloClient_SRCS += HelloClient.cpp
HelloClient_LIBS += dbIoc pvAccess pvData Com Storage Tools

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

