lib_LTLIBRARIES =
include_HEADERS =
bin_PROGRAMS    =

lib_LTLIBRARIES += libdarr-resize.la
libdarr_resize_la_SOURCES = darr-resize.c
libdarr_resize_la_LIBADD = -lm
include_HEADERS += darr-resize.h

lib_LTLIBRARIES += libdarr.la
libdarr_la_SOURCES = darr.c
#libdarr_la_LIBADD = -lm
include_HEADERS += darr.h
bin_PROGRAMS += darr
darr_SOURCES = darr-test.c
#darr_LDADD = libdarr.la -lm
darr_LDADD = libdarr.la libdarr-resize.la

AM_CPPFLAGS = -D__STDC_INT64__
