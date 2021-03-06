# included by bld/$(SPLICE_TARGET)/makefile

TARGET_DLIB	:= $(DDS_LOCUTIL)
TARGET_LINK_DIR := $(SPLICE_LIBRARY_PATH)

include	$(OSPL_HOME)/setup/makefiles/target.mak

CPPFLAGS	+= -DOSPL_BUILD_LOC_UTIL
CFLAGS	+= $(SHCFLAGS) $(MTCFLAGS)
CINCS	   += -I$(OSPL_HOME)/src/abstraction/include
CINCS	   += -I$(OSPL_HOME)/src/database/database/include
CINCS	   += -I$(OSPL_HOME)/src/kernel/include
CINCS	   += -I$(OSPL_HOME)/src/user/include

LDFLAGS	+= $(SHLDFLAGS)
LDLIBS	+= $(SHLDLIBS)
LDLIBS	+= -l$(DDS_OS) -l$(DDS_USER) -l$(DDS_KERNEL) -l$(DDS_DATABASE)

-include $(DEPENDENCIES)
