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

run: ##@tests run all tests
run: $(TEST_LOGS)


TEST_LOGS =

## ////////////////////////////////////////////////////////////////////////////////
## //  DAN COMPILE  ///////////////////////////////////////////////////////////////
## ////////////////////////////////////////////////////////////////////////////////

DAN_LIBS = \
		   ca  \
		   dan_archive \
		   dan_stream \
		   xml2 log sdn rt pthread tcn \
		   dan_base \
		   dan_api \
		   dan_client_api \
		   dan_hdf \
		   hdf5

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


## /////////////////////////////////////////////////////////////////////////////
## //  EXT  ////////////////////////////////////////////////////////////////////
## /////////////////////////////////////////////////////////////////////////////

ext: ##@ext compile externals
ext: $(CCSTICKERS_LA)
# .PHONY: $(CCSTICKERS_LA)
CCSTICKERS_LA = $(top_builddir)/ext/ccStickers/libccStickers.la
$(CCSTICKERS_LA):
	@ $(MAKE) $(AM_MAKEFLAGS) -C $(top_builddir)/ext ext-ccStickers




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

NODOCKERBUILD += $(FIO_LOGS)
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

NODOCKERBUILD += plots %.png
plots: fio_plot.sh | write_fio.log
	@ sh $< "seq-test"

.eps.png:
	@ convert -density 300 $< $@

.pdf.png:
	@ convert -density 300 $< $@


TEST_LOGS += write-fio.log

CLEANFILES += *.dat *.pdf *.plt



## /////////////////////////////////////////////////////////////////////////////
## //  RAW TEST   //////////////////////////////////////////////////////////////
## /////////////////////////////////////////////////////////////////////////////

bin_PROGRAMS        = write-raw
write_raw_SOURCES  = write_raw.cpp
write_raw_CPPFLAGS = -I$(top_builddir)/ext/ccStickers/src
write_raw_LDFLAGS  =
write_raw_LDADD    = $(CCSTICKERS_LA)

write_raw_ARGS = --size 10 --bs 512 --bufs 1024
write-raw.log: ##@tests run local write-raw application
write-raw.log: ext write-raw
	@ time ./write-raw write-raw.dat $(write_raw_ARGS)

CLEANFILES += write-raw.log
TEST_LOGS += write-raw.log




## ////////////////////////////////////////////////////////////////////////////////
## //  HDF  ///////////////////////////////////////////////////////////////////////
## ////////////////////////////////////////////////////////////////////////////////

bin_PROGRAMS += write-hdf
write_hdf_SOURCES  = $(addprefix $(COMMON_SOURCE_DIR)/,$(COMMON_SOURCES)) write_hdf.cpp
write_hdf_CPPFLAGS = $(DAN_CPPFLAGS) -I$(top_builddir)/ext/ccStickers/src
write_hdf_LDFLAGS  = $(DAN_LDLIBS)
write_hdf_LDADD    = $(CCSTICKERS_LA)



