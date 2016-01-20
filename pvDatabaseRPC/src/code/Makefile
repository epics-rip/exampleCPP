
SRC_DIRS += $(exampleRPCSRC)/code

DBD += exampleRPCRegister.dbd

LIBSRCS += exampleRPC.cpp
LIBSRCS += exampleRPCRegister.cpp


PROD_HOST += exampleRPCMain
exampleRPCMain_SRCS += exampleRPCMain.cpp
exampleRPCMain_LIBS += exampleRPC
exampleRPCMain_LIBS += pvDatabase
exampleRPCMain_LIBS += pvAccess
exampleRPCMain_LIBS += pvData
exampleRPCMain_LIBS += Com

PROD_HOST += move
move_SRCS += positionClient.cpp
move_LIBS += pvAccess pvData Com

