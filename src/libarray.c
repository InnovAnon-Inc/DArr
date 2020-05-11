#if HAVE_CONFIG_H
#include <config.h>
#endif

#define _POSIX_C_SOURCE 200112L
#define __STDC_VERSION__ 200112L

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include <mmalloc.h>
#include <swap.h>

#include <array.h>

__attribute__ ((const, leaf, nothrow, warn_unused_result))
size_t datasz (size_t esz, size_t n) { return esz * n; }

__attribute__ ((const, nothrow, warn_unused_result))
size_t arraysz (size_t esz, size_t n) {
	return sizeof (array_t) + datasz (esz, n);
}

__attribute__ ((nonnull (1), nothrow, pure, warn_unused_result))
size_t arraysz2 (array_t const *restrict array) {
	return arraysz (array->esz, array->n);
}

__attribute__ ((/*alloc_align (1),*/ /*alloc_size (1, 2),*/ /*malloc,*/
	nothrow, warn_unused_result))
array_t *ez_alloc_array (size_t esz, size_t n) {
	/*
	void *restrict combined[2];
	size_t eszs[2];
	array_t *restrict array;
	void *restrict data;

	eszs[0] = sizeof (array_t);
	eszs[1] = datasz  (esz, n);
	error_check (mmalloc (combined, eszs,
		eszs[0] + eszs[1], ARRSZ (eszs)) != 0)
		return NULL;
	array = (array_t *restrict) combined[0];
	data  = (void *restrict)    combined[1];

	init_array (array, data, esz, n);
	return array;
	*/
	void *restrict *restrict combined[2];
	size_t eszs[2];
	array_t *restrict caq;
	void *restrict data;

	eszs[0] = sizeof (array_t);
	eszs[1] = datasz  (esz, n);
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wstrict-aliasing"
   combined[0] = (void *restrict *restrict) &caq;
   combined[1] = (void *restrict *restrict) &data;
	#pragma GCC diagnostic pop
	error_check (mmalloc2 (combined, eszs,
		eszs[0] + eszs[1], ARRSZ (eszs)) != 0)
		return NULL;

   init_array (caq, data, esz, n);
	return caq;
}

__attribute__ ((leaf, nonnull (1), nothrow))
void ez_free_array (array_t *restrict array) {
	/*free_array (array);*/
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wstrict-aliasing"
	mfree ((void *restrict) array);
	#pragma GCC diagnostic pop
}

__attribute__ ((/*alloc_align (1),*/ /*alloc_size (1, 2),*/ /*malloc,*/
	nothrow, warn_unused_result))
array_t *ez_alloc_array2 (size_t esz, size_t n) {
	array_t *restrict array = malloc (sizeof (array_t));
	error_check (array == NULL) return NULL;
	error_check (alloc_array (array, esz, n) != 0) {
		free (array);
		return NULL;
	}
	return array;
}

__attribute__ ((leaf, nonnull (1), nothrow))
void ez_free_array2 (array_t *restrict array) {
	free_array (array);
	free (array);
}

NOTE (this would be way faster if esz were static
	i.e. this is the cost of this style of generics)
__attribute__ ((leaf, nonnull (1), nothrow, pure, returns_nonnull, warn_unused_result))
void *index_array (array_t const *restrict array, size_t i) {
	char *restrict data;
	char *restrict ret;
	assert (i < array->n || (i == 0 && array->n == 0));
	data = (char *restrict) array->data;
	ret  = data + i * array->esz;
	return (void *restrict) ret;
}

__attribute__ ((leaf, nonnull (1, 2), nothrow))
void init_array (array_t *restrict array,
	void *restrict data, size_t esz, size_t n) {
	array->data = data;
	array->esz  = esz;
	array->n    = n;
}

__attribute__ ((nonnull (1, 2), nothrow))
void init_array2 (array_t *restrict array,
	array_t const *restrict src, size_t min, size_t max) {
	init_array (array, index_array (src, min),
		src->esz, /*range_size_t (min, max)*/ /*max - min*/ range (min, max));
}

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
int alloc_array (array_t *restrict array,
	size_t esz, size_t n) {
	void *restrict data;
	data = malloc (datasz (esz, n));
	error_check (data == NULL) return -1;
	init_array (array, data, esz, n);
	return 0;
}

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
int realloc_array (array_t *restrict array, size_t n) {
	void *restrict new_data;
	assert (n != 0);
	new_data = realloc (array->data, datasz (array->esz, n));
	error_check (new_data == NULL) return -1;
	array->data = new_data;
	array->n = n;
	return 0;
}

__attribute__ ((nonnull (1, 3), nothrow))
void get_array (array_t const *restrict array, size_t i,
	void *restrict e) {
	void const *restrict src = index_array (array, i);
	(void) memcpy (e, src, array->esz);
	assert (memcmp (src, e, array->esz) == 0);
}

__attribute__ ((nonnull (1, 3), nothrow))
void gets_array (array_t const *restrict array, size_t i,
	void *restrict e, size_t n) {
	void const *restrict src;
	/*assert (i + n < array->n);*/
	assert (i + n <= array->n);
	if (n == 0) return;
	src = index_array (array, i);
	(void) memcpy (e, src, datasz (array->esz, n));
	TODO (verify that data is correctly copied)
}

__attribute__ ((nonnull (1, 3), nothrow))
void set_array (array_t const *restrict array, size_t i,
	void const *restrict e) {
	void *restrict dest = index_array (array, i);
	(void) memcpy (dest, e, array->esz);
	assert (memcmp (dest, e, array->esz) == 0);
}

__attribute__ ((nonnull (1, 3), nothrow))
void sets_array (array_t const *restrict array, size_t i,
	void const *restrict e, size_t n) {
	void *restrict dest;
	/*assert (i + n < array->n);*/
	assert (i + n <= array->n);
	if (n == 0) return;
	dest = index_array (array, i);
	(void) memcpy (dest, e, datasz (array->esz, n));
	TODO (verify that data is correctly copied)
}

__attribute__ ((nonnull (1), nothrow))
void cp_array (array_t const *restrict array, size_t i, size_t j) {
	void const *restrict src  = index_array (array, i);
	void *restrict dest = index_array (array, j);
	memcpy (dest, src, array->esz);
	assert (memcmp (src, dest, array->esz) == 0);
}

/* src and dest should not overlap */
__attribute__ ((nonnull (1), nothrow))
void cps_array (array_t const *restrict array,
	size_t i, size_t j, size_t n) {
	void const *restrict src;
	void *restrict dest;
	/*assert (i + n < array->n);
	assert (j + n < array->n);
	assert (i + n < j || j + n < i);*/
	assert (i + n <= array->n);
	assert (j + n <= array->n);
	assert (i + n <= j || j + n <= i);
	src  = index_array (array, i);
	dest = index_array (array, j);
	memcpy (dest, src, datasz (array->esz, n));
	TODO (verify that data is correctly copied)
}

__attribute__ ((nonnull (1), nothrow))
void mvs_array (array_t const *restrict array,
	size_t i, size_t j, size_t n) {
	void const *src;
	void *dest;
	/*assert (i + n < array->n);
	assert (j + n < array->n);*/
	assert (i + n <= array->n);
	assert (j + n <= array->n);
	if (n == 0) return;
	src  = index_array (array, i);
	dest = index_array (array, j);
	memmove (dest, src, datasz (array->esz, n));
	TODO (verify that data is correctly copied)
}

__attribute__ ((nonnull (1, 4), nothrow))
void swap_array (array_t const *restrict array,
	size_t i, size_t j, void *restrict tmp) {
	void *restrict src  = index_array (array, i);
	void *restrict dest = index_array (array, j);
	swap (src, dest, tmp, array->esz);
	TODO (verify that data is correctly copied)
}

__attribute__ ((nonnull (1, 5), nothrow))
void swaps_array (array_t const *restrict array,
	size_t i, size_t j, size_t n, void *restrict tmp) {
	void *restrict src;
	void *restrict dest;
	/*assert (i + n < array->n);
	assert (j + n < array->n);*/
	assert (i + n <= array->n);
	assert (j + n <= array->n);
	src  = index_array (array, i);
	dest = index_array (array, j);
	swaps (src, dest, tmp, array->esz, n);
	TODO (verify that data is correctly copied)
}

/* src and dest should not overlap ? */
__attribute__ ((nonnull (1), nothrow))
void swap_array2 (array_t const *restrict array,
	size_t i, size_t j) {
	void *restrict src  = index_array (array, i);
	void *restrict dest = index_array (array, j);
	swap2 (src, dest, array->esz);
	TODO (verify that data is correctly copied)
}

/* src and dest should not overlap ? */
__attribute__ ((nonnull (1), nothrow))
void swaps_array2 (array_t const *restrict array,
	size_t i, size_t j, size_t n) {
	void *restrict src  = index_array (array, i);
	void *restrict dest = index_array (array, j);
	swaps2 (src, dest, array->esz, n);
	TODO (verify that data is correctly copied)
}

__attribute__ ((leaf, nonnull (1), nothrow))
void free_array (array_t const *restrict array) {
	free (array->data);
}

__attribute__ ((leaf, nonnull (1, 2), nothrow, pure, warn_unused_result))
size_t indexOf_array (array_t const *restrict array,
	void const *restrict e) {
	size_t i;
	void *restrict tmp;
	TODO (is this pragma ok ?)
	#pragma GCC ivdep
	for (i = 0; i != array->n; i++) {
		tmp = index_array (array, i);
		if (memcmp (tmp, e, array->esz) == 0)
			return i;
	}
	assert (false);
	__builtin_unreachable ();
}

__attribute__ ((leaf, nonnull (1, 2), nothrow, pure, warn_unused_result))
bool contains_array (array_t const *restrict array,
	void const *restrict e) {
	size_t i;
	void *restrict tmp;
	TODO (is this pragma ok ?)
	#pragma GCC ivdep
	for (i = 0; i != array->n; i++) {
		tmp = index_array (array, i);
		if (memcmp (tmp, e, array->esz) == 0)
			return true;
	}
	return false;
}

__attribute__ ((nonnull (1, 2), nothrow, pure, warn_unused_result))
ssize_t indexOf_array_chk (array_t const *restrict array,
	void const *restrict e) {
	size_t i;
	void *restrict tmp;
	TODO (is this pragma ok ?)
	#pragma GCC ivdep
	for (i = 0; i != array->n; i++) {
		tmp = index_array (array, i);
		if (memcmp (tmp, e, array->esz) == 0)
			return (ssize_t) i;
	}
	return (ssize_t) -1;
}

__attribute__ ((leaf, nonnull (1, 2), nothrow))
void frees_array (array_t const *restrict array, free_t f) {
	size_t i;
	void *restrict tmp;
	#pragma GCC ivdep
	for (i = 0; i != array->n; i++) {
		tmp = index_array (array, i);
		f (tmp);
	}
}

/*
typedef __attribute__ ((nonnull (1, 3), warn_unused_result))
size_t (*print_elem_t) (char buf[], size_t bufsz,
	void const *restrict e) ;

typedef __attribute__ ((nonnull (1), warn_unused_result))
size_t (*print_sep_t) (char buf[], size_t bufsz) ;

TODO (maybe change to toString_data())

__attribute__ ((leaf, nonnull (1, 3, 4, 5), nothrow, warn_unused_result))
size_t toString_array (char buf[], size_t busfz,
	print_elem_t print_elem, print_sep_t print_sep,
	array_t const *restrict array) {
	size_t i, prt;
	void *restrict e;
	if (array->n == 0) return ?;

	e   = index_array (array, 0);
	prt = print_elem (buf + prt, bufsz - prt, e);

	for (i = 1; i != array->n && prt != bufsz; i++) {
		prt += print_sep  (buf + prt, bufsz - prt);
		if (prt == bufsz) break;
		e = index_array (array, i);
		prt += print_elem (buf + prt, bufsz - prt, e);
	}
	return prt;
}
*/

/* one version takes a statically allocated array */
/* another allocates it for you, based on the number of separators */
/*
__attribute__ ((leaf, nonnull (1, 3, 4), nothrow, warn_unused_result))
size_t fromString_array (array_t *restrict array,
	char const buf[], size_t busfz,
	parse_elem_t parse_elem) {
	TODO ()
	size_t i, prt;
	void *restrict e;
	if (array->n == 0) return;

	e   = index_array (array, 0);
	prt = print_elem (buf + prt, bufsz - prt, e);

	for (i = 1; i != array->n && prt != bufsz; i++) {
		prt += print_sep  (buf + prt, bufsz - prt);
		if (prt == bufsz) break;
		e = index_array (array, i);
		prt += print_elem (buf + prt, bufsz - prt, e);
	}
	return prt;
}
*/
/*
void toStdint_array (void *restrict buf, array_t const *restrict array) {
	*((uint64_t *) buf++) = array->n;
	*((uint64_t *) buf++) = array->esz;
	(void) memcpy (buf, array->data, datasz (array->esz, array->n));
}

void fromStdint_array (array_t *restrict array, void const *restrict buf) {
	array->n   = *((uint64_t *) buf++);
	array->esz = *((uint64_t *) buf++);
	(void) memcpy (array->data, buf, datasz (array->esz, array->n));
}
*/
