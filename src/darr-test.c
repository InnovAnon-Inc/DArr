#if HAVE_CONFIG_H
#include <config.h>
#endif

#define _POSIX_C_SOURCE 200112L
#define __STDC_VERSION__ 200112L

#define NDEBUG 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/*#include <math.h>*/

#include <darr.h>
#include <darr-resize.h>

__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result))
static int insert_rear_test (darr_t *restrict darr,
   int nums[], size_t nnum) {
   size_t k;
   for (k = 0; k != nnum; k++) {
      nums[k] = rand ();
      error_check (insert_rear_darr (darr, nums + k) != 0) {
         puts ("error -2"); fflush (stdout);
         return -2;
      }
   }
   return 0;
}

__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result))
static int remove_rear_test (darr_t *restrict darr,
   int const nums[], size_t nnum){
   size_t k;
   int num;
   for (k = 0; k != nnum; k++) {
      remove_rear_darr (darr, &num);
      error_check (trim_cap_darr (darr, nnum - k - 1) != 0) {
         puts ("error -3"); fflush (stdout);
         return -3;
      }
      error_check (num != nums[nnum - k - 1]) {
         puts ("error -4"); fflush (stdout);
         return -4;
      }
   }
   return 0;
}

__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result))
static int inserts_rear_test (darr_t *restrict darr,
   int nums[], size_t nnum) {
   size_t k;
   for (k = 0; k != nnum; k++)
      nums[k] = rand ();
   error_check (inserts_rear_darr (darr, nums, nnum) != 0) {
      puts ("error -6"); fflush (stdout);
      return -6;
   }
   return 0;
}

__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result))
static int removes_rear_test (darr_t *restrict darr,
   int const nums[], size_t nnum) {
   int *restrict tmps = malloc (sizeof (int) * nnum);
   error_check (tmps == 0) {
      return -1;
   }
   removes_rear_darr (darr, tmps, nnum);
   error_check (trim_cap_darr (darr, (size_t) 0) != 0) {
      puts ("error -7"); fflush (stdout);
      free (tmps);
      return -7;
   }
   error_check (memcmp (nums, tmps, nnum) != 0) {
      puts ("error -8"); fflush (stdout);
      free (tmps);
      return -8;
   }
   free (tmps);
   return 0;
}

__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result))
static int insert_front_test (darr_t *restrict darr,
   int nums[], size_t nnum) {
   size_t k, j;
   for (k = 0; k != nnum; k++) {
      if (darr->n == 0) j = 0;
      else j = (size_t) rand () % darr->n;
      nums[k] = rand ();
      error_check (insert_front_darr (darr, j,
         nums + k) != 0) {
         puts ("error -2"); fflush (stdout);
         return -2;
      }
   }
   return 0;
}

__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result))
static int remove_front_test (darr_t *restrict darr,
   int const nums[], size_t nnum) {
   size_t num;
   size_t k, j;
   for (k = 0; k != nnum; k++) {
      if (darr->n == 0) j = 0;
      else j = (size_t) rand () % darr->n;
      remove_front_darr (darr, j, &num);
      error_check (trim_cap_darr (darr, nnum - k - 1) != 0) {
         puts ("error -3"); fflush (stdout);
         return -3;
      }
      TODO (compare num against nums)
   }
   return 0;
}

__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result))
static int inserts_front_test (darr_t *restrict darr,
   int nums[], size_t nnum) {
   size_t k;
   for (k = 0; k != nnum; k++)
      nums[k] = rand ();
   error_check (inserts_front_darr (darr, (size_t) 0,
      nums, nnum) != 0) {
      puts ("error -6"); fflush (stdout);
      return -6;
   }
   return 0;
}

__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result))
static int removes_front_test (darr_t *restrict darr,
   int nums[], size_t nnum) {
   int *restrict tmps = malloc (sizeof (int) * nnum);
   error_check (tmps == NULL) {
      return -1;
   }
   removes_front_darr (darr, (size_t) 0, tmps, nnum);
   error_check (trim_cap_darr (darr, (size_t) 0) != 0) {
      puts ("error -7"); fflush (stdout);
      free (tmps);
      return -7;
   }
   error_check (memcmp (nums, tmps, nnum) != 0) {
      puts ("error -8"); fflush (stdout);
      free (tmps);
      return -8;
   }
   free (tmps);
   return 0;
}

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static int init_test (darr_t *restrict darr) {
   darr_resize_cb_t cb;
   void *restrict arg;
   int num;
   num = rand () % 3;
   switch (num) {
   case 0:
      darr->resizecb = darr_resize_exact;
      darr->cbargs   = NULL;
      break;
   case 1:
      darr->resizecb = darr_resize_linear;
      darr->cbargs = malloc (sizeof (size_t));
      error_check (darr->cbargs == NULL) return -1;
      /*sfactor = (size_t) (rand () + 1);*/
      *(size_t *restrict) (darr->cbargs) = 2;
      break;
   case 2:
      darr->resizecb = darr_resize_geometric;
      /*den = rand ();
      do num = rand ();
      while (num == 0);
      dfactor = (double) den / (double) num;*/
      darr->cbargs = malloc (sizeof (double));
      error_check (darr->cbargs == NULL) return -1;
      *(double *restrict) (darr->cbargs) = (double) 2;
      break;
   default: __builtin_unreachable ();
   }
   return init_darr (darr, sizeof (int), darr->resizecb, darr->cbargs);
}

__attribute__ ((nonnull (1), nothrow))
static void free_test (darr_t *restrict darr) {
   free_darr (darr);
   if (darr->cbargs != NULL) free (darr->cbargs);
}

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static int reset_test (darr_t *restrict darr) {
   free_test (darr);
   return init_test (darr);
}

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static int test0 (darr_t *restrict darr) {
   int num = rand ();
   error_check (insert_rear_darr (darr, &num) != 0) {
      puts ("error -10"); fflush (stdout);
      return -10;
   }
   return 0;
}

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static int test1 (darr_t *restrict darr, size_t nnum) {
   size_t k;
   size_t snum = (size_t) rand () % nnum;
   int *restrict nums = malloc (sizeof (int) * snum);
   for (k = 0; k != snum; k++)
      nums[k] = rand ();
   error_check (inserts_rear_darr (darr, nums, snum) != 0) {
      puts ("error -11"); fflush (stdout);
      free (nums);
      return -11;
   }
   free (nums);
   return 0;
}

__attribute__ ((nonnull (1), nothrow))
static void test2 (darr_t *restrict darr) {
   size_t num;
   if (darr->n == 0) return;
   remove_rear_darr (darr, &num);
}

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static int test3 (darr_t *restrict darr) {
   size_t snum;
   int *restrict nums;
   if (darr->n == 0) return 0;
   snum = (size_t) rand () % darr->n;
   nums = malloc (sizeof (int) * snum);
   error_check (nums == NULL) return -1;
   removes_rear_darr (darr, nums, snum);
   free (nums);
   return 0;
}

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static int test4 (darr_t *restrict darr) {
   int num;
   size_t k;
   if (darr->n == 0) k = 0;
   else k = (size_t) rand () % darr->n;
   num = rand ();
   error_check (insert_front_darr (darr, k, &num) != 0) {
      puts ("error -12"); fflush (stdout);
      return -12;
   }
   return 0;
}

__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result))
static int test5 (darr_t *restrict darr, int nums[], size_t nnum) {
   size_t snum = (size_t) rand () % nnum;
   size_t k;
   for (k = 0; k != snum; k++)
      nums[k] = rand ();
   if (darr->n == 0) k = 0;
   else k = (size_t) rand () % darr->n;
   error_check (inserts_front_darr (darr, k, nums, snum) != 0) {
      puts ("error -13"); fflush (stdout);
      return -13;
   }
   return 0;
}

__attribute__ ((nonnull (1), nothrow))
static void test6 (darr_t *restrict darr) {
   size_t k;
   int num;
   if (darr->n == 0) return;
   k = (size_t) rand () % darr->n;
   remove_front_darr (darr, k, &num);
}

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static int test7 (darr_t *restrict darr) {
   size_t k;
   size_t snum;
   int *restrict nums;
   if (darr->n == 0) return 0;
   k = (size_t) rand () % darr->n;
   snum = k + (size_t) rand () % (darr->n - k);
   nums = malloc (sizeof (int) * snum);
   error_check (nums == NULL) return -1;
   removes_front_darr (darr, k, nums, snum);
   free (nums);
   return 0;
}

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static int test8 (darr_t *restrict darr) {
   size_t snum;
   if (darr->n == 0) return 0;
   snum = (size_t) rand () % darr->n;
   error_check (trim_cap_darr (darr, snum) != 0) {
      puts ("error -15"); fflush (stdout);
      return -15;
   }
   return 0;
}

__attribute__ ((nothrow, warn_unused_result))
int main (void) {
   darr_t darr;
   double dfactor = 2;
   size_t sfactor = 2;
   time_t t;
   int num;
   int den;
   int nums[100];
   int tmps[ARRSZ (nums)];
   size_t k;
   size_t snum;
   /*darr_resize_cb_t cbs[3];*/
   size_t ntest = 100;
   size_t testi;

   t = time (NULL);
   srand ((unsigned int) t);

   error_check (init_test (&darr) != 0) return -1;

   error_check (insert_rear_test  (&darr, nums, ARRSZ (nums)) != 0) return -2;
   error_check (remove_rear_test  (&darr, nums, ARRSZ (nums)) != 0) return -2;

   error_check (reset_test (&darr) != 0) return -3;

   error_check (inserts_rear_test (&darr, nums, ARRSZ (nums)) != 0) return -2;
   error_check (removes_rear_test (&darr, nums, ARRSZ (nums)) != 0) return -2;

   error_check (reset_test (&darr) != 0) return -3;

   error_check (insert_front_test  (&darr, nums, ARRSZ (nums)) != 0) return -2;
   error_check (remove_front_test  (&darr, nums, ARRSZ (nums)) != 0) return -2;

   error_check (reset_test (&darr) != 0) return -3;

   error_check (inserts_front_test (&darr, nums, ARRSZ (nums)) != 0) return -2;
   error_check (removes_front_test (&darr, nums, ARRSZ (nums)) != 0) return -2;

   error_check (reset_test (&darr) != 0) return -3;

   error_check (test0 (&darr) != 0) return -4;
   error_check (test1 (&darr, ARRSZ (nums)) != 0) return -5;
   test2 (&darr);
   error_check (test3 (&darr) != 0) return -6;
   error_check (test4 (&darr) != 0) return -7;
   error_check (test5 (&darr, nums, ARRSZ (nums)) != 0) return -5;
   test6 (&darr);
   error_check (test7 (&darr) != 0) return -7;
   error_check (test8 (&darr) != 0) return -8;

   free_test (&darr);



   for (testi = 0; testi != ntest; testi++)
   switch (rand () % 10) {
   case 0:
      num = rand ();
      error_check (insert_rear_darr (&darr, &num) != 0) {
         puts ("error -10"); fflush (stdout);
         free_darr (&darr);
         return -10;
      }
      break;
   case 1:
      snum = (size_t) rand () % ARRSZ (nums);
      for (k = 0; k != snum; k++)
         nums[k] = rand ();
      error_check (inserts_rear_darr (&darr, nums, snum) != 0) {
         puts ("error -11"); fflush (stdout);
         free_darr (&darr);
         return -11;
      }
      break;
   case 2:
      if (darr.n == 0) break;
      remove_rear_darr (&darr, &num);
      break;
   case 3:
      if (darr.n == 0) break;
      snum = (size_t) rand () % darr.n;
      removes_rear_darr (&darr, nums, snum);
      break;
   case 4:
      if (darr.n == 0) k = 0;
      else k = (size_t) rand () % darr.n;
      num = rand ();
      error_check (insert_front_darr (&darr, k, &num) != 0) {
         puts ("error -12"); fflush (stdout);
         free_darr (&darr);
         return -12;
      }
      break;
   case 5:
      snum = (size_t) rand () % ARRSZ (nums);
      for (k = 0; k != snum; k++)
         nums[k] = rand ();
      if (darr.n == 0) k = 0;
      else k = (size_t) rand () % darr.n;
      error_check (inserts_front_darr (&darr, k, nums, snum) != 0) {
         puts ("error -13"); fflush (stdout);
         free_darr (&darr);
         return -13;
      }
      break;
   case 6:
      if (darr.n == 0) break;
      k = (size_t) rand () % darr.n;
      remove_front_darr (&darr, k, &num);
      break;
   case 7:
      if (darr.n == 0) break;
      k = (size_t) rand () % darr.n;
      snum = k + (size_t) rand () % (darr.n - k);
      removes_front_darr (&darr, k, nums, snum);
      break;
   case 8:
      if (darr.n == 0) break;
      snum = (size_t) rand () % darr.n;
      error_check (trim_cap_darr (&darr, snum) != 0) {
         puts ("error -15"); fflush (stdout);
         free_darr (&darr);
         return -15;
      }
      break;
   case 9:
      num = rand () % 3;
      switch (num) {
      case 0:
         darr.resizecb = darr_resize_exact;
         darr.cbargs   = NULL;
         break;
      case 1:
         darr.resizecb = darr_resize_linear;
         /*sfactor = (size_t) (rand () + 1);*/
         sfactor = 2;
         darr.cbargs = &sfactor;
         break;
      case 2:
         darr.resizecb = darr_resize_geometric;
         /*den = rand ();
         do num = rand ();
         while (num == 0);
         dfactor = (double) den / (double) num;*/
         dfactor = (double) 2;
         darr.cbargs = &dfactor;
         break;
      default: __builtin_unreachable ();
      }
      break;
   default: __builtin_unreachable ();
   }

   free_darr (&darr);

   puts ("success"); fflush (stdout);

   return EXIT_SUCCESS;
}
