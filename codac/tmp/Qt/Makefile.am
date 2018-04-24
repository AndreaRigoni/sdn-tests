include $(top_srcdir)/Common.mk
include $(top_srcdir)/conf/kscripts/qtdesktop.mk

PRO = dan_monitor.pro

# NODOCKERBUILD += %

# this Fixes the Qt-X11 window for exported desktop
export QT_X11_NO_MITSHM = 1

# user must define the same targets as in the pro file
TARGET = dan_monitor

all: $(TARGET)
clean: clean-local

dan_monitordir = .

dan_monitor_SOURCES = \
					  main.cpp \
					  mainwindow.cpp \
					  cpustat.cpp

dan_monitor_HEADERS = \
		mainwindow.h \
		cpustat.h

dan_monitor_FORMS = \
		mainwindow.ui

export $(TARGET)_SOURCES
export $(TARGET)_HEADERS
export $(TARGET)_FORMS

run: $(TARGET)
	@ export LD_LIBRARY_PATH=$(top_builddir)/ext/qwt/qwt/lib:$(top_builddir)/ext/ccSickers/.libs; \
	  $(builddir)/$(TARGET) $(ARGS)




