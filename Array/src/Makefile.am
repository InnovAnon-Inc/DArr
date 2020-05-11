AM_CPPFLAGS = -D__STDC_INT64__

#if USE_SUBMODULES
#SUBDIRS = ../glitter
#AM_CPPFLAGS += -I $(top_srcdir)/glitter/src
#
#SUBDIRS += ../swap
#AM_CPPFLAGS += -I $(top_srcdir)/swap/src
#
#SUBDIRS += ../MultiMalloc
#AM_CPPFLAGS += -I $(top_srcdir)/MultiMalloc/src
#endif

lib_LTLIBRARIES =
include_HEADERS =
bin_PROGRAMS    =

include Makefile-LIBADD.inc

lib_LTLIBRARIES += libarray.la
libarray_la_LIBADD = $(LIBARRAY_LIBADD)
include_HEADERS += array.h
bin_PROGRAMS += array-test
array_test_LDADD = libarray.la $(LIBARRAY_LIBADD)
