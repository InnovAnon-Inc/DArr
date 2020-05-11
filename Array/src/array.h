#ifndef _ARRAY_H_
#define _ARRAY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <sys/types.h>

#include <dsint.h>
#include <glitter.h>

typedef struct {
   void *restrict data;
   size_t esz;
   size_t n;
} array_t;

size_t datasz (size_t esz, size_t n)
__attribute__ ((const, leaf, nothrow, warn_unused_result)) ;

size_t arraysz (size_t esz, size_t n)
__attribute__ ((const, nothrow, warn_unused_result)) ;

size_t arraysz2 (array_t const *restrict array)
__attribute__ ((nonnull (1), nothrow, pure, warn_unused_result)) ;

array_t *ez_alloc_array (size_t esz, size_t n)
__attribute__ ((/*alloc_align (1),*/ /*alloc_size (1, 2),*/ /*malloc,*/
	nothrow, warn_unused_result)) ;

void ez_free_array (array_t *restrict array)
__attribute__ ((leaf, nonnull (1), nothrow)) ;

array_t *ez_alloc_array2 (size_t esz, size_t n)
__attribute__ ((/*alloc_align (1),*/ /*alloc_size (1, 2),*/ /*malloc,*/
	nothrow, warn_unused_result)) ;

void ez_free_array2 (array_t *restrict array)
__attribute__ ((leaf, nonnull (1), nothrow)) ;

void *index_array (array_t const *restrict array, size_t i)
__attribute__ ((leaf, nonnull (1), nothrow, pure, returns_nonnull, warn_unused_result)) ;

void init_array (array_t *restrict array,
	void *restrict data, size_t esz, size_t n)
__attribute__ ((leaf, nonnull (1, 2), nothrow)) ;

void init_array2 (array_t *restrict array,
	array_t const *restrict src, size_t min, size_t max)
__attribute__ ((nonnull (1, 2), nothrow)) ;

int alloc_array (array_t *restrict array,
	size_t esz, size_t n)
__attribute__ ((nonnull (1), nothrow, warn_unused_result)) ;

int realloc_array (array_t *restrict array, size_t n)
__attribute__ ((nonnull (1), nothrow, warn_unused_result)) ;

void get_array (array_t const *restrict array, size_t i,
   void *restrict e)
__attribute__ ((nonnull (1, 3), nothrow)) ;

void gets_array (array_t const *restrict array, size_t i,
   void *restrict e, size_t n)
__attribute__ ((nonnull (1, 3), nothrow)) ;

void set_array (array_t const *restrict array, size_t i,
	void const *restrict e)
__attribute__ ((nonnull (1, 3), nothrow)) ;

void sets_array (array_t const *restrict array, size_t i,
	void const *restrict e, size_t n)
__attribute__ ((nonnull (1, 3), nothrow)) ;

void free_array (array_t const *restrict array)
__attribute__ ((leaf, nonnull (1), nothrow)) ;

void cp_array (array_t const *restrict array, size_t i, size_t j)
__attribute__ ((nonnull (1), nothrow)) ;

/* src and dest should not overlap */
void cps_array (array_t const *restrict array,
	size_t i, size_t j, size_t n)
__attribute__ ((nonnull (1), nothrow)) ;

void mvs_array (array_t const *restrict array,
	size_t i, size_t j, size_t n)
__attribute__ ((nonnull (1), nothrow)) ;

void swap_array (array_t const *restrict array,
	size_t i, size_t j, void *restrict tmp)
__attribute__ ((nonnull (1, 4), nothrow)) ;

void swaps_array (array_t const *restrict array,
	size_t i, size_t j, size_t n, void *restrict tmp)
__attribute__ ((nonnull (1, 5), nothrow)) ;

/* src and dest should not overlap ? */
void swap_array2 (array_t const *restrict array,
	size_t i, size_t j)
__attribute__ ((nonnull (1), nothrow)) ;

/* src and dest should not overlap ? */
void swaps_array2 (array_t const *restrict array,
	size_t i, size_t j, size_t n)
__attribute__ ((nonnull (1), nothrow)) ;

size_t indexOf_array (array_t const *restrict array,
	void const *restrict e)
__attribute__ ((leaf, nonnull (1, 2), nothrow, pure, warn_unused_result)) ;

bool contains_array (array_t const *restrict array,
	void const *restrict e)
__attribute__ ((leaf, nonnull (1, 2), nothrow, pure, warn_unused_result)) ;

ssize_t indexOf_array_chk (array_t const *restrict array,
	void const *restrict e)
__attribute__ ((nonnull (1, 2), nothrow, pure, warn_unused_result)) ;

void frees_array (array_t const *restrict array, free_t cb)
__attribute__ ((leaf, nonnull (1, 2), nothrow)) ;

#ifdef __cplusplus
}
#endif

#endif /* _ARRAY_H_ */