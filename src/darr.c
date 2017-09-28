#if HAVE_CONFIG_H
#include <config.h>
#endif

#define _POSIX_C_SOURCE 200112L
#define __STDC_VERSION__ 200112L

#define NDEBUG 1

#ifndef NDEBUG
#include <stdio.h>
#endif

#include <darr.h>

__attribute__ ((nonnull (1, 3), nothrow, warn_unused_result))
int alloc_darr (darr_t *restrict darr, size_t esz,
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
int alloc_darr2 (darr_t *restrict darr
   size_t esz, size_t maxn,
   darr_resize_cb_t resizecb, void *restrict cbargs) {
   darr->n = 0;
   darr->resizecb = resizecb;
   darr->cbargs   = cbargs;
   error_check (alloc_array (&(darr->array), esz, maxn) != 0) return -1;
   return 0;
}

/* don't give us a statically allocated data* */
__attribute__ ((nonnull (1, 2, 4), nothrow))
void init_darr (darr_t *restrict darr,
   void *restrict data, size_t esz,
   darr_resize_cb_t resizecb, void *restrict cbargs) {
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wtraditional-conversion"
   size_t maxn = resizecb (0, cbargs);
	#pragma GCC diagnostic pop
   init_darr2 (darr, data, esz, maxn, resizecb, cbargs);
}

__attribute__ ((leaf, nonnull (1, 2, 5), nothrow))
void init_darr2 (darr_t *restrict darr,
   void *restrict data, size_t esz, size_t maxn,
   darr_resize_cb_t resizecb, void *restrict cbargs) {
   init_array (&(darr->array), data, esz, maxn);
   darr->n = 0;
   darr->resizecb = resizecb;
   darr->cbargs   = cbargs;
}

__attribute__ ((leaf, nonnull (1), nothrow, warn_unused_result))
int ensure_cap_darr (darr_t *restrict darr, size_t n) {
   void *restrict new_data;
   size_t new_n;
   if (n <= darr->array.n) return 0;
   TODO (max is probably unnecessary here)
   new_n = max (darr->resizecb (n, darr->cbargs), 1);
   error_check (new_n < n) return -1;
#ifndef NDEBUG
   printf ("DARRSZN (darr, %d): %d\n", (int) new_n, (int) DARRSZN (darr, new_n)); fflush (stdout);
#endif
   new_data = realloc (darr->array.data, DARRSZN (darr, new_n));
   error_check (new_data == NULL) return -1;
   darr->array.data = new_data;
   darr->array.n = new_n;
   return 0;
}

__attribute__ ((leaf, nonnull (1), nothrow, warn_unused_result))
int trim_cap_darr (darr_t *restrict darr, size_t n) {
   void *restrict new_data;
   size_t new_n;
   if (n >= darr->array.n) return 0;
#ifndef NDEBUG
   printf ("DARRSZN (darr, %d): %d\n", (int) n, (int) DARRSZN (darr, n)); fflush (stdout);
#endif
   TODO (new_n = darr->resizecb (n, darr->cbargss))
   new_n = max (n, 1);
   new_data = realloc (darr->array.data, DARRSZN (darr, new_n));
   error_check (new_data == NULL) return -1;
   darr->array.data = new_data;
   darr->array.n = new_n;
   darr->n    = min (darr->n, new_n);
   return 0;
}

__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result))
int insert_rear_darr (darr_t *restrict darr, void const *restrict e) {
   error_check (ensure_cap_darr (darr, darr->n + 1) != 0) return -1;
   set_array (&(darr->array), darr->n, e);
   darr->n++;
   return 0;
}

__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result))
int inserts_rear_darr (darr_t *restrict darr,
   void const *restrict e, size_t n) {
   error_check (ensure_cap_darr (darr, darr->n + n) != 0) return -1;
   sets_array (&(darr->array), darr->n, e, n);
   darr->n += n;
   return 0;
}

__attribute__ ((nonnull (1, 3), nothrow, warn_unused_result))
int insert_front_darr (darr_t *restrict darr, size_t i,
   void const *restrict e) {
   error_check (ensure_cap_darr (darr, darr->n + 1) != 0) return -1;
   /*if (i != darr->n)*/
      mvs_array (&(darr->array), i + 0, i + 1, darr->n - i);
   set_array (&(darr->array), i + 0, e);
   darr->n++;
   return 0;
}

__attribute__ ((nonnull (1, 3), nothrow, warn_unused_result))
int inserts_front_darr (darr_t *restrict darr, size_t i,
   void const *restrict e, size_t n) {
   size_t mv;
   error_check (ensure_cap_darr (darr, darr->n + n) != 0) return -1;

   /*if (i + n > darr->n)
      mv = darr->n - i;
   else
      mv = darr->n - i + n;*/
      mv = darr->n - i;

   /*if (mv != 0)*/
   mvs_array (&(darr->array), i + 0, i + n, mv);
   sets_array (&(darr->array), i + 0, e, n);
   darr->n += n;
   return 0;
}

__attribute__ ((leaf, nonnull (1, 2), nothrow))
void remove_rear_darr (darr_t *restrict darr, void *restrict e) {
   get_array (&(darr->array), darr->n - 1, e);
   darr->n--;
}

__attribute__ ((leaf, nonnull (1, 2), nothrow))
void removes_rear_darr (darr_t *restrict darr,
   void *restrict e, size_t n) {
   gets_array (&(darr->array), darr->n - n, e, n);
   darr->n -= n;
}

__attribute__ ((leaf, nonnull (1, 3), nothrow))
void remove_front_darr (darr_t *restrict darr, size_t i,
   void *restrict e) {
   get_array (&(darr->array), i + 0, e);
   if (i != darr->n)
      mvs_array (&(darr->array), i + 1, i + 0, darr->n - i - 1);
   darr->n--;
}

__attribute__ ((leaf, nonnull (1, 3), nothrow))
void removes_front_darr (darr_t *restrict darr, size_t i,
   void *restrict e, size_t n) {
   sets_array (&(darr->array), i + 0, e, n);
   /* TODO i + n != darr->n*/
   if (i + n < darr->n)
      mvs_array (&(darr->array), i + n, i + 0, darr->n - i - n);
   darr->n -= n;
}

__attribute__ ((leaf, nonnull (1), nothrow))
void free_darr (darr_t *restrict darr) {
   free_array (&(darr->array));
}
