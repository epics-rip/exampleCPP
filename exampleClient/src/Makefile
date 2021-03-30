TOP=..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE

EPICS_BASE_PVA_CORE_LIBS = pvaClient pvAccess pvAccessCA nt pvData ca Com
#EPICS_BASE_PVA_CORE_LIBS = pvaClient pvAccess pva2ca nt pvData ca Com

PROD_HOST += testRequest
testRequest_SRCS += testRequest.cpp
testRequest_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += get
get_SRCS += get.cpp
get_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += put
put_SRCS += put.cpp
put_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += process
process_SRCS += process.cpp
process_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += addRecord
addRecord_SRCS += addRecord.cpp
addRecord_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += getputmonitor
getputmonitor_SRCS += getputmonitor.cpp
getputmonitor_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += putGet
putGet_SRCS += putGet.cpp
putGet_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += monitor
monitor_SRCS += monitor.cpp
monitor_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += getField
getField_SRCS += getField.cpp
getField_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += examplePvaClientProcess
examplePvaClientProcess_SRCS += examplePvaClientProcess.cpp
examplePvaClientProcess_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += examplePvaClientGet
examplePvaClientGet_SRCS += examplePvaClientGet.cpp
examplePvaClientGet_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += examplePvaClientPut
examplePvaClientPut_SRCS += examplePvaClientPut.cpp
examplePvaClientPut_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += examplePvaClientMonitor
examplePvaClientMonitor_SRCS += examplePvaClientMonitor.cpp
examplePvaClientMonitor_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += examplePvaClientMultiDouble
examplePvaClientMultiDouble_SRCS += examplePvaClientMultiDouble.cpp
examplePvaClientMultiDouble_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += examplePvaClientNTMulti
examplePvaClientNTMulti_SRCS += examplePvaClientNTMulti.cpp
examplePvaClientNTMulti_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += ntMultiMonitor
ntMultiMonitor_SRCS += ntMultiMonitor.cpp
ntMultiMonitor_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += ntMultiGet
ntMultiGet_SRCS += ntMultiGet.cpp
ntMultiGet_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += ntMultiPut
ntMultiPut_SRCS += ntMultiPut.cpp
ntMultiPut_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += multiGetDouble
multiGetDouble_SRCS += multiGetDouble.cpp
multiGetDouble_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += multiPutDouble
multiPutDouble_SRCS += multiPutDouble.cpp
multiPutDouble_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += multiMonitorDouble
multiMonitorDouble_SRCS += multiMonitorDouble.cpp
multiMonitorDouble_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += helloWorldRPC
helloWorldRPC_SRCS += helloWorldRPC.cpp
helloWorldRPC_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += rpc
rpc_SRCS += rpc.cpp
rpc_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += helloWorldPutGet
helloWorldPutGet_SRCS += helloWorldPutGet.cpp
helloWorldPutGet_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)

PROD_HOST += putUnion
putUnion_SRCS += putUnion.cpp
putUnion_LIBS += $(EPICS_BASE_PVA_CORE_LIBS)


PROD_SYS_LIBS_WIN32 += ws2_32


#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE

