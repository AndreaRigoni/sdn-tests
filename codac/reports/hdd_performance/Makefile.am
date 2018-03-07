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

all: write-test


## ////////////////////////////////////////////////////////////////////////////////
## //  DAN COMPILE  ///////////////////////////////////////////////////////////////
## ////////////////////////////////////////////////////////////////////////////////

DAN_LIBS = ca dan_api dan_client_api dan_stream xml2 log sdn rt pthread tcn
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


# AM_CPPFLAGS = $(foreach inc,$(DAN_INCLUDE_DIRS),-I$(inc))
# AM_LDFLAGS  = $(DAN_LDLIBS)


## ////////////////////////////////////////////////////////////////////////////////
## //  FIO  ///////////////////////////////////////////////////////////////////////
## ////////////////////////////////////////////////////////////////////////////////

fio: ##@ext build fio tool
fio: $(FIO)

FIO = $(top_builddir)/ext/fio/fio
$(FIO):
	@ $(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/ext ext-fio

FIO_GENERATE_PLOTS = $(top_builddir)/ext/fio/tools/fio_generate_plots
$(FIO_GENERATE_PLOTS): fio


# // DISCONTINUED //
# FIO_LOG_TYPES = bw clat iops lat slat
# FIO_LOG_LOGS  = $(foreach y,$($(subst .log,,$(FIO_LOGS))_LOGS),\
#						  $(foreach x,$(FIO_LOG_TYPES),$y_$x.*.log))
# write_trs_LOGS = seq-test \
#				 rnd-test

FIO_LOGS = \
		   write_fio.log

CLEANFILES = $(FIO_LOGS) \
			 $(subst .log,.dat,$(FIO_LOGS)) \
			 $(FIO_LOG_LOGS) *.log \
			 *.eps *.png

## // exports for the ini files //
export builddir
export size
export blocksize
export numjobs

write_fio.log: ##@tests perform fio hdd throughput test
$(FIO_LOGS): %.log: %.ini
	@ $(FIO) $< --filename=$*.dat > $@


plots: fio_plot.sh
	@ sh $< "test"

# bin_PROGRAMS      = fio-plot
# fio_plot_SOURCES  = fio-plot.cpp
# fio_plot_CPPFLAGS = -I$(top_builddir)/ext/ccStickers/src

.eps.png:
	@ convert -density 300 $< $@



## ////////////////////////////////////////////////////////////////////////////////
## //  DAN INJECTOR TESTS  ////////////////////////////////////////////////////////
## ////////////////////////////////////////////////////////////////////////////////

DAN_INJECTOR = /opt/codac/tests/dan/dan-api-injector
DAN_INJ_TYPE = INT16_RAW_P INT16_HDF_P INT16_SRV_P

DAN_LOGS = write_dan

CLEANFILES += *.h5

# Number of channels 1 or 2
# Total payload size, B default 10M
# Transfer rate, KB/sec (absent or 0 - maximal available)
# Data block size, B default 100K
# HDF5 payload chunk size. B default 10 datablocks
# HDF5 headers chunk size, headers
write_dan_ARGS = 1 4G 0 512K 1M

write_dan: ##@tests perform DAN ijector tests
$(DAN_LOGS): _dan_inj = $(foreach typ,$(DAN_INJ_TYPE),\
			 $(DAN_INJECTOR)_$(typ) $(abs_builddir)/$1_$(typ).h5 $($1_ARGS) > \
			 $1_$(typ).log && mv DAN_Injector.log $1_data_$(typ).log;)

$(DAN_LOGS):
	@ $(call _dan_inj,$@)


shell:
	@ bash -l


## ////////////////////////////////////////////////////////////////////////////////
## //  INDEPENDENT TEST   /////////////////////////////////////////////////////////
## ////////////////////////////////////////////////////////////////////////////////


bin_PROGRAMS        = write-test
write_test_SOURCES  = write_test.cpp
write_test_CPPFLAGS = -std=c++0x -I$(top_builddir)/ext/ccStickers/src
write_test_LDFLAGS  =
write_test_LDADD    = $(top_builddir)/ext/ccStickers/libccStickers.la


$(addprefix run-,$(bin_PROGRAMS)): run-%: %
	@ ./$*


write_test_ARGS = --size 8 --bs 512 --bufs 1024
write-test.log: ##@tests run local write-test application
write-test.log: write-test
	@ ./write-test write-test.dat $(write_test_ARGS)












