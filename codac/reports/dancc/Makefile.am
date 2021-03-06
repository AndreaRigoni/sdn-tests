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


## /////////////////////////////////////////////////////////////////////////////
## //  DAN COMPILE  ////////////////////////////////////////////////////////////
## /////////////////////////////////////////////////////////////////////////////

DAN_LIBS = \
		   ca  \
		   dan_archive \
		   dan_stream \
		   xml2 log sdn rt pthread tcn \
		   dan_base \
		   dan_api \
		   dan_client_api \
		   dan_hdf \
		   pthread


#COMMON_SOURCE_DIR=$(top_builddir)/codac/units/m-dan-common/src/main/c/common
#COMMON_SOURCES = dan_cMetadataCash.c dan_stream_struct.c

DAN_LIBRARY_DIRS =

# DAN_DAQ_DIR = $(top_srcdir)/codac/units/
#
# Already set from docker env in /etc/profile (bash -l)
# CODAC_ROOT          ?= /opt/codac-5.4
# EPICS_BASE          ?= /opt/codac-5.4/epics
# SDN_TOPIC_DIRECTORY ?= /opt/codac-5.4/apps/include
# EPICS_HOST_ARCH     ?= linux-x86_64

DAN_INCLUDE_DIRS=. \
				 $(CODAC_ROOT)/include \
				 $(EPICS_BASE)/include/os/Linux \
				 $(EPICS_BASE)/include/compiler/gcc \
				 $(EPICS_BASE)/include /usr/include/libxml2 \
				 $(SDN_TOPIC_DIRECTORY)


DAN_LDLIBS=-L$(CODAC_ROOT)/lib \
		   -L$(EPICS_BASE)/lib/$(EPICS_HOST_ARCH) \
		   $(foreach libs,$(DAN_LIBRARY_DIRS),-L$(libs) -Wl,-rpath,$(libs)) \
		   $(foreach libs,$(DAN_LIBS),-l$(libs))

DAN_CPPFLAGS = -g -fPIC -O2 -fno-strict-aliasing -pthread -W -Wall -Wno-unused \
-Wno-parentheses -DNDEBUG -Di586 -DARCH='"i586"' -DLINUX -D_LARGEFILE64_SOURCE \
-D_GNU_SOURCE -D_REENTRANT -D_LITTLE_ENDIAN $(foreach inc,$(DAN_INCLUDE_DIRS),-I$(inc))

# AM_CPPFLAGS = $(foreach inc,$(DAN_INCLUDE_DIRS),-I$(inc))
# AM_LDFLAGS  = $(DAN_LDLIBS)


lib_LTLIBRARIES = libdancc.la

libdancc_la_SOURCES  = $(addprefix $(COMMON_SOURCE_DIR)/,$(COMMON_SOURCES)) \
						dancc_data_source.cpp \
						dancc_stream.cpp

libdancc_la_CPPFLAGS = $(DAN_CPPFLAGS) -I$(top_builddir)/ext/ccStickers/src
libdancc_la_LDFLAGS  = $(DAN_LDLIBS)
libdancc_la_LIBADD   = $(top_builddir)/ext/ccStickers/libccStickers.la



bin_PROGRAMS = test1 test2

test1_SOURCES = dancc_test1.cpp
test1_CPPFLAGS = $(DAN_CPPFLAGS) -I$(top_builddir)/ext/ccStickers/src
test1_LDFLAGS  = $(DAN_LDLIBS)
test1_LDADD   = $(top_builddir)/ext/ccStickers/libccStickers.la libdancc.la

test2_SOURCES = dancc_test2.cpp
test2_CPPFLAGS = $(DAN_CPPFLAGS) -I$(top_builddir)/ext/ccStickers/src
test2_LDFLAGS  = $(DAN_LDLIBS)
test2_LDADD   = $(top_builddir)/ext/ccStickers/libccStickers.la libdancc.la


NAME = $(lastword $(bin_PROGRAMS))
run: ##@tests run test in NAME
run: $(NAME)
	@ $(builddir)/$(NAME) $(ARGS)

debug: ##@tests debug test in NAME
debug: $(NAME)
	@ libtool --mode=execute gdb $(builddir)/$(NAME) $(ARGS)

clean_local:
	@ rm -rf /tmp/data/*

plot: ##@tests plot all /tmp/data files
	@ dan-plot VT /tmp/data/*.h5 0 0

shell:
	@ bash -l
