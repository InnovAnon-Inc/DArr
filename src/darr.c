#if HAVE_CONFIG_H
#include <config.h>
#endif

#define _POSIX_C_SOURCE 200112L
#define __STDC_VERSION__ 200112L

/*#define NDEBUG 1*/

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
   return init_darr2 (darr, esz, maxn, resizecb, cbargs);
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
   if (n <= darr->maxn) return 0;
   new_n = darr->resizecb (n, darr->cbargs);
#ifndef NDEBUG
   printf ("DARRSZN (darr, %d): %d\n", (int) new_n, (int) DARRSZN (darr, new_n)); fflush (stdout);
#endif
   new_data = realloc (darr->data, DARRSZN (darr, new_n));
   error_check (new_data == NULL) return -1;
   darr->data = new_data;
   darr->maxn = new_n;
   return 0;
}

__attribute__ ((leaf, nonnull (1), nothrow, warn_unused_result))
int trim_cap_darr (darr_t *restrict darr, size_t n) {
   void *restrict new_data;
   if (n >= darr->maxn) return 0;
#ifndef NDEBUG
   printf ("DARRSZN (darr, %d): %d\n", (int) n, (int) DARRSZN (darr, n)); fflush (stdout);
#endif
   new_data = realloc (darr->data, DARRSZN (darr, max (n, 1)));
   error_check (new_data == NULL) return -1;
   darr->data = new_data;
   darr->maxn = max (n, 1);
   darr->n    = min (darr->n, darr->maxn);
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

__attribute__ ((nonnull (1, 3), nothrow, warn_unused_result))
int insert_front_darr (darr_t *restrict darr, size_t i,
   void const *restrict e) {
   void *dest;
   void *src;
   error_check (ensure_cap_darr (darr, darr->n + 1) != 0) return -1;
   dest = (void *) ((char *) (darr->data) + (i + 1) * darr->esz);
   src  = (void *) ((char *) (darr->data) + (i + 0) * darr->esz);
   if (i != darr->n)
      memmove (dest, src, (darr->n - i) * darr->esz);
   (void) memcpy (src, e, darr->esz);
   darr->n++;
   return 0;
}

__attribute__ ((nonnull (1, 3), nothrow, warn_unused_result))
int inserts_front_darr (darr_t *restrict darr, size_t i,
   void const *restrict e, size_t n) {
   void *dest;
   void *src;
   size_t mv;
   error_check (ensure_cap_darr (darr, darr->n + n) != 0) return -1;
   dest = (void *) ((char *) (darr->data) + (i + n) * darr->esz);
   src  = (void *) ((char *) (darr->data) + (i + 0) * darr->esz);

   if (i + n > darr->n)
      mv = darr->n - i;
   else
      mv = darr->n - i + n;

   memmove (dest, src, mv * darr->esz);
   (void) memcpy (src, e, darr->esz * n);
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

__attribute__ ((leaf, nonnull (1, 3), nothrow))
void remove_front_darr (darr_t *restrict darr, size_t i,
   void *restrict e) {
   void *dest;
   void *src;
   dest = (void *) ((char *) (darr->data) + (i + 1) * darr->esz);
   src  = (void *) ((char *) (darr->data) + (i + 0) * darr->esz);
   memcpy (e, src, darr->esz);
   if (i != darr->n)
      memmove (src, dest, (darr->n - i - 1) * darr->esz);
   darr->n--;
}

__attribute__ ((leaf, nonnull (1, 3), nothrow))
void removes_front_darr (darr_t *restrict darr, size_t i,
   void *restrict e, size_t n) {
   void *dest;
   void *src;
   dest = (void *) ((char *) (darr->data) + (i + n) * darr->esz);
   src  = (void *) ((char *) (darr->data) + (i + 0) * darr->esz);
   memcpy (e, src, darr->esz * n);
   /* TODO i + n != darr->n*/
   if (i + n < darr->n)
      memmove (src, dest, (darr->n - i - n) * darr->esz);
   darr->n -= n;
}

__attribute__ ((leaf, nonnull (1), nothrow))
void free_darr (darr_t *restrict darr) {
   free (darr->data);
}
