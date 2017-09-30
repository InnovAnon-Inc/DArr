LIBDARR_RESIZE_LIBADD = -lm

include $(top_srcdir)/Array/src/Makefile-LIBADD.inc
#include $(top_srcdir)/swap/src/Makefile-LIBADD.inc

if USE_SUBMODULES
LIBDARR_LIBADD  = ../Array/src/.libs/libarray.la $(LIBARRAY_LIBADD)
#LIBDARR_LIBADD += ../swap/src/.libs/libswap.la $(LIBSWAP_LIBADD)
else
LIBDARR_LIBADD  = -larray $(LIBARRAY_LIBADD)
#LIBDARR_LIBADD += -lswap $(LIBSWAP_LIBADD)
endif
