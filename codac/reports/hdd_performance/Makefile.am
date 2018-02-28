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

_LIBS = ca dan_api dan_client_api dan_stream xml2 log sdn rt pthread tcn
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



fio: ##@ext build fio tool
fio: $(FIO)

FIO = $(top_builddir)/ext/fio/fio
$(FIO):
	@ $(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/ext ext-fio

FIO_GENERATE_PLOTS = $(top_builddir)/ext/fio/tools/fio_generate_plots
$(FIO_GENERATE_PLOTS): fio


## Random read/write performance
## ./fio --randrepeat=1 --ioengine=libaio --direct=1 --gtod_reduce=1 \
		 --name=test --filename=test --bs=4k --iodepth=64 --size=4G  \
		 --readwrite=randrw --rwmixread=75

FIO_LOG_TYPES = bw clat iops lat slat
FIO_LOG_LOGS  = $(foreach y,$($(subst .log,,$(FIO_LOGS))_LOGS),\
						  $(foreach x,$(FIO_LOG_TYPES),$y_$x.*.log))

FIO_LOGS = \
		   write_trs.log
#		   rand_access.log

write_trs_LOGS = seq-test


CLEANFILES = $(FIO_LOGS) \
			 $(subst .log,.dat,$(FIO_LOGS)) \
			 $(FIO_LOG_LOGS) \
			 *.eps

## exports for the ini files
export builddir

$(FIO_LOGS): %.log: %.ini
	@ $(FIO) $< --filename=$*.dat > $@


# bin_PROGRAMS      = fio-plot
# fio_plot_SOURCES  = fio-plot.cpp
# fio_plot_CPPFLAGS = -I$(top_builddir)/ext/ccStickers/src

plots: fio_plot.sh
	@ sh $< "test"


.eps.png:
	@ convert -density 300 $< $@



all: $(FIO) $(FIO_LOGS)








