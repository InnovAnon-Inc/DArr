#include $(top_srcdir)/swap/src/Makefile-LIBADD.inc
#include $(top_srcdir)/MultiMalloc/src/Makefile-LIBADD.inc
include ../swap/src/Makefile-LIBADD.inc
include ../MultiMalloc/src/Makefile-LIBADD.inc

#if USE_SUBMODULES
#LIBARRAY_LIBADD  = ../swap/src/.libs/libswap.la $(LIBSWAP_LIBADD)
#LIBARRAY_LIBADD += ../MultiMalloc/src/.libs/libmmalloc.la $(LIBMMALLOC_LIBADD)
#else
LIBARRAY_LIBADD = -lswap $(LIBSWAP_LIBADD)
LIBARRAY_LIBADD += -lmmalloc $(LIBMMALLOC_LIBADD)
#endif
