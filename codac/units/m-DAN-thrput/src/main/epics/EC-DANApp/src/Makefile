TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS AFTER THIS LINE
#========================================

#========================================
# Build the IOC application

PROD_IOC = EC-DAN
# EC-DAN.dbd will be created and installed
DBD += EC-DAN.dbd

# EC-DAN.dbd will be made up from these files:
-include $(EPICS_ROOT)/mk/codac-common.mk


# Add all the support libraries needed by this IOC
-include $(EPICS_ROOT)/mk/picmg.mk

EC-DAN_DBD += $(CODAC_DBD)
EC-DAN_LIBS += $(CODAC_LIBS)

# # SNCSEQ
# <sncProgram>_SNCFLAGS += +r -c +d
# EC-DAN_DBD += <sncProgram>.dbd
# EC-DAN_SRCS += <sncProgram>.stt
# EC-DAN_LIBS += seq pv


# EC-DAN_registerRecordDeviceDriver.cpp derives from EC-DAN.dbd
EC-DAN_SRCS += EC-DAN_registerRecordDeviceDriver.cpp

# Build the main IOC entry point on workstation OSs.
EC-DAN_SRCS_DEFAULT += EC-DANMain.cpp
EC-DAN_SRCS_vxWorks += -nil-

# Add support from base/src/vxWorks if needed
#EC-DAN_OBJS_vxWorks += $(EPICS_BASE_BIN)/vxComLibrary

#=============================
# Include SNL program makefile snippets

-include ../*.snlprog

#=============================


# Finally link to the EPICS Base libraries
EC-DAN_LIBS += $(EPICS_BASE_IOC_LIBS)

#===========================

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD RULES AFTER THIS LINE
#----------------------------------------
