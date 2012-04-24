TOP=..

include $(TOP)/configure/CONFIG

USR_SYS_LIBS += expat

PROD_HOST += showtypes
showtypes_SRCS += showtypes.cpp
showtypes_LIBS += pvIOC pvAccess pvData Com Storage Tools

PROD_HOST += ArchiverServiceRPC
ArchiverServiceRPC_SRCS += ArchiverServiceRPCMain.cpp
ArchiverServiceRPC_SRCS += ArchiverServiceRPC.cpp
ArchiverServiceRPC_LIBS += pvIOC pvAccess pvData Com Storage Tools

PROD_HOST += ArchiverClient
ArchiverClient_SRCS += ArchiverClient.cpp
ArchiverClient_SRCS += ServiceClient.cpp
ArchiverClient_SRCS += ArchiverClientResponseHandler.cpp
ArchiverClient_LIBS += pvAccess pvData Com

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

