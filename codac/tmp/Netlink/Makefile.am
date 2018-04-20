include $(top_srcdir)/Common.mk
include $(top_srcdir)/conf/kscripts/qtdesktop.mk

noinst_LTLIBRARIES = libnlstats.la
libnlstats_la_SOURCES = nl_stats.c


bin_PROGRAMS = nl_stats_test
nl_stats_test_SOURCES = nl_stats_test.cpp
nl_stats_test_LDADD = libnlstats.la


bin_PROGRAMS += tuntap_test
tuntap_test_SOURCES = tuntap_test.cpp


# bin_PROGRAMS += socket_mirror
# socket_mirror_SOURCES = socket_mirror.cpp

bin_PROGRAMS += nllist






LDADD = -lm

