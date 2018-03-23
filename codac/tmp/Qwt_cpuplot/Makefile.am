include $(top_srcdir)/Common.mk
include $(top_srcdir)/conf/kscripts/qtdesktop.mk

PRO = cpuplot.pro

# this Fixes the Qt-X11 window for exported desktop
export QT_X11_NO_MITSHM = 1

# user must define the same targets as in the pro file
TARGET = cpuplot

all: $(TARGET)
clean: clean-local

NAME ?= $(lastword $(TARGET))

run: export LD_LIBRARY_PATH = $(top_builddir)/ext/qwt/qwt/lib
run:
	@ $(builddir)/$(NAME) $(ARGS)
