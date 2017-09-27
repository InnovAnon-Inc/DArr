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

/*#include <math.h>*/

#include <darr.h>

__attribute__ ((nonnull (1, 3), nothrow, warn_unused_result))
int init_darr (darr_t *restrict darr, size_t esz,
   darr_resize_cb_t resizecb, void *restrict cbargs) {
   /* if your resizecb is geometric, then you may have to use
    * init_darr2 (darr, esz, resizecb(1), resizecb) */
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wtraditional-conversion"
   size_t maxn = resizecb (0, cbargs);
	#pragma GCC diagnostic pop
   return init_darr2 (darr, esz, maxn, resizecb);
}

__attribute__ ((leaf, nonnull (1, 4), nothrow, warn_unused_result))
int init_darr2 (darr_t *restrict darr, size_t esz, size_t maxn,
   darr_resize_cb_t resizecb, void *restrict cbargs) {
   darr->esz  = esz;
   darr->maxn = maxn;
   darr->n    = 0;
   darr->resizecb = resizecb;
   darr->cbargs   = cbargs;
   darr->data = malloc (DARRSZ (darr));
   error_check (darr->data == NULL) return -1;
   return 0;
}

__attribute__ ((leaf, nonnull (1), nothrow, warn_unused_result))
int ensure_cap_darr (darr_t *restrict darr, size_t n) {
   void *restrict new_data;
   size_t new_n;
   if (n <= darr->maxn) return;
   new_n = darr->resizecb (n, darr->cbargs);
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

__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result))
int insert_rear_darr (darr_t *restrict darr, void const *restrict e) {
   error_check (ensure_cap_darr (darr, darr->n + 1) != 0) return -1;
   (void) memcpy ((void *) ((char *) (darr->data) + darr->n * darr->esz),
      e, darr->esz);
   darr->n++;
   return 0;
}

__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result))
int inserts_rear_darr (darr_t *restrict darr,
   void const *restrict e, size_t n) {
   error_check (ensure_cap_darr (darr, darr->n + n) != 0) return -1;
   (void) memcpy ((void *) ((char *) (darr->data) + darr->n * darr->esz),
      e, darr->esz * n);
   darr->n += n;
   return 0;
}

__attribute__ ((leaf, nonnull (1, 2), nothrow))
void remove_rear_darr (darr_t *restrict darr, void *restrict e) {
   memcpy (e,
      (void *) ((char *) (darr->data) + (darr->n - 1) * darr->esz),
      darr->esz);
   darr->n--;
}

__attribute__ ((leaf, nonnull (1, 2), nothrow))
void removes_rear_darr (darr_t *restrict darr,
   void *restrict e, size_t n) {
   memcpy (e,
      (void *) ((char *) (darr->data) + (darr->n - n) * darr->esz),
      darr->esz * n);
   darr->n -= n;
}

__attribute__ ((leaf, nonnull (1), nothrow))
void free_darr (darr_t *restrict darr) {
   free (darr->data);
}
