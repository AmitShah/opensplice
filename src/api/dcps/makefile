SUBSYSTEMS	:=

ifeq ($(INCLUDE_API_DCPS_GAPI),yes)
SUBSYSTEMS	+= gapi
ifeq ($(INCLUDE_API_DCPS_SAC),yes)
SUBSYSTEMS	+= sac
endif # INCLUDE_API_DCPS_SAC
ifeq ($(INCLUDE_JAVA),yes)
SUBSYSTEMS	+= java
endif # INCLUDE_JAVA
ifeq ($(INCLUDE_CXX),yes)
ifeq ($(INCLUDE_API_DCPS_CCPP),yes)
SUBSYSTEMS 	+= ccpp
endif # INCLUDE_API_DCPS_CCPP
ifeq ($(INCLUDE_API_DCPS_SACPP),yes)
SUBSYSTEMS 	+= sacpp
endif # INCLUDE_API_DCPS_SACPP
endif # INCLUDE_CXX
ifeq ($(INCLUDE_CS),yes)
ifeq ($(INCLUDE_API_DCPS_SACS),yes)
SUBSYSTEMS      += sacs
endif # INCLUDE_API_DCPS_SACS
endif # INCLUDE_CS
endif # INCLUDE_API_DCPS_GAPI

ifeq ($(INCLUDE_JAVA),yes)
    ifeq ($(INCLUDE_API_DCPS_JNI),yes)
SUBSYSTEMS	+= jni
endif
endif

include $(OSPL_HOME)/setup/makefiles/subsystem.mak

ifeq ($(INCLUDE_CXX),yes)
ifeq ($(INCLUDE_API_DCPS_ISOCPP),yes)
clean: clean_isocpp

link: build_isocpp

build_isocpp:
	magic_make.pl --make --type $(MPC_CISH_TYPE_TO_GEN) --src-co $(MPC_ARGS) isocpp/

clean_isocpp:
	magic_make.pl --squeaky --clean --type $(MPC_CISH_TYPE_TO_GEN) --src-co $(MPC_ARGS) isocpp/

endif # INCLUDE_API_DCPS_ISOCPP
endif # INCLUDE_CXX
