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
typedef __attribute__ ((const, warn_unused_result))
size_t (*darr_resize_cb_t (size_t esz, size_t n, size_t inc);

typedef struct {
   size_t esz;
   size_t maxn;
   size_t n;
   void *restrict data;
   darr_resize_cb_t resizecb;
} darr_t;
#endif

__attribute__ ((nonnull (1, 3), nothrow, warn_unused_result))
int init_darr (darr_t *restrict darr, size_t esz,
   darr_resize_cb_t resizecb) {
   /* if your resizecb is geometric, then you may have to use
    * init_darr2 (darr, esz, resizecb(1), resizecb) */
   return init_darr2 (darr, esz, resizecb (0), resizecb);
}

__attribute__ ((leaf, nonnull (1, 4), nothrow, warn_unused_result))
int init_darr2 (darr_t *restrict darr, size_t esz, size_t maxn,
   darr_resize_cb_t resizecb) {
   darr->esz  = esz;
   darr->maxn = maxn;
   darr->n    = 0;
   darr->resizecb = resizecb;
   darr->data = malloc (DARRSZ (darr));
   error_check (darr->data == NULL) return -1;
   return 0;
}

__attribute__ ((leaf, nonnull (1), nothrow, warn_unused_result))
int ensure_cap_darr (darr_t *restrict darr, size_t n) {
   void *restrict new_data;
   size_t new_n;
   if (n <= darr->maxn) return;
   new_n = darr->resizecb (n);
   new_data = realloc (darr->data, DARRSZN (darr, new_n));
   error_check (new_data == NULL) return -1;
   darr->data = new_data;
   darr->maxn = new_n;
   return 0;
}

__attribute__ ((leaf, nonnull (1), nothrow, warn_unused_result))
int trim_cap_darr (darr_t *restrict darr, size_t n) {
   void *restrict new_data;
   if (n >= darr->maxn) return;
   new_data = realloc (darr->data, DARRSZN (darr, n));
   error_check (new_data == NULL) return -1;
   darr->data = new_data;
   darr->maxn = n;
   return 0;
}

__attribute__ ((nonnull (1, 2), nothrow))
void add_darr (darr_t *restrict darr, void const *restrict e) {}

__attribute__ ((nonnull (1, 2), nothrow))
void adds_darr (darr_t *restrict darr, void const *restrict e, size_t n) {}

__attribute__ ((nonnull (1, 2), nothrow))
void remove_darr (darr_t *restrict darr, void *restrict e) {}

__attribute__ ((nonnull (1, 2), nothrow))
void removes_darr (darr_t *restrict darr, void *restrict e, size_t n) {}

__attribute__ ((const, leaf, nonnull (1), nothrow))
void free_darr (darr_t *restrict darr) {}
