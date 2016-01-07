SRC_DIRS += $(powerSupplySRC)/code

DBD += powerSupplyRegister.dbd

LIBSRCS += powerSupply.cpp
LIBSRCS += powerSupplyRegister.cpp


PROD_HOST += powerSupplyMain
powerSupplyMain_SRCS += powerSupplyMain.cpp
powerSupplyMain_LIBS += pvDatabase
powerSupplyMain_LIBS += pvAccess
powerSupplyMain_LIBS += pvData
powerSupplyMain_LIBS += Com
powerSupplyMain_LIBS += powerSupply

PROD_HOST += powerSupplyClient
powerSupplyClient_SRCS += powerSupplyClient.cpp
powerSupplyClient_LIBS += pvaClient
powerSupplyClient_LIBS += pvAccess
powerSupplyClient_LIBS += pvData
powerSupplyClient_LIBS += Com

