SRC_DIRS += $(exampleLinkSRC)/code

DBD += exampleLinkRegister.dbd

LIBRARY = exampleLink
LIBSRCS += exampleLink.cpp
LIBSRCS += exampleLinkRegister.cpp
exampleLink_LIBS += pvDatabase pvAccess nt pvData ca Com

PROD_HOST += exampleLinkMain
exampleLinkMain_SRCS += exampleLinkMain.cpp
exampleLinkMain_LIBS += exampleLink
exampleLinkMain_LIBS += pvDatabase pvAccess nt pvData  Com



PROD_HOST += exampleLinkClient
exampleLinkClient_SRCS += exampleLinkClient.cpp
exampleLinkClient_LIBS += pvaClient pvAccess pvData ca Com

