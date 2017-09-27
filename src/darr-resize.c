#if HAVE_CONFIG_H
#include <config.h>
#endif

#define _POSIX_C_SOURCE 200112L
#define __STDC_VERSION__ 200112L

#define NDEBUG 1

#ifndef NDEBUG
#include <stdio.h>
#endif
#include <stdlib.h>
#include <string.h>

#include <math.h>

#include <darr-resize.h>

__attribute__ ((leaf, warn_unused_result))
size_t darr_resize_exact (size_t inc,
   void *restrict unused) {
   return inc;
}

__attribute__ ((leaf, warn_unused_result))
size_t darr_resize_geometric (size_t inc,
   void *restrict _factor) {
   double const *restrict factor = (double *restrict) _factor;
   double tmp = ceil (log ((double) inc) / log (*factor));
   double ret = pow (*factor, tmp);
   return (size_t) ret;
}

__attribute__ ((leaf, warn_unused_result))
size_t darr_resize_linear (size_t inc,
   void *restrict _factor) {
   size_t const *restrict factor = (size_t const *restrict) _factor;
   double tmp = ceil ((double) inc / (double) *factor);
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wfloat-conversion"
   double ret = (double) (*factor) * tmp;
	#pragma GCC diagnostic pop
   return (size_t) ret;
}
