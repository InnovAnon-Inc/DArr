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
   darr_resize_cb_t resizecb, void const *restrict cbargs) {
   /* if your resizecb is geometric, then you may have to use
    * init_darr2 (darr, esz, resizecb(1), resizecb) */
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wtraditional-conversion"
   size_t maxn = resizecb (0, cbargs);
	#pragma GCC diagnostic pop
   return alloc_darr2 (darr, esz, maxn, resizecb, cbargs);
}

__attribute__ ((leaf, nonnull (1, 4), nothrow, warn_unused_result))
int alloc_darr2 (darr_t *restrict darr,
   size_t esz, size_t maxn,
   darr_resize_cb_t resizecb, void const *restrict cbargs) {
   darr->n = 0;
   darr->resizecb = resizecb;
      TODO (this looks shady)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
   darr->cbargs   = cbargs;
	#pragma GCC diagnostic pop
   error_check (alloc_array (&(darr->array), esz, maxn) != 0) return -1;
   return 0;
}

/* don't give us a statically allocated data* */
__attribute__ ((nonnull (1, 2, 4), nothrow))
void init_darr (darr_t *restrict darr,
   void *restrict data, size_t esz,
   darr_resize_cb_t resizecb, void const *restrict cbargs) {
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wtraditional-conversion"
   size_t maxn = resizecb (0, cbargs);
	#pragma GCC diagnostic pop
   init_darr2 (darr, data, esz, maxn, resizecb, cbargs);
}

__attribute__ ((leaf, nonnull (1, 2, 5), nothrow))
void init_darr2 (darr_t *restrict darr,
   void *restrict data, size_t esz, size_t maxn,
   darr_resize_cb_t resizecb, void const *restrict cbargs) {
   init_array (&(darr->array), data, esz, maxn);
   darr->n = 0;
   darr->resizecb = resizecb;
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
   darr->cbargs   = cbargs;
	#pragma GCC diagnostic pop
}

__attribute__ ((leaf, nonnull (1), nothrow, warn_unused_result))
int ensure_cap_darr (darr_t *restrict darr, size_t n) {
   void *restrict new_data;
   size_t new_n;
   if (n <= darr->array.n) return 0;
   TODO (max is probably unnecessary here)
   new_n = max (darr->resizecb (n, darr->cbargs), 1);
   /*error_check (new_n < n) return -1;*/
#ifndef NDEBUG
   printf ("DARRSZN (darr, %d): %d\n", (int) new_n, (int) DARRSZN (darr, new_n)); fflush (stdout);
#endif
   error_check (realloc_array (&(darr->array), new_n) != 0) return -2;
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
   error_check (realloc_array (&(darr->array), new_n) != 0) return -2;
   darr->n    = min (darr->n, new_n);
   return 0;
}

__attribute__ ((leaf, nonnull (1, 2), nothrow))
void insert_rear_darr (darr_t *restrict darr, void const *restrict e) {
   /*error_check (ensure_cap_darr (darr, darr->n + 1) != 0) return -1;*/
   set_array (&(darr->array), darr->n, e);
   darr->n++;
}

__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result))
int ez_insert_rear_darr (darr_t *restrict darr, void const *restrict e) {
   error_check (ensure_cap_darr (darr, darr->n + 1) != 0) return -1;
   insert_rear_darr (darr, e);
   return 0;
}

__attribute__ ((leaf, nonnull (1, 2), nothrow))
void inserts_rear_darr (darr_t *restrict darr,
   void const *restrict e, size_t n) {
   sets_array (&(darr->array), darr->n, e, n);
   darr->n += n;
}

__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result))
int ez_inserts_rear_darr (darr_t *restrict darr,
   void const *restrict e, size_t n) {
   error_check (ensure_cap_darr (darr, darr->n + n) != 0) return -1;
   inserts_rear_darr (darr, e, n);
   return 0;
}

__attribute__ ((leaf, nonnull (1, 3), nothrow))
void insert_front_darr (darr_t *restrict darr, size_t i,
   void const *restrict e) {
   /*if (i != darr->n)*/
      mvs_array (&(darr->array), i + 0, i + 1, darr->n - i);
   set_array (&(darr->array), i + 0, e);
   darr->n++;
}

__attribute__ ((nonnull (1, 3), nothrow, warn_unused_result))
int ez_insert_front_darr (darr_t *restrict darr, size_t i,
   void const *restrict e) {
   error_check (ensure_cap_darr (darr, darr->n + 1) != 0) return -1;
   insert_front_darr (darr, i, e);
   return 0;
}

__attribute__ ((leaf, nonnull (1, 3), nothrow))
void inserts_front_darr (darr_t *restrict darr, size_t i,
   void const *restrict e, size_t n) {
   size_t mv;

   /*if (i + n > darr->n)
      mv = darr->n - i;
   else
      mv = darr->n - i + n;*/
      mv = darr->n - i;

   /*if (mv != 0)*/
   mvs_array (&(darr->array), i + 0, i + n, mv);
   sets_array (&(darr->array), i + 0, e, n);
   darr->n += n;
}

__attribute__ ((nonnull (1, 3), nothrow, warn_unused_result))
int ez_inserts_front_darr (darr_t *restrict darr, size_t i,
   void const *restrict e, size_t n) {
   error_check (ensure_cap_darr (darr, darr->n + n) != 0) return -1;
   inserts_front_darr (darr, i, e, n);
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

__attribute__ ((leaf, nonnull (1, 2), nothrow))
void make_gaps_darr (darr_t *restrict darr,
   size_t const is[], size_t n) {
   size_t i, prev;

   for (i = n, prev = darr->n; i != 0; i--, prev = is[i])
      mvs_array (&(darr->array),
         is[i - 1], is[i - 1] + i, prev - is[i - 1]);

   darr->n += n;
}

__attribute__ ((nonnull (1, 2, 3), nothrow))
void inserts_darr (darr_t *restrict darr, size_t const is[],
   void const *restrict e, size_t n) {
   array_t tmp;
   size_t i;
   void const *restrict E;

   make_gaps_darr (darr, is, n);

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wdiscarded-qualifiers"
   init_array (&tmp, e, darr->array.esz, n);
	#pragma GCC diagnostic pop

	#pragma GCC ivdep
   for (i = 0; i != n; i++) {
      E = index_array (&tmp, i);
      set_array (&(darr->array), is[i] + i, E);
   }

   /*
   for (i = n; i != 0; i--) {
      mvs_array (&(darr->array), is[i - 1], i, darr->n - (i - 1));
      E = index_array (&tmp, n - i);
      set_array (&(darr->array), is[i - 1] + (i - 1), E);
   }

   darr->n += n;
   */
}

__attribute__ ((nonnull (1, 2, 3), nothrow, warn_unused_result))
int ez_inserts_darr (darr_t *restrict darr, size_t const is[],
   void const *restrict e, size_t n) {
   error_check (ensure_cap_darr (darr, darr->n + n) != 0) return -1;
   inserts_darr (darr, is, e, n);
   return 0;
}

__attribute__ ((leaf, nonnull (1, 2), nothrow))
void unmake_gaps_darr (darr_t *restrict darr,
   size_t const is[], size_t n) {
   size_t i;

   for (i = 0; i != n - 1; i++)
      mvs_array (&(darr->array), is[i], is[i] + i, is[i + 1] - is[i]);
   mvs_array (&(darr->array), is[i], is[i] + i, darr->n - is[i]);

   darr->n -= n;
}

__attribute__ ((nonnull (1, 2, 3), nothrow))
void removes_darr (darr_t *restrict darr, size_t const is[],
   void *restrict e, size_t n) {
   array_t tmp;
   size_t i;
   void *restrict E;

   init_array (&tmp, e, darr->array.esz, n);

	#pragma GCC ivdep
   for (i = 0; i != n; i++) {
      E = index_array (&tmp, i);
      get_array (&(darr->array), is[i] /*+ i*/, E);
   }

   unmake_gaps_darr (darr, is, n);
}

__attribute__ ((leaf, nonnull (1), nothrow, pure, warn_unused_result))
bool isfull_darr (darr_t const *restrict darr) {
   return darr->n == darr->array.n;
}

__attribute__ ((leaf, nonnull (1), nothrow, pure, warn_unused_result))
bool isempty_darr (darr_t const *restrict darr) {
   return darr->n == 0;
}

__attribute__ ((leaf, nonnull (1), nothrow))
void free_darr (darr_t const *restrict darr) {
   free_array (&(darr->array));
}
