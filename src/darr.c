#if HAVE_CONFIG_H
#include <config.h>
#endif

#define _POSIX_C_SOURCE 200112L
#define __STDC_VERSION__ 200112L

#define NDEBUG 1

#ifndef NDEBUG
#include <stdio.h>
#endif

/*#include <math.h>*/

#include <darr.h>

#ifdef TEST
typedef struct {
   size_t esz;
   size_t maxn;
   size_t n;
   void *restrict data;
} darr_t;
#endif

__attribute__ ((nonnull (1), nothrow))
void init_darr (darr_t *restrict darr) {}

__attribute__ ((leaf, nonnull (1), nothrow))
void init_darr2 (darr_t *restrict darr, size_t maxn) {}

__attribute__ ((nonnull (1), nothrow))
darr_t *alloc_darr () {}

__attribute__ ((leaf, nonnull (1), nothrow))
darr_t *alloc_darr2 (size_t maxn) {}

__attribute__ ((leaf, nonnull (1), nothrow))
void ensure_cap_darr (darr_t *restrict darr, size_t n) {}

__attribute__ ((leaf, nonnull (1), nothrow))
void trim_cap_darr (darr_t *restrict darr, size_t n) {}

__attribute__ ((leaf, nonnull (1, 2), nothrow))
void add_darr (darr_t *restrict darr, void const *restrict e) {}

__attribute__ ((leaf, nonnull (1, 2), nothrow))
void adds_darr (darr_t *restrict darr, void const *restrict e, size_t n) {}

__attribute__ ((leaf, nonnull (1, 2), nothrow))
void remove_darr (darr_t *restrict darr, void *restrict e) {}

__attribute__ ((leaf, nonnull (1, 2), nothrow))
void removes_darr (darr_t *restrict darr, void *restrict e, size_t n) {}

__attribute__ ((const, leaf, nonnull (1), nothrow))
void free_darr (darr_t *restrict darr) {}
