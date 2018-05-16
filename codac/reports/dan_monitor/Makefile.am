include $(top_srcdir)/Common.mk
include $(top_srcdir)/conf/kscripts/qtdesktop.mk
include $(top_srcdir)/codac/CodacDan.mk

PRO = dan_monitor.pro

# this Fixes the Qt-X11 window for exported desktop
export QT_X11_NO_MITSHM = 1

# user must define the same targets as in the pro file
TARGET = dan_monitor

all: qmake_all
clean: clean-local

dan_monitordir = .

export top_builddir
export VPATH


DAN_LIBS = ca dan_api dan_client_api dan_base dan_stream xml2 log sdn rt pthread tcn
LIBRARY_DIRS =

INCLUDE_DIRS=. \
			 $(CODAC_ROOT)/include \
			 $(EPICS_BASE)/include/os/Linux \
			 $(EPICS_BASE)/include/compiler/gcc \
			 $(EPICS_BASE)/include /usr/include/libxml2 \
			 $(SDN_TOPIC_DIRECTORY) \
			 $(top_builddir)/codac/units/m-dan-daq/src/main/c/dan_api

DAN_INCLUDE_DIRS = $(foreach x,$(INCLUDE_DIRS),-I$x )

LDLIBS = -L$(CODAC_ROOT)/lib \
		 -L$(EPICS_BASE)/lib/$(EPICS_HOST_ARCH) \
		 $(foreach libs,$(LIBRARY_DIRS),-L$(libs) -Wl,-rpath,$(libs)) \
		 $(foreach libs,$(DAN_LIBS),-l$(libs))



export INCLUDE_DIRS
export DAN_INCLUDE_DIRS
export LDLIBS

print:
	@ echo $(VPATH)

run: all
	@ export LD_LIBRARY_PATH=$(top_builddir)/ext/qwt/qwt/lib:$(top_builddir)/ext/ccSickers/.libs; \
	  $(DBG) $(builddir)/$(TARGET) $(ARGS)

debug: ##@tests debug test in NAME
debug: all
	@ export LD_LIBRARY_PATH=$(top_builddir)/ext/qwt/qwt/lib:$(top_builddir)/ext/ccSickers/.libs; \
	  libtool --mode=execute gdb $(builddir)/$(TARGET) $(ARGS)

shell:
	@ bash -l

