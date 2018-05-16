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
		   dan_hdf


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

DAN_COOMON = $(addprefix $(COMMON_SOURCE_DIR)/,$(COMMON_SOURCES)))

AM_CPPFLAGS = $(DAN_CPPFLAGS) -I$(top_builddir)/ext/ccStickers/src
AM_LDFLAGS  = $(DAN_LDLIBS)
LDADD       = $(top_builddir)/ext/ccStickers/libccStickers.la


bin_PROGRAMS = danraw danhdf danclt test_multich

danraw_SOURCES = dan_raw.cpp
danhdf_SOURCES = dan_hdf.cpp
danclt_SOURCES = dan_clt.cpp
test_multich_SOURCES = test_multich.cpp



## ////////////////////////////////////////////////////////////////////////////////
## //  DAN INJECTOR TESTS  ////////////////////////////////////////////////////////
## ////////////////////////////////////////////////////////////////////////////////
#
# NOT CURRENTLY IN USE
#

DAN_TARGET_DIR ?= $(abs_builddir)
DIRECTORIES    += $(DAN_TARGET_DIR)
export DAN_TARGET_DIR


DAN_INJECTOR       ?= /opt/codac/tests/dan/dan-api-injector
DAN_INJ_TYPE       ?= INT16_RAW_P INT16_HDF_P INT16_SRV_P INT16_CLT_P
DAN_TARGET_SRV     ?= localhost
DAN_NCHANNELS      ?= 1
DAN_TSIZE          ?= 2G
DAN_DBSIZE         ?= 512K
DAN_TRATE_MB       ?= 250
DAN_LOGS_PREFIX    ?= dan
DAN_AVERAGE_POINTS ?= 100


# ARGS:
# Number of channels 1 or 2
# Total payload size, B default 10M
# Transfer rate, KB/sec (absent or 0 - maximal available)
# Data block size, B default 100K
# HDF5 payload chunk size. B default 10 datablocks
# HDF5 headers chunk size, headers

define _dan_injtarget =
$2_ARGS ?= $(if $(findstring CLT,$2),\
 $(DAN_TARGET_SRV) 9999 $(DAN_NCHANNELS) $(DAN_TSIZE) 0 $(DAN_DBSIZE) ,\
 $(DAN_TARGET_DIR)/$1_$2.out $(DAN_NCHANNELS) $(DAN_TSIZE) 0 $(DAN_DBSIZE))
$1_data_$2.log: | $(DAN_TARGET_DIR) $(if $(findstring CLT,$2),dan-aw-start)
	@ $$(DAN_INJECTOR)_$2 $$($2_ARGS) > $1_$2.log && mv DAN_Injector.log $$@
$1_plot_DT_$2.png: $1_data_$2.log
	@ plot-perf -s DT $1_data_$2.log && mv Plot-perf.png $$@
$1_plot_BUFALL_$2.png: $1_data_$2.log
	@ plot-perf -s BUFALL $1_data_$2.log && mv Plot-perf.png $$@
$1_plot_BUF_$2.png: $1_data_$2.log
	@ plot-perf -s BUF $(DAN_TRATE_MB) $1_data_$2.log && mv Plot-perf.png $$@
endef
$(foreach t,$(DAN_INJ_TYPE),$(eval $(call _dan_injtarget,$(DAN_LOGS_PREFIX),$t)))

show_plots_DT:
	@ plot-perf DT $(DAN_LOGS_PREFIX)_data_*.log
show_plots_BUFALL:
	@ plot-perf BUFALL $(DAN_LOGS_PREFIX)_data_*.log
show_plots_TRS:
	@ plot-perf TRS $(DAN_AVERAGE_POINTS) $(DAN_LOGS_PREFIX)_data_*.log
show_plots_BUF:
	@ plot-perf BUF $(DAN_TRATE_MB) $(DAN_LOGS_PREFIX)_data_*.log


DAN_LOGS = $(foreach t,$(DAN_INJ_TYPE),$(DAN_LOGS_PREFIX)_data_$t.log)
DAN_PLOTS_DT = $(foreach t,$(DAN_INJ_TYPE),$(DAN_LOGS_PREFIX)_plot_DT_$t.png)
DAN_PLOTS_BUFALL = $(foreach t,$(DAN_INJ_TYPE),$(DAN_LOGS_PREFIX)_plot_BUFALL_$t.png)

dan_log: ##@tests perform DAN ijector tests
dan_log: $(DAN_LOGS)

dan_logplot: ##@tests perform DAN ijector tests and save plots to png files
dan_logplot: $(DAN_PLOTS_DT) $(DAN_PLOTS_BUFALL)

DAN_LOGSHOW ?= DT TRS BUFALL BUF
dan_showlog: ##@tests show log results using pyplot
dan_showlog: $(DAN_LOGS)
	$(MAKE) -j $(foreach type,$(DAN_LOGSHOW), show_plots_$(type) )



CLEANFILES = *.h5 $(DAN_TARGET_DIR)/*.out $(DAN_LOGS) *.png *.log /tmp/data/*.h5






## ////////////////////////////////////////////////////////////////////////////////
## //  RUN  ///////////////////////////////////////////////////////////////////////
## ////////////////////////////////////////////////////////////////////////////////


NAME = $(lastword $(bin_PROGRAMS))
run: ##@tests run test in NAME
run: $(NAME)
	@ $(builddir)/$(NAME) $(ARGS)

shell:
	@ bash -l


