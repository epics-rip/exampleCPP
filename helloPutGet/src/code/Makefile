SRC_DIRS += $(helloPutGetSRC)/code

DBD += helloPutGetRegister.dbd

LIBSRCS += helloPutGet.cpp
LIBSRCS += helloPutGetRegister.cpp


PROD_HOST += helloPutGetMain
helloPutGetMain_SRCS += helloPutGetMain.cpp
helloPutGetMain_LIBS += helloPutGet
helloPutGetMain_LIBS += pvDatabase
helloPutGetMain_LIBS += pvAccess
helloPutGetMain_LIBS += pvData
helloPutGetMain_LIBS += Com

PROD_HOST += helloPutGetClient
helloPutGetClient_SRCS += helloPutGetClient.cpp
helloPutGetClient_LIBS += pvaClient
helloPutGetClient_LIBS += pvAccess
helloPutGetClient_LIBS += pvData
helloPutGetClient_LIBS += Com

