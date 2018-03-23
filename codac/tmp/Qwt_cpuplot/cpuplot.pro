################################################################
# Qwt Widget Library
# Copyright (C) 1997   Josef Wilgen
# Copyright (C) 2002   Uwe Rathmann
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the Qwt License, Version 1.0
################################################################


TARGET       = cpuplot

HEADERS =  \
    cpuplot.h \
    cpustat.h \
    cpupiemarker.h 

SOURCES = \
    cpuplot.cpp \
    cpustat.cpp \
    cpupiemarker.cpp 

LIBS += -L$$(top_builddir)/ext/qwt/qwt/lib -lqwt
INCLUDEPATH += $$(top_builddir)/ext/qwt/qwt/src
DEPENDPATH  += $$(top_builddir)/ext/qwt/qwt/src
