SRC_DIRS += $(powerSupplySRC)/code

DBD += powerSupplyRegister.dbd

LIBRARY = powerSupply
LIBSRCS += powerSupply.cpp
LIBSRCS += powerSupplyRegister.cpp
powerSupply_LIBS += pvDatabase pvAccess nt pvData Com

PROD_HOST += powerSupplyMain
powerSupplyMain_SRCS += powerSupplyMain.cpp
powerSupplyMain_LIBS += powerSupply
powerSupplyMain_LIBS += pvDatabase
powerSupplyMain_LIBS += pvAccess pvData Com


PROD_HOST += powerSupplyClient
powerSupplyClient_SRCS += powerSupplyClient.cpp
powerSupplyClient_LIBS += pvaClient pvAccess pvData ca Com

