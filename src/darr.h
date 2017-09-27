#ifndef _DARR_H_
#define _DARR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>

#include <glitter.h>

#define DARRSZN(D, N) ((D)->esz * (N))
#define DARRSZ(D) (DARRSZN ((D), (D)->maxsz))

typedef __attribute__ ((const, warn_unused_result))
size_t (*darr_resize_cb_t) (size_t esz, size_t n, size_t inc) ;

typedef struct {
   size_t esz;
   size_t maxn;
   size_t n;
   void *restrict data;
   darr_resize_cb_t resizecb;
} darr_t;

int init_darr (darr_t *restrict darr, size_t esz,
   darr_resize_cb_t resizecb)
__attribute__ ((nonnull (1, 3), nothrow, warn_unused_result)) ;

int init_darr2 (darr_t *restrict darr, size_t maxn, size_t esz,
   darr_resize_cb_t resizecb)
__attribute__ ((leaf, nonnull (1, 4), nothrow, warn_unused_result)) ;

int ensure_cap_darr (darr_t *restrict darr, size_t n)
__attribute__ ((leaf, nonnull (1), nothrow, warn_unused_result)) ;

int trim_cap_darr (darr_t *restrict darr, size_t n)
__attribute__ ((leaf, nonnull (1), nothrow, warn_unused_result)) ;

int insert_rear_darr (darr_t *restrict darr, void const *restrict e)
__attribute__ ((nonnull (1, 2), nothrow)) ;

int inserts_rear_darr (darr_t *restrict darr,
   void const *restrict e, size_t n)
__attribute__ ((nonnull (1, 2), nothrow)) ;

void remove_rear_darr (darr_t *restrict darr, void *restrict e)
__attribute__ ((leaf, nonnull (1, 2), nothrow)) ;

void removes_rear_darr (darr_t *restrict darr, void *restrict e, size_t n)
__attribute__ ((leaf, nonnull (1, 2), nothrow)) ;

void free_darr (darr_t *restrict darr)
__attribute__ ((leaf, nonnull (1), nothrow)) ;

#ifdef __cplusplus
}
#endif

#endif /* _DARR_H_ */