#if HAVE_CONFIG_H
#include <config.h>
#endif

#define _POSIX_C_SOURCE 200112L
#define __STDC_VERSION__ 200112L

#include <assert.h>
#ifndef NDEBUG
#include <stdio.h>
#endif
#include <stdint.h>
#include <stdlib.h>

#include <mmalloc.h>

#include <darr.h>

__attribute__ ((const, leaf, nothrow, warn_unused_result))
size_t darrsz (size_t esz, size_t n) {
   return sizeof (darr_t) + datasz (esz, n);
}

__attribute__ ((nonnull (1), nothrow, pure, warn_unused_result))
size_t darrsz2 (darr_t const *restrict darr) {
   return darrsz (darr->array.esz, darr->array.n);
}

__attribute__ ((/*alloc_align (1),*/ /*alloc_size (1, 2),*/ /*malloc,*/
	nonnull (2, 3), nothrow, warn_unused_result))
darr_t *ez_alloc_darr11 (size_t esz,
   darr_resize_cb_t resizecb, void const *restrict cbargs) {
   size_t maxn = resizecb ((size_t) 0, cbargs);
   return ez_alloc_darr12 (esz, maxn, resizecb, cbargs);
}

__attribute__ ((/*alloc_align (1),*/ /*alloc_size (1, 2),*/ /*malloc,*/
	nonnull (3, 4), nothrow, warn_unused_result))
darr_t *ez_alloc_darr12 (size_t esz, size_t maxn,
   darr_resize_cb_t resizecb, void const *restrict cbargs) {
      /*
   void *restrict combined[2];
	size_t eszs[2];
	darr_t *restrict darr;
	void *restrict data;

	eszs[0] = sizeof (darr_t);
	eszs[1] = datasz  (esz, maxn);
	error_check (mmalloc (combined, eszs,
		eszs[0] + eszs[1], ARRSZ (eszs)) != 0)
		return NULL;
	darr = (darr_t *restrict) combined[0];
	data = (void *restrict)   combined[1];

   init_darr2 (darr, data, esz, maxn, resizecb, cbargs);
	return darr;
   */
   void *restrict *restrict combined[2];
	size_t eszs[2];
	darr_t *restrict caq;
	void *restrict data;

	eszs[0] = sizeof (darr_t);
	eszs[1] = datasz  (esz, maxn);
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wstrict-aliasing"
   combined[0] = (void *restrict *restrict) &caq;
   combined[1] = (void *restrict *restrict) &data;
	#pragma GCC diagnostic pop
	error_check (mmalloc2 (combined, eszs,
		eszs[0] + eszs[1], ARRSZ (eszs)) != 0)
		return NULL;

   init_darr2 (caq, data, esz, maxn, resizecb, cbargs);
	return caq;
}

__attribute__ ((leaf, nonnull (1), nothrow))
void ez_free_darr1 (darr_t *restrict darr) {
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wstrict-aliasing"
   mfree ((void *restrict) darr);
	#pragma GCC diagnostic pop
   /*free (darr);*/
}

__attribute__ ((/*alloc_align (1),*/ /*alloc_size (1, 2),*/ /*malloc,*/
	nonnull (2, 3), nothrow, warn_unused_result))
darr_t *ez_alloc_darr21 (size_t esz,
   darr_resize_cb_t resizecb, void const *restrict cbargs) {
   size_t maxn = resizecb ((size_t) 0, cbargs);
   return ez_alloc_darr22 (esz, maxn, resizecb, cbargs);
}

__attribute__ ((/*alloc_align (1),*/ /*alloc_size (1, 2),*/ /*malloc,*/
	nonnull (3, 4), nothrow, warn_unused_result))
darr_t *ez_alloc_darr22 (size_t esz, size_t maxn,
   darr_resize_cb_t resizecb, void const *restrict cbargs) {
	darr_t *restrict array = malloc (sizeof (darr_t));
	error_check (array == NULL) return NULL;
	error_check (alloc_darr2 (array, esz, maxn, resizecb, cbargs) != 0) {
		free (array);
		return NULL;
	}
	return array;
}

__attribute__ ((leaf, nonnull (1), nothrow))
void ez_free_darr2 (darr_t *restrict array) {
	free_darr (array);
	/*free (array);*/
}

__attribute__ ((nonnull (1, 3), nothrow, warn_unused_result))
int alloc_darr (darr_t *restrict darr, size_t esz,
   darr_resize_cb_t resizecb, void const *restrict cbargs) {
   /* if your resizecb is geometric, then you may have to use
    * init_darr2 (darr, esz, resizecb(1), resizecb) */
   size_t maxn = resizecb ((size_t) 0, cbargs);
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

__attribute__ ((nonnull (1, 2, 4), nothrow))
void init_darr (darr_t *restrict darr,
   void *restrict data, size_t esz,
   darr_resize_cb_t resizecb, void const *restrict cbargs) {
   size_t maxn = resizecb ((size_t) 0, cbargs);
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
   assert (new_n >= n);
   error_check (realloc_array (&(darr->array), new_n) != 0) return -2;
   return 0;
}

__attribute__ ((leaf, nonnull (1), nothrow, warn_unused_result))
int trim_cap_darr (darr_t *restrict darr, size_t n) {
   void *restrict new_data;
   size_t new_n;
   TODO (new_n = darr->resizecb (n, darr->cbargss))
   new_n = max (n, 1);
   if (new_n >= darr->array.n) return 0;
   /*TODO (new_n = darr->resizecb (n, darr->cbargss))
   new_n = max (n, 1);*/
   error_check (realloc_array (&(darr->array), new_n) != 0) return -2;
   darr->n    = min (darr->n, new_n);
   assert (darr->n <= new_n);
   return 0;
}

__attribute__ ((leaf, nonnull (1, 2), nothrow))
void insert_rear_darr (darr_t *restrict darr, void const *restrict e) {
   /*error_check (ensure_cap_darr (darr, darr->n + 1) != 0) return -1;*/
   assert (! isfull_darr (darr));
#ifndef NDEBUG
   fprintf (stderr, "insert rear()\n");
   /*fprintf (stderr, "i    : %d\n", (int) i);*/
   /*fprintf (stderr, "n    : %d\n", (int) n);*/
   /*fprintf (stderr, "i + 1: %d\n", (int) (i + 1));*/
   fprintf (stderr, "curN : %d\n", (int) (darr->n));
   fprintf (stderr, "maxN : %d\n", (int) (darr->array.n));
   /*fprintf (stderr, "mv   : %d\n", (int) mv);*/
#endif
   set_array (&(darr->array), darr->n, e);
   darr->n++;
   assert (darr->n <= darr->array.n);
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
   assert (remaining_space_darr (darr) >= n);
#ifndef NDEBUG
   fprintf (stderr, "inserts rear()\n");
   /*fprintf (stderr, "i    : %d\n", (int) i);*/
   fprintf (stderr, "n    : %d\n", (int) n);
   /*fprintf (stderr, "i + 1: %d\n", (int) (i + 1));*/
   fprintf (stderr, "curN : %d\n", (int) (darr->n));
   fprintf (stderr, "maxN : %d\n", (int) (darr->array.n));
   /*fprintf (stderr, "mv   : %d\n", (int) mv);*/
#endif
   sets_array (&(darr->array), darr->n, e, n);
   darr->n += n;
   assert (darr->n >= n);
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
   size_t mv;
   assert (! isfull_darr (darr));
   if (i + 1 >= darr->n)
      mv = darr->n - i;
   else
      mv = darr->n - (i + 1);
#ifndef NDEBUG
   fprintf (stderr, "insert front()\n");
   fprintf (stderr, "i    : %d\n", (int) i);
   /*fprintf (stderr, "n    : %d\n", (int) n);*/
   fprintf (stderr, "i + 1: %d\n", (int) (i + 1));
   fprintf (stderr, "curN : %d\n", (int) (darr->n));
   fprintf (stderr, "maxN : %d\n", (int) (darr->array.n));
   fprintf (stderr, "mv   : %d\n", (int) mv);
#endif
   mvs_array (&(darr->array), i + 0, i + 1, mv);
   set_array (&(darr->array), i + 0, e);
   darr->n++;
   assert (darr->n <= darr->array.n);
#ifdef TEST
   /*if (i != darr->n)*/
      mvs_array (&(darr->array), i + 0, i + 1, darr->n - i);
   set_array (&(darr->array), i + 0, e);
   darr->n++;
   assert (darr->n <= darr->array.n);
#endif
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
   assert (remaining_space_darr (darr) >= n);
   if (i + n >= darr->n)
      mv = darr->n - i;
   else
      mv = darr->n - (i + n);
#ifndef NDEBUG
   fprintf (stderr, "inserts front()\n");
   fprintf (stderr, "i    : %d\n", (int) i);
   fprintf (stderr, "n    : %d\n", (int) n);
   fprintf (stderr, "i + n: %d\n", (int) (i + n));
   fprintf (stderr, "curN : %d\n", (int) (darr->n));
   fprintf (stderr, "maxN : %d\n", (int) (darr->array.n));
   fprintf (stderr, "mv   : %d\n", (int) mv);
#endif
   mvs_array (&(darr->array), i + 0, i + n, mv);
   sets_array (&(darr->array), i + 0, e, n);
   darr->n += n;
   assert (darr->n >= n);
#ifdef TEST
   if (i + n >= darr->n)
      mv = darr->n - i;
   else
      mv = darr->n - (i + n);
      /*mv = darr->n - i;*/
      /*mv = n;*/
   /*if (mv != 0)*/
   mvs_array (&(darr->array), i + 0, i + n, mv);
   sets_array (&(darr->array), i + 0, e, n);
   darr->n += n;
   assert (darr->n >= n);
#endif
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
   assert (! isempty_darr (darr));
#ifndef NDEBUG
   fprintf (stderr, "remove rear()\n");
   /*fprintf (stderr, "i    : %d\n", (int) i);*/
   /*fprintf (stderr, "n    : %d\n", (int) n);*/
   /*fprintf (stderr, "i + 1: %d\n", (int) (i + 1));*/
   fprintf (stderr, "curN : %d\n", (int) (darr->n));
   fprintf (stderr, "maxN : %d\n", (int) (darr->array.n));
   /*fprintf (stderr, "mv   : %d\n", (int) mv);*/
#endif
   get_array (&(darr->array), darr->n - 1, e);
   darr->n--;
   assert (darr->n != SIZE_MAX);
}

__attribute__ ((leaf, nonnull (1, 2), nothrow))
void removes_rear_darr (darr_t *restrict darr,
   void *restrict e, size_t n) {
   assert (darr->n >= n || n == 0);
#ifndef NDEBUG
   fprintf (stderr, "removes rear()\n");
   /*fprintf (stderr, "i    : %d\n", (int) i);*/
   fprintf (stderr, "n    : %d\n", (int) n);
   /*fprintf (stderr, "i + 1: %d\n", (int) (i + 1));*/
   fprintf (stderr, "curN : %d\n", (int) (darr->n));
   fprintf (stderr, "maxN : %d\n", (int) (darr->array.n));
   /*fprintf (stderr, "mv   : %d\n", (int) mv);*/
#endif
   gets_array (&(darr->array), darr->n - n, e, n);
   darr->n -= n;

#ifdef TEST
#ifndef NDEBUG
   fprintf (stderr, "remaining:  %d\n", (int) remaining_space_darr (darr));
   fprintf (stderr, "amt_to_add: %d\n", (int) n);
#endif
   assert (darr->n >= n || n == 0);
   gets_array (&(darr->array), darr->n - n, e, n);
   darr->n -= n;
   TODO (check underflow)
#endif
}

__attribute__ ((leaf, nonnull (1, 3), nothrow))
void remove_front_darr (darr_t *restrict darr, size_t i,
   void *restrict e) {
   size_t mv;
   assert (! isempty_darr (darr));
   assert (darr->n >= 1);
   if (i + 1 >= darr->n)
      mv = darr->n - i - 1;
   else
      mv = darr->n - (i + 1) - 1;
#ifndef NDEBUG
   fprintf (stderr, "remove front()\n");
   fprintf (stderr, "i    : %d\n", (int) i);
   /*fprintf (stderr, "n    : %d\n", (int) n);*/
   fprintf (stderr, "i + 1: %d\n", (int) (i + 1));
   fprintf (stderr, "curN : %d\n", (int) (darr->n));
   fprintf (stderr, "maxN : %d\n", (int) (darr->array.n));
   fprintf (stderr, "mv   : %d\n", (int) mv);
#endif
   get_array (&(darr->array), i + 0, e);
   mvs_array (&(darr->array), i + 1, i + 0, mv);
   darr->n--;
#ifdef TEST
   assert (! isempty_darr (darr));
   get_array (&(darr->array), i + 0, e);
   if (i != darr->n)
      mvs_array (&(darr->array), i + 1, i + 0, darr->n - i - 1);
   darr->n--;
   assert (darr->n != SIZE_MAX);
#endif
}

__attribute__ ((leaf, nonnull (1, 3), nothrow))
void removes_front_darr (darr_t *restrict darr, size_t i,
   void *restrict e, size_t n) {
   size_t mv;
/*#ifndef NDEBUG
   fprintf (stderr, "remaining:  %d\n", (int) remaining_space_darr (darr));
   fprintf (stderr, "amt_to_rem: %d\n", (int) n);
#endif*/
   assert (darr->n >= n || n == 0);
   assert (! isempty_darr (darr));
   assert (darr->n >= n);
   if (i + n >= darr->n)
      mv = darr->n - i - 1;
   else
      mv = darr->n - (i + n) - 1;
#ifndef NDEBUG
   fprintf (stderr, "removes front()\n");
   fprintf (stderr, "i    : %d\n", (int) i);
   fprintf (stderr, "n    : %d\n", (int) n);
   fprintf (stderr, "i + n: %d\n", (int) (i + n));
   fprintf (stderr, "curN : %d\n", (int) (darr->n));
   fprintf (stderr, "maxN : %d\n", (int) (darr->array.n));
   fprintf (stderr, "mv   : %d\n", (int) mv);
#endif
   gets_array (&(darr->array), i + 0, e, n);
   mvs_array (&(darr->array), i + n, i + 0, mv);
   darr->n -=n;
#ifdef TEST
   sets_array (&(darr->array), i + 0, e, n);
   /* TODO i + n != darr->n*/
   if (i + n < darr->n)
      mvs_array (&(darr->array), i + n, i + 0, darr->n - i - n);
   darr->n -= n;
   TODO (check underflow)
#endif
}

__attribute__ ((leaf, nonnull (1, 2), nothrow))
void make_gaps_darr (darr_t *restrict darr,
   size_t const is[], size_t n) {
   size_t i, prev;
   assert (remaining_space_darr (darr) <= n);
   for (i = n, prev = darr->n; i != 0; i--, prev = is[i])
      mvs_array (&(darr->array),
         is[i - 1], is[i - 1] + i, prev - is[i - 1]);

   darr->n += n;
   assert (darr->n >= n);
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
   assert (remaining_space_darr (darr) >= n);
   for (i = 0; i != n - 1; i++)
      mvs_array (&(darr->array), is[i], is[i] + i, is[i + 1] - is[i]);
   mvs_array (&(darr->array), is[i], is[i] + i, darr->n - is[i]);

   darr->n -= n;
   TODO (check underflow)
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
   if (darr->n == darr->array.n) {
      assert (remaining_space_darr (darr) == 0);
      return true;
   }
   assert (remaining_space_darr (darr) != 0 || darr->array.n == 0);
   return false;
}

__attribute__ ((leaf, nonnull (1), nothrow, pure, warn_unused_result))
bool isempty_darr (darr_t const *restrict darr) {
   if (darr->n == 0) {
      assert (remaining_space_darr (darr) != 0 || darr->array.n == 0);
      return true;
   }
   /*assert (remaining_space_darr (darr) == 0);*/
   return false;
}

__attribute__ ((leaf, nonnull (1), nothrow, pure, warn_unused_result))
size_t remaining_space_darr (darr_t const *restrict darr) {
   return darr->array.n - darr->n;
}

__attribute__ ((leaf, nonnull (1), nothrow, pure, warn_unused_result))
size_t used_space_darr (darr_t const *restrict darr) {
   return darr->n;
}

__attribute__ ((leaf, nonnull (1), nothrow))
void free_darr (darr_t const *restrict darr) {
   free_array (&(darr->array));
}

__attribute__ ((leaf, nonnull (1, 2), nothrow, pure, warn_unused_result))
size_t indexOf_darr (darr_t const *restrict darr,
	void const *restrict e) {
   size_t ret;
   array_t tmp;
   init_array2 (&tmp, &(darr->array), (size_t) 0, darr->n);
   ret = indexOf_array (&tmp, e);
   assert (ret < darr->n);
   return ret;
}

__attribute__ ((leaf, nonnull (1, 2), nothrow, pure, warn_unused_result))
bool contains_darr (darr_t const *restrict darr,
	void const *restrict e) {
   bool ret;
   array_t tmp;
   init_array2 (&tmp, &(darr->array), (size_t) 0, darr->n);
   ret = contains_array (&tmp, e);
   return ret;
}

__attribute__ ((nonnull (1, 2), nothrow, pure, warn_unused_result))
ssize_t indexOf_darr_chk (darr_t const *restrict darr,
   void const *restrict e) {
   ssize_t ret;
   array_t tmp;
   init_array2 (&tmp, &(darr->array), (size_t) 0, darr->n);
   ret = indexOf_array_chk (&tmp, e);
   assert (ret == (ssize_t) -1 || ret < (ssize_t) darr->n);
   return ret;
}

__attribute__ ((leaf, nonnull (1), nothrow, pure, returns_nonnull, warn_unused_result))
void *index_darr (darr_t const *restrict darr, size_t i) {
   array_t tmp;
   init_array2 (&tmp, &(darr->array), (size_t) 0, darr->n);
   return index_array (&tmp, i);
}

__attribute__ ((leaf, nonnull (1, 2), nothrow))
void frees_darr (darr_t const *restrict darr, free_t f) {
   array_t tmp;
   init_array2 (&tmp, &(darr->array), (size_t) 0, darr->n);
   frees_array (&tmp, f);
}
