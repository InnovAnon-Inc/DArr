AM_CPPFLAGS = -D__STDC_INT64__

lib_LTLIBRARIES =
include_HEADERS =
bin_PROGRAMS    =

lib_LTLIBRARIES += libdarr.la
libdarr_la_CPPFLAGS = $(GLITTER_CFLAGS) $(ARRAY_CFLAGS) $(MMALLOC_CFLAGS)
libdarr_la_LIBADD   = $(GLITTER_LIBS)   $(ARRAY_LIBS)   $(MMALLOC_LIBS)
include_HEADERS += darr.h

lib_LTLIBRARIES += libdarr-resize.la
libdarr_resize_la_CPPFLAGS =        $(GLITTER_CFLAGS)
libdarr_resize_la_LIBADD   = -ldarr $(GLITTER_LIBS) -lm
include_HEADERS += darr-resize.h
EXTRA_libdarr_resize_la_DEPENDENCIES = libdarr.la

bin_PROGRAMS += darr-test
darr_test_CPPFLAGS =                      $(GLITTER_CFLAGS) $(ARRAY_CFLAGS) $(MMALLOC_CFLAGS)
darr_test_LDADD    = -ldarr-resize -ldarr $(GLITTER_LIBS)   $(ARRAY_LIBS)   $(MMALLOC_LIBS) -lm
EXTRA_darr_test_DEPENDENCIES = libdarr.la libdarr-resize.la

