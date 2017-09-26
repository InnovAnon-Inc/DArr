lib_LTLIBRARIES =
include_HEADERS =
bin_PROGRAMS    =

lib_LTLIBRARIES += libdarr.la
libdarr_la_SOURCES = darr.c
#libdarr_la_LIBADD = -lm
include_HEADERS += darr.h
bin_PROGRAMS += darr
darr_SOURCES = darr-test.c
#darr_LDADD = libdarr.la -lm
darr_LDADD = libdarr.la

AM_CPPFLAGS = -D__STDC_INT64__
