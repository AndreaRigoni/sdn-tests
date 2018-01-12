include $(top_srcdir)/Common.mk
include $(top_srcdir)/codac/CodacDev.mk


_LIBS = ca dan_api dan_client_api dan_stream xml2 log sdn rt pthread tcn
LIBRARY_DIRS =

DAN_DAQ_DIR = $(top_srcdir)/codac/units/

# Already set from docker env in /etc/profile (bash -l)
#
# CODAC_ROOT          ?= /opt/codac-5.4
# EPICS_BASE          ?= /opt/codac-5.4/epics
# SDN_TOPIC_DIRECTORY ?= /opt/codac-5.4/apps/include
# EPICS_HOST_ARCH     ?= linux-x86_64

INCLUDE_DIRS=. \
			 $(CODAC_ROOT)/include \
			 $(EPICS_BASE)/include/os/Linux \
			 $(EPICS_BASE)/include/compiler/gcc \
			 $(EPICS_BASE)/include /usr/include/libxml2 \
			 $(SDN_TOPIC_DIRECTORY)


LDLIBS=-L$(CODAC_ROOT)/lib \
	   -L$(EPICS_BASE)/lib/$(EPICS_HOST_ARCH) \
		 $(foreach libs,$(LIBRARY_DIRS),-L$(libs) -Wl,-rpath,$(libs)) \
		 $(foreach libs,$(_LIBS),-l$(libs))


AM_CPPFLAGS = $(foreach inc,$(INCLUDE_DIRS),-I$(inc))
AM_LDFLAGS  = $(LDLIBS)

bin_PROGRAMS = monitor
monitor_SOURCES = monitor.cpp

_dump = $(if $($1),$(info $1=$($1)),$(info $1 <empty>))
env-dump: ##@codac dump env variables used for build codac components
	@ $(call _dump,CODAC_ROOT) \
	  $(call _dump,EPICS_HOST_ARCH) \
	  $(call _dump,EPICS_BASE) \
	  $(call _dump,SDN_TOPIC_DIRECTORY)

