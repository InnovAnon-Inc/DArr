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

__attribute__ ((nonnull (1), nothrow))
static void darr_print (darr_t const *restrict darr) {
   size_t i;
   fprintf (stderr, "esz : %d\n", (int) darr->esz);  fflush (stderr);
   fprintf (stderr, "maxn: %d\n", (int) darr->maxn); fflush (stderr);
   fprintf (stderr, "n   : %d\n", (int) darr->n);    fflush (stderr);
   if (darr->resizecb == darr_resize_linear) {
   fprintf (stderr, "f   : %d\n", (int) *(size_t *restrict) darr->cbargs); fflush (stderr);
   } else if (darr->resizecb == darr_resize_geometric) {
   fprintf (stderr, "f   : %g\n", *(double *restrict) darr->cbargs); fflush (stderr);
   }
   if (darr->n <= 30) {
      fprintf (stderr, "["); fflush (stderr);
      if (darr->n > 0) {
         fprintf (stderr, "%d", ((int *restrict) darr->data)[0]); fflush (stderr);
         for (i = 1; i != darr->n; i++)
            fprintf (stderr, ", %d", ((int *restrict) darr->data)[i]); fflush (stderr);
      }
      fprintf (stderr, "]\n"); fflush (stderr);
   }
   fputs ("", stderr); fflush (stderr);
}

__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result))
static int insert_rear_test (darr_t *restrict darr,
   int nums[], size_t nnum) {
   size_t k;
   fprintf (stderr, "insert rear test (nnum=%d)\n", (int) nnum); fflush (stderr);
   for (k = 0; k != nnum; k++) {
      nums[k] = rand ();
      error_check (insert_rear_darr (darr, nums + k) != 0) {
         fputs ("error -2", stderr); fflush (stderr);
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
   fprintf (stderr, "remove rear test (nnum=%d)\n", (int) nnum); fflush (stderr);
   for (k = 0; k != nnum; k++) {
      remove_rear_darr (darr, &num);
      error_check (trim_cap_darr (darr, nnum - k - 1) != 0) {
         fputs ("error -3", stderr); fflush (stderr);
         return -3;
      }
      error_check (num != nums[nnum - k - 1]) {
         fputs ("error -4", stderr); fflush (stderr);
         return -4;
      }
   }
   return 0;
}

__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result))
static int inserts_rear_test (darr_t *restrict darr,
   int nums[], size_t nnum) {
   size_t k;
   fprintf (stderr, "inserts rear test (nnum=%d)\n", (int) nnum); fflush (stderr);
   for (k = 0; k != nnum; k++)
      nums[k] = rand ();
   error_check (inserts_rear_darr (darr, nums, nnum) != 0) {
      fputs ("error -6", stderr); fflush (stderr);
      return -6;
   }
   return 0;
}

__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result))
static int removes_rear_test (darr_t *restrict darr,
   int const nums[], size_t nnum) {
   int *restrict tmps;
   fprintf (stderr, "removes rear test (nnum=%d)\n", (int) nnum); fflush (stderr);
   tmps = malloc (sizeof (int) * nnum);
   error_check (tmps == 0) {
      return -1;
   }
   removes_rear_darr (darr, tmps, nnum);
   error_check (trim_cap_darr (darr, (size_t) 0) != 0) {
      fputs ("error -7", stderr); fflush (stderr);
      free (tmps);
      return -7;
   }
   error_check (memcmp (nums, tmps, nnum) != 0) {
      fputs ("error -8", stderr); fflush (stderr);
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
   fprintf (stderr, "insert front test (nnum=%d)\n", (int) nnum); fflush (stderr);
   for (k = 0; k != nnum; k++) {
      if (darr->n == 0) j = 0;
      else j = (size_t) rand () % darr->n;
      nums[k] = rand ();
      error_check (insert_front_darr (darr, j,
         nums + k) != 0) {
         fputs ("error -2", stderr); fflush (stderr);
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
   fprintf (stderr, "remove front test (nnum=%d)\n", (int) nnum); fflush (stderr);
   for (k = 0; k != nnum; k++) {
      if (darr->n == 0) j = 0;
      else j = (size_t) rand () % darr->n;
      remove_front_darr (darr, j, &num);
      error_check (trim_cap_darr (darr, nnum - k - 1) != 0) {
         fputs ("error -3", stderr); fflush (stderr);
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
   fprintf (stderr, "inserts front test (nnum=%d)\n", (int) nnum); fflush (stderr);
   for (k = 0; k != nnum; k++)
      nums[k] = rand ();
   error_check (inserts_front_darr (darr, (size_t) 0,
      nums, nnum) != 0) {
      fputs ("error -6", stderr); fflush (stderr);
      return -6;
   }
   return 0;
}

__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result))
static int removes_front_test (darr_t *restrict darr,
   int nums[], size_t nnum) {
   int *restrict tmps;
   fprintf (stderr, "removes front test (nnum=%d)\n", (int) nnum); fflush (stderr);
   tmps = malloc (sizeof (int) * nnum);
   error_check (tmps == NULL) {
      return -1;
   }
   removes_front_darr (darr, (size_t) 0, tmps, nnum);
   error_check (trim_cap_darr (darr, (size_t) 0) != 0) {
      fputs ("error -7", stderr); fflush (stderr);
      free (tmps);
      return -7;
   }
   error_check (memcmp (nums, tmps, nnum) != 0) {
      fputs ("error -8", stderr); fflush (stderr);
      free (tmps);
      return -8;
   }
   free (tmps);
   return 0;
}

__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result))
static int get_cb (darr_resize_cb_t *restrict resizecb,
   void *restrict *restrict cbargs) {
   int num;
   num = rand () % 3;
   switch (num) {
   case 0:
      *resizecb = darr_resize_exact;
      *cbargs   = NULL;
      return 0;
   case 1:
      *resizecb = darr_resize_linear;
      *cbargs = malloc (sizeof (size_t));
      error_check (*cbargs == NULL) return -1;
      /*sfactor = (size_t) (rand () + 1);*/
      *(size_t *restrict) (*cbargs) = 2;
      return 0;
   case 2:
      *resizecb = darr_resize_geometric;
      /*den = rand ();
      do num = rand ();
      while (num == 0);
      dfactor = (double) den / (double) num;*/
      *cbargs = malloc (sizeof (double));
      error_check (*cbargs == NULL) return -1;
      *(double *restrict) (*cbargs) = (double) 2;
      return 0;
   default: __builtin_unreachable ();
   }
}

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static int init_test (darr_t *restrict darr) {
   darr_resize_cb_t cb;
   void *restrict arg;
   fputs ("init_test ()", stderr); fflush (stderr);
   error_check (get_cb (&cb, &arg) != 0) return -1;
   return init_darr (darr, sizeof (int), cb, arg);
}

__attribute__ ((nonnull (1), nothrow))
static void free_test (darr_t *restrict darr) {
   fputs ("free_test ()", stderr); fflush (stderr);
   free_darr (darr);
   if (darr->cbargs != NULL) free (darr->cbargs);
}

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static int reset_test (darr_t *restrict darr) {
   fputs ("reset_test ()", stderr); fflush (stderr);
   free_test (darr);
   return init_test (darr);
}

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static int test0 (darr_t *restrict darr) {
   int num = rand ();
   size_t sz = darr->n;
   fprintf (stderr, "test0 () num:%d, sz:%d\n", num, (int) sz); fflush (stderr);
   error_check (insert_rear_darr (darr, &num) != 0) {
      fputs ("error -10", stderr); fflush (stderr);
      return -10;
   }
   error_check (sz + 1 != darr->n) return -1;
   return 0;
}

__attribute__ ((nonnull (1), nothrow))
static void get_nums (int nums[], size_t snum) {
   size_t k;
   for (k = 0; k != snum; k++)
      nums[k] = rand ();
}

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static int test1 (darr_t *restrict darr, size_t nnum) {
   size_t k;
   size_t snum = (size_t) rand () % nnum;
   size_t sz = darr->n;
   int *restrict nums;
   fprintf (stderr, "test1 () snum: %d, sz: %d\n", (int) snum, (int) sz); fflush (stderr);
   nums = malloc (sizeof (int) * snum);
   get_nums (nums, snum);
   error_check (inserts_rear_darr (darr, nums, snum) != 0) {
      fputs ("error -11", stderr); fflush (stderr);
      free (nums);
      return -11;
   }
   free (nums);
   error_check (sz + snum != darr->n) return -1;
   darr_print (darr);
   return 0;
}

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static int test2 (darr_t *restrict darr) {
   size_t num;
   size_t sz = darr->n;
   if (darr->n == 0) return 0;
   fprintf (stderr, "test2 () sz: %d\n", (int) sz); fflush (stderr);
   remove_rear_darr (darr, &num);
   error_check (sz - 1 != darr->n) return -1;
   darr_print (darr);
   return 0;
}

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static int test3 (darr_t *restrict darr) {
   size_t snum;
   int *restrict nums;
   size_t sz = darr->n;
   if (darr->n == 0) return 0;
   snum = (size_t) rand () % darr->n;
   fprintf (stderr, "test3 () sz: %d, snum: %d\n", (int) sz, (int) snum); fflush (stderr);
   nums = malloc (sizeof (int) * snum);
   error_check (nums == NULL) return -1;
   removes_rear_darr (darr, nums, snum);
   free (nums);
   error_check (sz - snum != darr->n) return -2;
   darr_print (darr);
   return 0;
}

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static int test4 (darr_t *restrict darr) {
   int num;
   size_t k;
   size_t sz = darr->n;
   if (darr->n == 0) k = 0;
   else k = (size_t) rand () % (darr->n + 1);
   num = rand ();
   fprintf (stderr, "test4 () sz: %d, k: %d, num: %d\n", (int) sz, (int) k, (int) num); fflush (stderr);
   error_check (insert_front_darr (darr, k, &num) != 0) {
      fputs ("error -12", stderr); fflush (stderr);
      return -12;
   }
   error_check (sz + 1 != darr->n) return -1;
   darr_print (darr);
   return 0;
}

__attribute__ ((nonnull (1, 2), nothrow, warn_unused_result))
static int test5 (darr_t *restrict darr, int nums[], size_t nnum) {
   size_t snum = (size_t) rand () % nnum;
   size_t k;
   size_t sz = darr->n;
   get_nums (nums, snum);
   if (darr->n == 0) k = 0;
   else k = ((size_t) rand ()) % (darr->n + 1);
   fprintf (stderr, "test5 () snum: %d, sz: %d\n", (int) snum, (int) sz); fflush (stderr);
   fprintf (stderr, "snum: %d\n", (int) snum); fflush (stderr);
   fprintf (stderr, "k   : %d\n", (int) k);    fflush (stderr);
   darr_print (darr);

   error_check (inserts_front_darr (darr, k, nums, snum) != 0) {
      fputs ("error -13", stderr); fflush (stderr);
      return -13;
   }
   error_check (sz + snum != darr->n) return -1;
   darr_print (darr);
   return 0;
}

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static int test6 (darr_t *restrict darr) {
   size_t k;
   int num;
   size_t sz = darr->n;
   if (darr->n == 0) return;
   k = (size_t) rand () % darr->n;
   fprintf (stderr, "test6 () sz: %d, k: %d\n", (int) sz, (int) k); fflush (stderr);
   remove_front_darr (darr, k, &num);
   error_check (sz - 1 != darr->n) return -1;
   darr_print (darr);
   return 0;
}

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static int test7 (darr_t *restrict darr) {
   size_t k;
   size_t snum;
   int *restrict nums;
   size_t sz = darr->n;
   if (darr->n == 0) return 0;
   if (darr->n == 1) {
      k    = 0;
      snum = 1;
   } else {
      k    = (size_t) rand () % darr->n;
      snum = (size_t) rand () % (darr->n - k);
   }
   fprintf (stderr, "test7 () sz: %d, k: %d, snum: %d\n", (int) sz, (int) k, (int) snum); fflush (stderr);
   /*snum = 1 + ((size_t) rand () % darr->n);
   k    = (size_t) rand () % (darr->n - snum);*/
   nums = malloc (sizeof (int) * snum);
   error_check (nums == NULL) return -1;
   removes_front_darr (darr, k, nums, snum);
   free (nums);
   error_check (sz - snum != darr->n) return -1;
   darr_print (darr);
   return 0;
}

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static int test8 (darr_t *restrict darr) {
   size_t snum;
   if (darr->n == 0) return 0;
   snum = (size_t) rand () % darr->n;
   fprintf (stderr, "test8 () snum: %d\n", (int) snum); fflush (stderr);
   error_check (trim_cap_darr (darr, snum) != 0) {
      fputs ("error -15", stderr); fflush (stderr);
      return -15;
   }
   error_check (darr->n > snum && !(snum == 0 && darr->n == 1)) return -1;
   darr_print (darr);
   return 0;
}

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static int test9 (darr_t *restrict darr) {
   darr_resize_cb_t cb;
   void *restrict cbargs;
   fprintf (stderr, "test9 ()\n"); fflush (stderr);
   if (darr->cbargs != NULL) free (darr->cbargs);
   error_check (get_cb (&cb, &cbargs) != 0) return -1;
   darr->resizecb = cb;
   darr->cbargs   = cbargs;
   darr_print (darr);
   return 0;
}

__attribute__ ((nothrow, warn_unused_result))
int main (void) {
   darr_t darr;
   time_t t;
   int nums[10];
   size_t ntest = 100;
   size_t testi;

   t = time (NULL);
   srand ((unsigned int) t);

   error_check (init_test (&darr) != 0) return -1;

   get_nums (nums, ARRSZ (nums));
   /*
   for (testi = 0; testi != ARRSZ (nums); testi++) {
      error_check (insert_rear_darr (&darr, nums + testi) != 0)
         return -1;
      error_check (trim_cap_darr (&darr, testi + 1) != 0)
         return -1;
      darr_print (&darr);
   }
   for (testi = 0; testi != ARRSZ (nums); testi++) {
      remove_rear_darr (&darr, nums + testi);
      error_check (trim_cap_darr (&darr, ARRSZ (nums) - testi - 1) != 0)
         return -1;
      darr_print (&darr);
      fprintf (stderr, "nums[%d]: %d\n", (int) testi, nums[testi]);
   }
   for (testi = 0; testi != ARRSZ (nums); testi++) {
      error_check (insert_rear_darr (&darr, nums + testi) != 0)
         return -1;
      error_check (trim_cap_darr (&darr, testi + 1) != 0)
         return -1;
      darr_print (&darr);
   }
   for (testi = 0; testi != ARRSZ (nums); testi++) {
      remove_rear_darr (&darr, nums + testi);
      error_check (trim_cap_darr (&darr, ARRSZ (nums) - testi - 1) != 0)
         return -1;
   }
   */
   /*
   error_check (inserts_rear_darr (&darr, nums, (size_t) 1) != 0) return -1;
   error_check (trim_cap_darr (&darr, (size_t) 1) != 0) return -1;
   darr_print (&darr);
   removes_rear_darr (&darr, nums, (size_t) 1);
   error_check (trim_cap_darr (&darr, (size_t) 0) != 0) return -1;
   darr_print (&darr);
   error_check (inserts_rear_darr (&darr, nums, ARRSZ (nums)) != 0) return -1;
   error_check (trim_cap_darr (&darr, ARRSZ (nums)) != 0) return -1;
   darr_print (&darr);
   removes_rear_darr (&darr, nums, ARRSZ (nums));
   error_check (trim_cap_darr (&darr, (size_t) 0) != 0) return -1;
   darr_print (&darr);
   */
   /*
   for (testi = 0; testi != ARRSZ (nums); testi++) {
      error_check (insert_front_darr (&darr, (size_t) 0, nums + testi) != 0)
         return -1;
      error_check (trim_cap_darr (&darr, testi + 1) != 0)
         return -1;
      darr_print (&darr);
   }
   for (testi = 0; testi != ARRSZ (nums); testi++) {
      remove_front_darr (&darr, (size_t) 0, nums + testi);
      error_check (trim_cap_darr (&darr, ARRSZ (nums) - testi - 1) != 0)
         return -1;
      darr_print (&darr);
      fprintf (stderr, "nums[%d]: %d\n", (int) testi, nums[testi]);
   }
   for (testi = 0; testi != ARRSZ (nums); testi++) {
      error_check (insert_front_darr (&darr, (size_t) 0, nums + testi) != 0)
         return -1;
      error_check (trim_cap_darr (&darr, testi + 1) != 0)
         return -1;
      darr_print (&darr);
   }
   for (testi = 0; testi != ARRSZ (nums); testi++) {
      remove_front_darr (&darr, (size_t) 0, nums + testi);
      error_check (trim_cap_darr (&darr, ARRSZ (nums) - testi - 1) != 0)
         return -1;
   }
   */
   /*
   for (testi = 0; testi != ARRSZ (nums); testi++) {
      error_check (insert_front_darr (&darr, darr.n, nums + testi) != 0)
         return -1;
      error_check (trim_cap_darr (&darr, testi + 1) != 0)
         return -1;
      darr_print (&darr);
   }
   for (testi = 0; testi != ARRSZ (nums); testi++) {
      remove_front_darr (&darr, darr.n - 1, nums + testi);
      error_check (trim_cap_darr (&darr, ARRSZ (nums) - testi - 1) != 0)
         return -1;
      darr_print (&darr);
      fprintf (stderr, "nums[%d]: %d\n", (int) testi, nums[testi]);
   }
   for (testi = 0; testi != ARRSZ (nums); testi++) {
      error_check (insert_front_darr (&darr, darr.n, nums + testi) != 0)
         return -1;
      error_check (trim_cap_darr (&darr, testi + 1) != 0)
         return -1;
      darr_print (&darr);
   }
   for (testi = 0; testi != ARRSZ (nums); testi++) {
      remove_front_darr (&darr, darr.n - 1, nums + testi);
      error_check (trim_cap_darr (&darr, ARRSZ (nums) - testi - 1) != 0)
         return -1;
   }
   */
   /*
   error_check (inserts_front_darr (&darr, (size_t) 0, nums, (size_t) 1) != 0) return -1;
   error_check (trim_cap_darr (&darr, (size_t) 1) != 0) return -1;
   darr_print (&darr);
   removes_front_darr (&darr, (size_t) 0, nums, (size_t) 1);
   error_check (trim_cap_darr (&darr, (size_t) 0) != 0) return -1;
   darr_print (&darr);
   error_check (inserts_front_darr (&darr, (size_t) 0, nums, ARRSZ (nums)) != 0) return -1;
   error_check (trim_cap_darr (&darr, ARRSZ (nums)) != 0) return -1;
   darr_print (&darr);
   removes_front_darr (&darr, (size_t) 0, nums, ARRSZ (nums));
   error_check (trim_cap_darr (&darr, (size_t) 0) != 0) return -1;
   darr_print (&darr);
   */
   /*
   error_check (inserts_front_darr (&darr, darr.n, nums, (size_t) 1) != 0) return -1;
   error_check (trim_cap_darr (&darr, (size_t) 1) != 0) return -1;
   darr_print (&darr);
   removes_front_darr (&darr, darr.n, nums, (size_t) 1);
   error_check (trim_cap_darr (&darr, (size_t) 0) != 0) return -1;
   darr_print (&darr);
   error_check (inserts_front_darr (&darr, darr.n, nums, ARRSZ (nums)) != 0) return -1;
   error_check (trim_cap_darr (&darr, ARRSZ (nums)) != 0) return -1;
   darr_print (&darr);
   removes_from_darr (&darr, darr.n, nums, ARRSZ (nums));
   error_check (trim_cap_darr (&darr, (size_t) 0) != 0) return -1;
   darr_print (&darr);
   */
   /*
   for (testi = 0; testi != ntest; testi++) {
      fprintf (stderr, "testi: %d\n", (int) testi);
      error_check (test0 (&darr) != 0) return -4;
      error_check (test1 (&darr, ARRSZ (nums)) != 0) return -5;
      error_check (test2 (&darr) != 0) return -6;
      error_check (test3 (&darr) != 0) return -6;
      error_check (test4 (&darr) != 0) return -7;
      error_check (test5 (&darr, nums, ARRSZ (nums)) != 0) return -5;
      error_check (test6 (&darr) != 0) return -7;
      error_check (test7 (&darr) != 0) return -7;
      error_check (test8 (&darr) != 0) return -8;
      error_check (test9 (&darr) != 0) return -9;
   }
   */
   /*
   for (testi = 0; testi != ntest; testi++) {
      fprintf (stderr, "testi: %d\n", (int) testi);
      error_check (test9 (&darr) != 0) return -9;
      error_check (test8 (&darr) != 0) return -8;
      error_check (test7 (&darr) != 0) return -7;
      error_check (test6 (&darr) != 0) return -7;
      error_check (test5 (&darr, nums, ARRSZ (nums)) != 0) return -5;
      error_check (test4 (&darr) != 0) return -7;
      error_check (test3 (&darr) != 0) return -6;
      error_check (test2 (&darr) != 0) return -6;
      error_check (test1 (&darr, ARRSZ (nums)) != 0) return -5;
      error_check (test0 (&darr) != 0) return -4;
   }
   */
   /*
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
   */
   /*
   error_check (test0 (&darr) != 0) return -4;
   error_check (test1 (&darr, ARRSZ (nums)) != 0) return -5;
   error_check (test2 (&darr) != 0) return -6;
   error_check (test3 (&darr) != 0) return -6;
   error_check (test4 (&darr) != 0) return -7;
   error_check (test5 (&darr, nums, ARRSZ (nums)) != 0) return -5;
   error_check (test6 (&darr) != 0) return -7;
   error_check (test7 (&darr) != 0) return -7;
   error_check (test8 (&darr) != 0) return -8;
   error_check (test9 (&darr) != 0) return -9;

   error_check (reset_test (&darr) != 0) return -3;

   error_check (test9 (&darr) != 0) return -9;
   error_check (test8 (&darr) != 0) return -8;
   error_check (test7 (&darr) != 0) return -7;
   error_check (test6 (&darr) != 0) return -7;
   error_check (test5 (&darr, nums, ARRSZ (nums)) != 0) return -5;
   error_check (test4 (&darr) != 0) return -7;
   error_check (test3 (&darr) != 0) return -6;
   error_check (test2 (&darr) != 0) return -6;
   error_check (test1 (&darr, ARRSZ (nums)) != 0) return -5;
   error_check (test0 (&darr) != 0) return -4;

   error_check (reset_test (&darr) != 0) return -3;
   */

   for (testi = 0; testi != ntest; testi++)
      switch (rand () % 10) {
      case 0:
         error_check (test0 (&darr) != 0) return -4;
         break;
      case 1:
         error_check (test1 (&darr, ARRSZ (nums)) != 0) return -5;
         break;
      case 2:
         error_check (test2 (&darr) != 0) return -6;
         break;
      case 3:
         error_check (test3 (&darr) != 0) return -6;
         break;
      case 4:
         error_check (test4 (&darr) != 0) return -7;
         break;
      case 5:
         error_check (test5 (&darr, nums, ARRSZ (nums)) != 0) return -5;
         break;
      case 6:
         error_check (test6 (&darr) != 0) return -7;
         break;
      case 7:
         error_check (test7 (&darr) != 0) return -7;
         break;
      case 8:
         error_check (test8 (&darr) != 0) return -8;
         break;
      case 9:
         error_check (test9 (&darr) != 0) return -9;
         break;
      default: __builtin_unreachable ();
      }

   free_test (&darr);

   fputs ("success", stderr); fflush (stderr);

   return EXIT_SUCCESS;
}
