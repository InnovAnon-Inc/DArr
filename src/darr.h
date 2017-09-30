#ifndef _DARR_H_
#define _DARR_H_

#ifdef __cplusplus
extern "C" {
#endif

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
   void *restrict cbargs;
} darr_t;

int alloc_darr (darr_t *restrict darr, size_t esz,
   darr_resize_cb_t resizecb, void *restrict cbargs)
__attribute__ ((nonnull (1, 3), nothrow, warn_unused_result)) ;

int alloc_darr2 (darr_t *restrict darr,
   size_t esz, size_t maxn,
   darr_resize_cb_t resizecb, void *restrict cbargs)
__attribute__ ((leaf, nonnull (1, 4), nothrow, warn_unused_result)) ;

/* don't give us a statically allocated data* */
void init_darr (darr_t *restrict darr,
   void *restrict data, size_t esz,
   darr_resize_cb_t resizecb, void *restrict cbargs)
__attribute__ ((nonnull (1, 2, 4), nothrow)) ;

void init_darr2 (darr_t *restrict darr,
   void *restrict data, size_t esz, size_t maxn,
   darr_resize_cb_t resizecb, void *restrict cbargs)
__attribute__ ((leaf, nonnull (1, 2, 5), nothrow)) ;

int ensure_cap_darr (darr_t *restrict darr, size_t n)
__attribute__ ((leaf, nonnull (1), nothrow, warn_unused_result)) ;

int trim_cap_darr (darr_t *restrict darr, size_t n)
__attribute__ ((leaf, nonnull (1), nothrow, warn_unused_result)) ;

int insert_rear_darr (darr_t *restrict darr, void const *restrict e)
__attribute__ ((nonnull (1, 2), nothrow)) ;

int inserts_rear_darr (darr_t *restrict darr,
   void const *restrict e, size_t n)
__attribute__ ((nonnull (1, 2), nothrow)) ;

int insert_front_darr (darr_t *restrict darr, size_t i,
   void const *restrict e)
__attribute__ ((nonnull (1, 3), nothrow)) ;

int inserts_front_darr (darr_t *restrict darr, size_t,
   void const *restrict e, size_t n)
__attribute__ ((nonnull (1, 3), nothrow)) ;

void remove_rear_darr (darr_t *restrict darr, void *restrict e)
__attribute__ ((leaf, nonnull (1, 2), nothrow)) ;

void removes_rear_darr (darr_t *restrict darr, void *restrict e, size_t n)
__attribute__ ((leaf, nonnull (1, 2), nothrow)) ;

void remove_front_darr (darr_t *restrict darr, size_t i, void *restrict e)
__attribute__ ((leaf, nonnull (1, 3), nothrow)) ;

void removes_front_darr (darr_t *restrict darr, size_t i,
   void *restrict e, size_t n)
__attribute__ ((leaf, nonnull (1, 3), nothrow)) ;

void free_darr (darr_t const *restrict darr)
__attribute__ ((leaf, nonnull (1), nothrow)) ;

#ifdef __cplusplus
}
#endif

#endif /* _DARR_H_ */