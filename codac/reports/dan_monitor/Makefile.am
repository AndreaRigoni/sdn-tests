include $(top_srcdir)/Common.mk
include $(top_srcdir)/conf/kscripts/qtdesktop.mk

PRO = dan_monitor.pro

# this Fixes the Qt-X11 window for exported desktop
export QT_X11_NO_MITSHM = 1

# user must define the same targets as in the pro file
TARGET = dan_monitor

all: $(TARGET)
clean: clean-local

dan_monitordir = .


export top_builddir
export VPATH

print:
	@ echo $(VPATH)

run: $(TARGET)
	@ export LD_LIBRARY_PATH=$(top_builddir)/ext/qwt/qwt/lib:$(top_builddir)/ext/ccSickers/.libs; \
	  $(DBG) $(builddir)/$(TARGET) $(ARGS)

shell:
	@ bash -l

