AM_CPPFLAGS = -D__STDC_INT64__

#if USE_SUBMODULES
#SUBDIRS = ../glitter
#AM_CPPFLAGS += -I $(top_srcdir)/glitter/src
#
#SUBDIRS += ../Array
#AM_CPPFLAGS += -I $(top_srcdir)/Array/src
#endif

lib_LTLIBRARIES =
include_HEADERS =
bin_PROGRAMS    =

#include Makefile-LIBADD.inc
dist_bin_SCRIPTS = libdarr_config libdarr_resize_config darr_test_config

lib_LTLIBRARIES += libdarr-resize.la
#libdarr_resize_la_LIBADD = $(LIBDARR_RESIZE_LIBADD)
libdarr_resize_la_LIBADD = @LIBDARR_RESIZE_LIBADD@
include_HEADERS += darr-resize.h

lib_LTLIBRARIES += libdarr.la
#libdarr_la_LIBADD = $(LIBDARR_LIBADD)
libdarr_la_LIBADD = @LIBDARR_LIBADD@
include_HEADERS += darr.h
bin_PROGRAMS += darr-test
#darr_test_LDADD  = libdarr.la $(LIBDARR_LIBADD)
#darr_test_LDADD += libdarr-resize.la $(LIBDARR_RESIZE_LIBADD)
#darr_test_LDADD = $(DARR_TEST_LIBADD)
darr_test_LDADD = @DARR_TEST_LIBADD@
#EXTRA_darr_test_DEPENDENCIES = libdarr.la libdarr-resize.la
EXTRA_darr_test_DEPENDENCIES = libdarr-resize.la libdarr.la
