SRC_DIRS += $(exampleDatabaseSRC)/code

DBD += exampleDatabaseRegister.dbd

LIBRARY = exampleDatabase
LIBSRCS += exampleHello.cpp
LIBSRCS += exampleHelloRPC.cpp
LIBSRCS += exampleDatabase.cpp
LIBSRCS += exampleDatabaseRegister.cpp
exampleDatabase_LIBS += pvDatabase pvAccess nt pvData Com


PROD_HOST += exampleDatabaseMain
exampleDatabaseMain_SRCS += exampleDatabaseMain.cpp
exampleDatabaseMain_LIBS += exampleDatabase
exampleDatabaseMain_LIBS += pvDatabase pvAccess nt pvData Com


