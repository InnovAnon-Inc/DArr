#ifndef _DARR_RESIZE_H_
#define _DARR_RESIZE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>

#include <glitter.h>

#include <darr.h>

size_t darr_resize_exact (size_t inc,
   void const *restrict unused)
__attribute__ ((const, leaf, warn_unused_result)) ;

size_t darr_resize_geometric (size_t inc,
   void const *restrict factor)
__attribute__ ((leaf, nonnull (2), pure, warn_unused_result)) ;

size_t darr_resize_linear (size_t inc,
   void const *restrict factor)
__attribute__ ((leaf, nonnull (2), pure, warn_unused_result)) ;

#ifdef __cplusplus
}
#endif

#endif /* _DARR_RESIZE_H_ */