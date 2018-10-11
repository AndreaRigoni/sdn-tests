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
include $(top_srcdir)/conf/kscripts/ext_mdsplus.mk





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


#danraw: ##@examples simple data data archiving using the raw disk write form posix
#dansrv: ##@examples dansrv program as an example of data archiving through the srv api
#danclt: ##@examples send a stream of data to a specific dan archiver daemon
#test_multich: ##@examples concurrent threads write
bin_PROGRAMS = danraw dansrv danclt test_multich

danraw_SOURCES = dan_raw.cpp
dansrv_SOURCES = dan_srv.cpp
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
DAN_TARGET_PRT     ?= 9999
DAN_NCHANNELS      ?= 1
DAN_TSIZE          ?= 10G
DAN_DBSIZE         ?= 512K
DAN_TRATE_MB       ?= 100
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
 $(DAN_TARGET_SRV) $(DAN_TARGET_PRT) $(DAN_NCHANNELS) $(DAN_TSIZE) 0 $(DAN_DBSIZE) ,\
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
## //  MDSPLUS  ///////////////////////////////////////////////////////////////////
## ////////////////////////////////////////////////////////////////////////////////

MDS_TARGET_SRV ?= $(DAN_TARGET_SRV)
MDS_TARGET_PRT ?= 8000
MDS_HOWMANY    ?= 1

if EXT_MDSPLUS
bin_PROGRAMS += mds-srv
mds_srv_SOURCES      = mds_srv.cpp
mds_srv_CPPFLAGS     = $(AM_CPPFLAGS) $(MDS_CPPFLAGS)
mds_srv_LDFLAGS      = $(AM_LDFLAGS) $(MDS_LDFLAGS) -Wl,-rpath,${MDS_LIBDIR}

bin_PROGRAMS += mds-clt
mds_clt_SOURCES      = mdsipmain.c mds_clt.cpp
mds_clt_CPPFLAGS     = $(AM_CPPFLAGS) $(MDS_CPPFLAGS) -I$(top_builddir)/ext/mdsplus
mds_clt_LDFLAGS      = $(AM_LDFLAGS) $(MDS_LDFLAGS) -Wl,-rpath,${MDS_LIBDIR}
else
$(warning ext sources for Mdsplus are enabled, please reconfigure)
endif

CLEANFILES += mdssrv_data_001.*
CLEANFILES += mdsclt_data_001.*

mdssrv_data.log: | mds-srv
	@ $(builddir)/mds-srv --file $(basename $@) --prof-file $(basename $@) \
	  --size $(DAN_TSIZE) --db-size $(DAN_DBSIZE)


mdsclt_data.log: | mds-clt
	@ $(builddir)/mds-clt --file $(basename $@) --prof-file $(basename $@) \
	  --size $(DAN_TSIZE) --db-size $(DAN_DBSIZE) \
	  --server_name $(MDS_TARGET_SRV) --server_port $(MDS_TARGET_PRT) \
	  --homany $(MDS_HOWMANY)


DAN_LOGS += mdssrv_data.log








# ////////////////////////////////////////////////////////////////////////////////
# //  REPORTS  ///////////////////////////////////////////////////////////////////
# ////////////////////////////////////////////////////////////////////////////////
# ////////////////////////////////////////////////////////////////////////////////
# ////////////////////////////////////////////////////////////////////////////////


## // DAN RAW VS HDF //

danrawsrv_LOGS = dan_data_INT16_RAW_P.log dan_data_INT16_SRV_P.log

danrawsrv_DT.png: $(danrawsrv_LOGS)
	@ plot-perf -s DT $^ && mv Plot-perf.png $@

danrawsrv_DT: $(danrawsrv_LOGS)
	@ plot-perf DT $^

danrawsrv_TRS.png: $(danrawsrv_LOGS)
	@ plot-perf -s TRS 200 $^ && mv Plot-perf.png $@

danrawsrv_TRS: $(danrawsrv_LOGS)
	@ plot-perf TRS 300 $^

danrawsrv_BUFALL.png: $(danrawsrv_LOGS)
	@ plot-perf -s BUFALL $^ && mv Plot-perf.png $@

danrawsrv_BUFALL: $(danrawsrv_LOGS)
	@ plot-perf BUFALL $^




## // MDS VS DAN //

mdsvsdan_SRV_LOGS = dan_data_INT16_SRV_P.log mdssrv_data.log

mdsvsdan_SRV_TRS.png: $(mdsvsdan_SRV_LOGS)
	@ plot-perf -s TRS 10 $^ && mv Plot-perf.png $@

mdsvsdan_SRV_TRS: $(mdsvsdan_SRV_LOGS)
	@ plot-perf TRS 300 $^

mdsvsdan_SRV_DT: $(mdsvsdan_SRV_LOGS)
	@ plot-perf DT $^

mdsvsdan_SRV_BUFALL.png: $(mdsvsdan_SRV_LOGS)
	@ plot-perf -s BUFALL $^ && mv Plot-perf.png $@

mdsvsdan_SRV_BUFALL: $(mdsvsdan_SRV_LOGS)
	@ plot-perf BUFALL $^



CLEANFILES += mdsvsdan_SRV_TRS.png



## // DAN RAW VS HDF //

dansrvclt_LOGS = dan_data_INT16_SRV_P.log dan_data_INT16_CLT_P.log

dansrvclt_DT.png: $(dansrvclt_LOGS)
	@ plot-perf -s DT $^ && mv Plot-perf.png $@

dansrvclt_DT: $(dansrvclt_LOGS)
	@ plot-perf DT $^

dansrvclt_TRS.png: $(dansrvclt_LOGS)
	@ plot-perf -s TRS 200 $^ && mv Plot-perf.png $@

dansrvclt_TRS: $(dansrvclt_LOGS)
	@ plot-perf TRS 300 $^

dansrvclt_BUFALL.png: $(dansrvclt_LOGS)
	@ plot-perf -s BUFALL $^ && mv Plot-perf.png $@

dansrvclt_BUFALL: $(dansrvclt_LOGS)
	@ plot-perf BUFALL $^





## // MDS VS DAN - CLT //

mdsvsdan_CLT_LOGS = dan_data_INT16_CLT_P.log mdsclt_data.log

mdsvsdan_CLT_TRS.png: $(mdsvsdan_CLT_LOGS)
	@ plot-perf -s TRS 10 $^ && mv Plot-perf.png $@

mdsvsdan_CLT_TRS: $(mdsvsdan_CLT_LOGS)
	@ plot-perf TRS 300 $^

mdsvsdan_CLT_DT: $(mdsvsdan_CLT_LOGS)
	@ plot-perf DT $^

mdsvsdan_CLT_BUFALL.png: $(mdsvsdan_CLT_LOGS)
	@ plot-perf -s BUFALL $^ && mv Plot-perf.png $@

mdsvsdan_CLT_BUFALL: $(mdsvsdan_CLT_LOGS)
	@ plot-perf BUFALL $^

mdsvsdan_CLT_BUF: $(mdsvsdan_CLT_LOGS)
	@ plot-perf BUF $(DAN_TRATE_MB) $^












