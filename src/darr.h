#ifndef _DARR_H_
#define _DARR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <sys/types.h>

#include <glitter.h>
#include <array.h>

#define DARRSZN(D, N) ((D)->array.esz * (N))
#define DARRSZ(D) (DARRSZN ((D), (D)->array.n))

typedef __attribute__ ((warn_unused_result))
size_t (*darr_resize_cb_t) (size_t inc,
   void const *restrict args) ;

typedef struct {
   array_t array;
   size_t n;
   darr_resize_cb_t resizecb;
   void /*const*/ *restrict cbargs;
} darr_t;

size_t darrsz (size_t esz, size_t maxn)
__attribute__ ((const, leaf, nothrow, warn_unused_result)) ;

size_t darrsz2 (darr_t const *restrict darr)
__attribute__ ((nonnull (1), nothrow, pure, warn_unused_result)) ;

darr_t *ez_alloc_darr11 (size_t esz,
   darr_resize_cb_t resizecb, void const *restrict cbargs)
__attribute__ ((/*alloc_align (1),*/ /*alloc_size (1, 2),*/ /*malloc,*/
	nonnull (2, 3), nothrow, warn_unused_result)) ;

darr_t *ez_alloc_darr12 (size_t esz, size_t maxn,
   darr_resize_cb_t resizecb, void const *restrict cbargs)
__attribute__ ((/*alloc_align (1),*/ /*alloc_size (1, 2),*/ /*malloc,*/
	nonnull (3, 4), nothrow, warn_unused_result)) ;

void ez_free_darr1 (darr_t *restrict darr)
__attribute__ ((leaf, nonnull (1), nothrow)) ;

darr_t *ez_alloc_darr21 (size_t esz,
   darr_resize_cb_t resizecb, void const *restrict cbargs)
__attribute__ ((/*alloc_align (1),*/ /*alloc_size (1, 2),*/ /*malloc,*/
	nonnull (2, 3), nothrow, warn_unused_result)) ;

darr_t *ez_alloc_darr22 (size_t esz, size_t maxn,
   darr_resize_cb_t resizecb, void const *restrict cbargs)
__attribute__ ((/*alloc_align (1),*/ /*alloc_size (1, 2),*/ /*malloc,*/
	nonnull (3, 4), nothrow, warn_unused_result)) ;

void ez_free_darr2 (darr_t *restrict array)
__attribute__ ((leaf, nonnull (1), nothrow)) ;

int alloc_darr (darr_t *restrict darr, size_t esz,
   darr_resize_cb_t resizecb, void const *restrict cbargs)
__attribute__ ((nonnull (1, 3), nothrow, warn_unused_result)) ;

int alloc_darr2 (darr_t *restrict darr,
   size_t esz, size_t maxn,
   darr_resize_cb_t resizecb, void const *restrict cbargs)
__attribute__ ((leaf, nonnull (1, 4), nothrow, warn_unused_result)) ;

void init_darr (darr_t *restrict darr,
   void *restrict data, size_t esz,
   darr_resize_cb_t resizecb, void const *restrict cbargs)
__attribute__ ((nonnull (1, 2, 4), nothrow)) ;

void init_darr2 (darr_t *restrict darr,
   void *restrict data, size_t esz, size_t maxn,
   darr_resize_cb_t resizecb, void const *restrict cbargs)
__attribute__ ((leaf, nonnull (1, 2, 5), nothrow)) ;

int ensure_cap_darr (darr_t *restrict darr, size_t n)
__attribute__ ((leaf, nonnull (1), nothrow, warn_unused_result)) ;

int trim_cap_darr (darr_t *restrict darr, size_t n)
__attribute__ ((leaf, nonnull (1), nothrow, warn_unused_result)) ;

void insert_rear_darr (darr_t *restrict darr, void const *restrict e)
__attribute__ ((leaf, nonnull (1, 2), nothrow)) ;

int ez_insert_rear_darr (darr_t *restrict darr, void const *restrict e)
__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result)) ;

void inserts_rear_darr (darr_t *restrict darr,
   void const *restrict e, size_t n)
__attribute__ ((leaf, nonnull (1, 2), nothrow)) ;

int ez_inserts_rear_darr (darr_t *restrict darr,
   void const *restrict e, size_t n)
__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result)) ;

void insert_front_darr (darr_t *restrict darr, size_t i,
   void const *restrict e)
__attribute__ ((leaf, nonnull (1, 3), nothrow)) ;

int ez_insert_front_darr (darr_t *restrict darr, size_t i,
   void const *restrict e)
__attribute__ ((nonnull (1, 3), nothrow, warn_unused_result)) ;

void inserts_front_darr (darr_t *restrict darr, size_t i,
   void const *restrict e, size_t n)
__attribute__ ((leaf, nonnull (1, 3), nothrow)) ;

int ez_inserts_front_darr (darr_t *restrict darr, size_t i,
   void const *restrict e, size_t n)
__attribute__ ((nonnull (1, 3), nothrow, warn_unused_result)) ;

void remove_rear_darr (darr_t *restrict darr, void *restrict e)
__attribute__ ((leaf, nonnull (1, 2), nothrow)) ;

void removes_rear_darr (darr_t *restrict darr,
   void *restrict e, size_t n)
__attribute__ ((leaf, nonnull (1, 2), nothrow)) ;

void remove_front_darr (darr_t *restrict darr, size_t i,
   void *restrict e)
__attribute__ ((leaf, nonnull (1, 3), nothrow)) ;

void removes_front_darr (darr_t *restrict darr, size_t i,
   void *restrict e, size_t n)
__attribute__ ((leaf, nonnull (1, 3), nothrow)) ;

void make_gaps_darr (darr_t *restrict darr,
   size_t const is[], size_t n)
__attribute__ ((leaf, nonnull (1, 2), nothrow)) ;

void inserts_darr (darr_t *restrict darr, size_t const is[],
   void const *restrict e, size_t n)
__attribute__ ((nonnull (1, 2, 3), nothrow)) ;

int ez_inserts_darr (darr_t *restrict darr, size_t const is[],
   void const *restrict e, size_t n)
__attribute__ ((nonnull (1, 2, 3), nothrow, warn_unused_result)) ;

void unmake_gaps_darr (darr_t *restrict darr,
   size_t const is[], size_t n)
__attribute__ ((leaf, nonnull (1, 2), nothrow)) ;

void removes_darr (darr_t *restrict darr, size_t const is[],
   void *restrict e, size_t n)
__attribute__ ((nonnull (1, 2, 3), nothrow)) ;

bool isfull_darr (darr_t const *restrict darr)
__attribute__ ((leaf, nonnull (1), nothrow, pure, warn_unused_result)) ;

bool isempty_darr (darr_t const *restrict darr)
__attribute__ ((leaf, nonnull (1), nothrow, pure, warn_unused_result)) ;

size_t remaining_space_darr (darr_t const *restrict darr)
__attribute__ ((leaf, nonnull (1), nothrow, pure, warn_unused_result)) ;

size_t used_space_darr (darr_t const *restrict darr)
__attribute__ ((leaf, nonnull (1), nothrow, pure, warn_unused_result)) ;

void free_darr (darr_t const *restrict darr)
__attribute__ ((leaf, nonnull (1), nothrow)) ;

size_t indexOf_darr (darr_t const *restrict darr,
	void const *restrict e)
__attribute__ ((leaf, nonnull (1, 2), nothrow, pure, warn_unused_result)) ;

bool contains_darr (darr_t const *restrict darr,
	void const *restrict e)
__attribute__ ((leaf, nonnull (1, 2), nothrow, pure, warn_unused_result)) ;

ssize_t indexOf_darr_chk (darr_t const *restrict array,
   void const *restrict e)
__attribute__ ((nonnull (1, 2), nothrow, pure, warn_unused_result)) ;

void *index_darr (darr_t const *restrict darr, size_t i)
__attribute__ ((leaf, nonnull (1), nothrow, pure, returns_nonnull, warn_unused_result)) ;

void frees_darr (darr_t const *restrict darr, free_t cb)
__attribute__ ((leaf, nonnull (1, 2), nothrow)) ;

#ifdef __cplusplus
}
#endif

#endif /* _DARR_H_ */