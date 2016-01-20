SRC_DIRS += $(exampleLinkSRC)/code

DBD += exampleLinkRegister.dbd

LIBSRCS += exampleLink.cpp
LIBSRCS += exampleLinkRegister.cpp


PROD_HOST += exampleLinkMain
exampleLinkMain_SRCS += exampleLinkMain.cpp
exampleLinkMain_LIBS += exampleLink
exampleLinkMain_LIBS += pvDatabase
exampleLinkMain_LIBS += pvAccess
exampleLinkMain_LIBS += pvData
exampleLinkMain_LIBS += nt
exampleLinkMain_LIBS += Com


PROD_HOST += exampleLinkClient
exampleLinkClient_SRCS += exampleLinkClient.cpp
exampleLinkClient_LIBS += pvaClient
exampleLinkClient_LIBS += pvAccess
exampleLinkClient_LIBS += pvData
exampleLinkClient_LIBS += Com

