SRC_DIRS += $(helloPutGetSRC)/code

DBD += helloPutGetRegister.dbd

LIBRARY = helloPutGet
LIBSRCS += helloPutGet.cpp
LIBSRCS += helloPutGetRegister.cpp
helloPutGet_LIBS += pvDatabase pvAccess nt pvData Com


PROD_HOST += helloPutGetMain
helloPutGetMain_SRCS += helloPutGetMain.cpp
helloPutGetMain_LIBS += helloPutGet
helloPutGetMain_LIBS += pvDatabase pvAccess pvData Com


PROD_HOST += helloPutGetClient
helloPutGetClient_SRCS += helloPutGetClient.cpp
helloPutGetClient_LIBS += pvaClient
helloPutGetClient_LIBS += pvAccess pvData ca Com
