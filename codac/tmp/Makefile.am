## ////////////////////////////////////////////////////////////////////////// //
##
## This file is part of the dan-tests project.
## Copyright 2018 Andrea Rigoni Garola <andrea.rigoni@igi.cnr.it>.
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.
##
## ////////////////////////////////////////////////////////////////////////// //


include $(top_srcdir)/Common.mk
include $(top_srcdir)/codac/CodacDan.mk

_LIBS = ca dan_api dan_client_api dan_base dan_stream xml2 log sdn rt pthread tcn
LIBRARY_DIRS =

# DAN_DAQ_DIR = $(top_srcdir)/codac/units/
#
# Already set from docker env in /etc/profile (bash -l)
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

bin_PROGRAMS    = monitor
monitor_SOURCES = monitor.cpp

bin_PROGRAMS = ic_dump
ic_dump_SOURCES = ic_dump.cpp
ic_dump_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_builddir)/codac/units/m-dan-daq/src/main/c/dan_api



bin_PROGRAMS   += pv_test
pv_test_SOURCES = pv_access.cpp pv_test.cpp

bin_PROGRAMS   += dan_minimal
dan_minimal_SOURCES = pv_access.cpp dan_minimal.cpp
dan_minimal_CONFIG  = dan_minimal_6_0.xml
dan_minimal_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_builddir)/ext/ccStickers/src

bin_PROGRAMS   += dan_minimal_autoload
dan_minimal_autoload_SOURCES = dan_minimal_autoload.cpp
dan_minimal_autoload_CONFIG  = dan_minimal_6_0.xml
dan_minimal_autoload_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_builddir)/codac/units/m-dan-daq/src/main/c/dan_api


# // SHM MINIMAL TEST //
# NODOCKERBUILD += shm_minimal_test
bin_PROGRAMS   += shm_minimal_test
shm_minimal_test_SOURCES = shm_minimal_test.cpp
shm_minimal_test_CPPFLAGS = $(AM_CPPFLAGS) -I$(top_builddir)/codac/units/m-dan-daq/src/main/c/dan_api
shm_minimal_test_LDFLAGS = $(AM_LDFLAGS) -lrt



DAN_TESTS = dan_minimal dan_minimal_autoload
NAME = dan_minimal_autoload

list: ##@dan list defined dan tests
list: _item = $(info | $1: ) $(foreach x,$($1),$(info |  - $x))
list:
	@ \
	$(info ,-----------------------------------------------------------------) \
	$(info | DAN tests defined ($$NAME) ) \
	$(info |) \
	$(call _item,DAN_TESTS) \
	$(info |) \
	$(info | CURRENT: $(NAME)) \
	$(info |) \
	$(info `-----------------------------------------------------------------) :


## DEFAULTS ##
DAN_ARCHIVE_MASTER = localhost:9999
DAN_ARCHIVE_SLAVE  = localhost:9999
DAN_TESTS_TRATE    = 1024
DAN_TESTS_CSIZE    = 12
DAN_TESTS_BSIZE    = 64
DAN_TESTS_DURATION = 10

export DAN_ARCHIVE_MASTER \
	   DAN_ARCHIVE_SLAVE \
	   DAN_TESTS_TRATE \
	   DAN_TESTS_CSIZE \
	   DAN_TESTS_BSIZE \
	   DAN_TESTS_DURATION

test-set: ##@dan initialize api for test in $NAME
test-set: CU_PCF_DANCONF = $(CU_SETTINGS_DIR)/$($(NAME)_CONFIG)
test-set: dan-aw-start dan-api-init dan-st-start

test-run: ##@dan perform test in $NAME
test-run: export CU_PCF_DANCONF = $(CU_SETTINGS_DIR)/$($(NAME)_CONFIG)
test-run:
	@ ./$(NAME)

test-stop: ##@dan stop test in $NAME
test-stop: CU_PCF_DANCONF = $(CU_SETTINGS_DIR)/$($(NAME)_CONFIG)
test-stop: dan-st-stop

env-dump: ##@codac dump env variables used for build codac components
env-dump: _dump = $(if $($1),$(info - int: $1=$($1)),$(info - int: $1 <empty>)) echo "- env: $1=$${$1}";
env-dump:
	@ $(call _env_set) \
	  $(call _dump,CODAC_ROOT) \
	  $(call _dump,EPICS_HOST_ARCH) \
	  $(call _dump,EPICS_BASE) \
	  $(call _dump,SDN_TOPIC_DIRECTORY) \
	  $(call _dump,DAN_ARCHIVE_MASTER) \
	  $(call _dump,DAN_ARCHIVE_SLAVE)

# CU_PCF_DANCONF   = $(CU_SETTINGS_DIR)/dan_minimal.xml

shell:
	@ bash -l


.dot.ps:
	@ dot -Tps $< -o $@

.dot.pdf:
	@ dot -Tpdf $< -o $@

.pdf.png:
	@ convert $< $@


DOTS = call_archiver \
	   call_openstream

$(DOTS):
	@ $(MAKE) $@.pdf

dots: ##@tests generate dot files for documentation
dots: $(DOTS)

CLEANFILES = $(addsuffix .pdf,$(DOTS)) $(addsuffix .png,$(DOTS))

