AM_CPPFLAGS = -D__STDC_INT64__

SUBDIRS = ../glitter
AM_CPPFLAGS += -I $(top_srcdir)/glitter/src

SUBDIRS += ../Array
AM_CPPFLAGS += -I $(top_srcdir)/Array/src

lib_LTLIBRARIES =
include_HEADERS =
bin_PROGRAMS    =

LIBDARR_RESIZE_LIBADD = -lm

lib_LTLIBRARIES += libdarr-resize.la
libdarr_resize_la_LIBADD = $(LIBDARR_RESIZE_LIBADD)
include_HEADERS += darr-resize.h

LIBDARR_LIBADD  = ../Array/src/.libs/libarray.la $(LIBARRAY_LIBADD)
LIBDARR_LIBADD += ../swap/src/.libs/libswap.la $(LIBSWAP_LIBADD)

lib_LTLIBRARIES += libdarr.la
libdarr_la_SOURCES = darr.c
libdarr_la_LIBADD = $(LIBDARR_LIBADD)
include_HEADERS += darr.h
bin_PROGRAMS += darr
darr_SOURCES = darr-test.c
darr_LDADD  = libdarr.la $(LIBDARR_LIBADD)
darr_LDADD += libdarr-resize.la $(LIBDARR_RESIZE_LIBADD)
