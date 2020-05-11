#if HAVE_CONFIG_H
#include <config.h>
#endif

#define _POSIX_C_SOURCE 200112L
#define __STDC_VERSION__ 200112L

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <swap.h>
#include <mmalloc.h>
#include <simon.h>

#include <array.h>

#ifndef TEST
static void data_print (void const *restrict data,
   size_t i, size_t j) {
   fprintf (stderr, "["); fflush (stderr);
   /*if (array->n != 0) {*/
   if (i != j) {
      fprintf (stderr, "%d", ((int const *restrict) data)[i]); fflush (stderr);
      for (i++; i != j; i++)
         fprintf (stderr, ", %d", ((int const *restrict) data)[i]); fflush (stderr);
   }
   fprintf (stderr, "]\n"); fflush (stderr);
}

/* must provide callback for printing array->data */

__attribute__ ((nonnull (1), nothrow))
static void array_print (array_t const *restrict array,
   size_t i, size_t j) {
   fprintf (stderr, "esz : %d\n", (int) array->esz);  fflush (stderr);
   fprintf (stderr, "maxn: %d\n", (int) array->n); fflush (stderr);
   data_print (array->data, i, j);
}

__attribute__ ((nonnull (1), nothrow))
static void get_nums (int nums[], size_t snum, int maxnum) {
   size_t k;
   for (k = 0; k != snum; k++)
      nums[k] = rand () % maxnum;
}











__attribute__ ((nonnull (1), nothrow))
static void dumpq(array_t const *restrict q) {
   size_t i;
   fputs ("Q: ", stderr);
   for (i = 0; i != q->n; i++) {
      void *restrict head = index_array (q, i);
      fprintf (stderr, "(%1d:%3d), ", (int) i, *(int *restrict) head);
   }
   fputs ("\n", stderr);
}










__attribute__ ((nothrow, warn_unused_result))
int main (void) {
   array_t array;
   time_t t;
   int nums[10];
   int maxn = 20;
   size_t ntest = 100;
   size_t testi;
   int valid[ARRSZ (nums)];

   t = time (NULL);
   srand ((unsigned int) t);

   error_check (alloc_array (&array, sizeof (int), ARRSZ (nums)) != 0)
      return -2;

   get_nums (nums, ARRSZ (nums), maxn);
   data_print (nums, (size_t) 0, ARRSZ (nums));

   array_print (&array, (size_t) 0, (size_t) 0);
   for (testi = 0; testi != ARRSZ (nums); testi++) {
      fprintf (stderr, "nums[%d]: %d\n", (int) testi, nums[testi]);
      set_array (&array, testi, nums + testi);
      array_print (&array, (size_t) 0, testi + 1);
   }

   get_nums (nums, ARRSZ (nums), maxn);
   data_print (nums, (size_t) 0, ARRSZ (nums));

   for (testi = 0; testi != ARRSZ (nums); testi++) {
      get_array (&array, testi, nums + testi);
      data_print (nums, (size_t) 0, testi + 1);
   }

   get_nums (nums, ARRSZ (nums), maxn);
   data_print (nums, (size_t) 0, ARRSZ (nums));

   array_print (&array, (size_t) 0, (size_t) 0);
   sets_array (&array, (size_t) 0, nums, ARRSZ (nums));
   array_print (&array, (size_t) 0, ARRSZ (nums));

   get_nums (nums, ARRSZ (nums), maxn);
   data_print (nums, (size_t) 0, ARRSZ (nums));

   gets_array (&array, (size_t) 0, nums, ARRSZ (nums));
   data_print (nums, (size_t) 0, ARRSZ (nums));



   for (testi = 0; testi != ARRSZ (nums); testi++)
      valid[testi] = (int) testi;
   for (testi = 0; testi != ARRSZ (nums); testi++)
      set_array (&array, testi, valid + testi);
   for (testi = 0; testi != ARRSZ (nums); testi++)
      get_array (&array, testi, nums + testi);
   for (testi = 0; testi != ARRSZ (nums); testi++)
      assert (valid[testi] == nums[testi]);

   for (testi = 0; testi != ARRSZ (nums); testi++)
      valid[testi] = (int)  (ARRSZ (nums) - testi);
   sets_array (&array, (size_t) 0, valid, ARRSZ (nums));
   gets_array (&array, (size_t) 0, nums,  ARRSZ (nums));
   for (testi = 0; testi != ARRSZ (nums); testi++)
      assert (valid[testi] == nums[testi]);

   for (testi = 0; testi != ARRSZ (nums); testi++)
      valid[testi] = (int) testi;
   sets_array (&array, (size_t) 0, valid, ARRSZ (nums));
   for (testi = 0; testi != ARRSZ (nums) / 2; testi++) {
      cp_array (&array, testi, ARRSZ (nums) / 2 + testi);
      valid[ARRSZ (nums) / 2 + testi] = valid[testi];
   }
   gets_array (&array, (size_t) 0, nums,  ARRSZ (nums));
   for (testi = 0; testi != ARRSZ (nums); testi++)
      assert (valid[testi] == nums[testi]);

   for (testi = 0; testi != ARRSZ (nums); testi++)
      valid[testi] = (int) (ARRSZ (nums) - testi);
   sets_array (&array, (size_t) 0, valid, ARRSZ (nums));
   cps_array (&array, (size_t) 0, ARRSZ (nums) / 2, ARRSZ (nums) / 2);
   for (testi = 0; testi != ARRSZ (nums) / 2; testi++)
      valid[ARRSZ (nums) / 2 + testi] = valid[testi];
   gets_array (&array, (size_t) 0, nums,  ARRSZ (nums));
   for (testi = 0; testi != ARRSZ (nums); testi++)
      assert (valid[testi] == nums[testi]);

   for (testi = 0; testi != ARRSZ (nums); testi++)
      valid[testi] = (int) testi;
   sets_array (&array, (size_t) 0, valid, ARRSZ (nums));
   for (testi = 0; testi != ARRSZ (nums) - 1; testi++) {
      mvs_array (&array, testi, testi + 1, ARRSZ (nums) - testi - 1);
      valid[testi + 1] = valid[0];
   }
   gets_array (&array, (size_t) 0, nums,  ARRSZ (nums));
   for (testi = 0; testi != ARRSZ (nums); testi++)
      assert (valid[testi] == nums[testi]);

   for (testi = 0; testi != ARRSZ (nums); testi++)
      valid[testi] = (int) testi;
   sets_array (&array, (size_t) 0, valid, ARRSZ (nums));
   dumpq (&array);
   for (testi = 0; testi < ARRSZ (nums) / 2; testi += 2) {
      if (testi == ARRSZ (nums) - testi - 2) continue;
      swaps_array2 (&array, testi, ARRSZ (nums) - testi - 2, (size_t) 2);
      dumpq (&array);
      swap2 (valid + testi, valid + ARRSZ (nums) - testi - 2, sizeof (int));
      swap2 (valid + testi + 1, valid + ARRSZ (nums) - testi - 1, sizeof (int));
   }
   gets_array (&array, (size_t) 0, nums,  ARRSZ (nums));
   for (testi = 0; testi != ARRSZ (nums); testi++)
      assert (valid[testi] == nums[testi]);

   for (testi = 0; testi != ARRSZ (nums); testi++)
      valid[testi] = (int) (ARRSZ (nums) - testi);
   sets_array (&array, (size_t) 0, valid, ARRSZ (nums));
   for (testi = 0; testi != ARRSZ (nums); testi++)
      assert (indexOf_array (&array, valid + testi) == testi);
   for (testi = 0; testi != ARRSZ (nums); testi++)
      assert (contains_array (&array, valid + testi));

   for (testi = 0; testi != ARRSZ (nums); testi++)
      valid[testi] = (int) (20 + testi);
   for (testi = 0; testi != ARRSZ (nums); testi++)
      assert (! contains_array (&array, valid + testi));

   free_array (&array);

   fputs ("success", stderr); fflush (stderr);

   return EXIT_SUCCESS;
}
#else



__attribute__ ((nonnull (1), nothrow))
static void dumpq(array_t const *restrict q) {
   size_t i;
   fputs ("Q: ", stderr);
   for (i = 0; i != used_space_caq (q); i++) {
      void *restrict head = index_array (q, i);
      fprintf (stderr, "(%1d:%3d), ", (int) i, *(int *restrict) head);
   }
   fputs ("\n", stderr);
}

typedef struct {
   size_t esz;
   size_t n;
} alloc_t;

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static void *array_alloc (void const *restrict arg_) {
   alloc_t const *restrict arg = (alloc_t const *restrict) arg_;
   return ez_alloc_array (arg->esz, arg->n);
}

__attribute__ ((nonnull (1), nothrow))
static void generate_int (void *restrict arg_) {
   int *restrict arg = (int *restrict) arg_;
   *arg = random_range_java (-10, 10); /* arbitrary params */
}

__attribute__ ((nonnull (1), nothrow))
static void generates_int (void *restrict arg_, size_t n) {
   int *restrict arg = (int *restrict) arg_;
   size_t i;
   for (i = 0; i != n; i++)
      arg[i] = random_range_java (-10, 10); /* arbitrary params */
}

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static int array_add_test (void *restrict arg_) {
   int tmp;
   TODO (set_test)
   int err = add_test (arg_, &tmp,
      (isfull_t) ???, generate_int, (add_t) enqueue);
   if (err == TEST_NA) return 0;
   error_check (err != 0) return -1;
   fprintf (stderr, "caq_add_test (), tmp:%d\n", tmp);
   dumpq ((array_t *restrict) arg_);
   return 0;
}

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static int array_remove_test (void *restrict arg_) {
   int tmp;
   TODO (get_test)
   int err = remove_test (arg_, &tmp,
      (isempty_t) isempty, (remove_t) dequeue);
   if (err == TEST_NA) return 0;
   error_check (err != 0) return -1;
   fprintf (stderr, "caq_remove_test (), tmp:%d\n", tmp);
   dumpq ((array_t *restrict) arg_);
   return 0;
}

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static int array_adds_test (void *restrict arg_) {
   int tmps[13]; /* arbitrary params */
   TODO (adds_test)
   error_check (adds_test (arg_, tmps, ARRSZ (tmps),
      (remaining_space_t) remaining_space_caq,
      generates_int, (adds_t) enqueues) != 0)
      return -1;
   /* can't print tmps, because we don't know how many elements are init'd */
   fprintf (stderr, "caq_adds_test ()\n");
   dumpq ((array_t *restrict) arg_);
   return 0;
}

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static int array_removes_test (void *restrict arg_) {
   int tmps[12]; /* arbitrary params */
   TODO (removes_test)
   error_check (removes_test (arg_, tmps, ARRSZ (tmps),
      (used_space_t) used_space_caq, (removes_t) dequeues) != 0)
      return -1;
   /* can't print tmps, because we don't know how many elements are init'd */
   fprintf (stderr, "caq_removes_test ()\n");
   dumpq ((array_t *restrict) arg_);
   return 0;
}

__attribute__ ((nonnull (1), nothrow, warn_unused_result))
static int array_cb (void *restrict arg) {
   stdcb_t tests[4];

   TODO (more tests)
   tests[0] = array_add_test;
   tests[1] = array_remove_test;
   tests[2] = array_adds_test;
   tests[3] = array_removes_test;

   error_check (random_ops (arg, tests, ARRSZ (tests), 100) != 0) /* arbitrary params */
   /*random_ops2 (arg, tests, ARRSZ (tests));*/
      return -1;

   return 0;
}

int main(void) {
   time_t t;
   alloc_t alloc_arg;

   t = time (NULL);
   srand ((unsigned int) t);

   alloc_arg.esz = sizeof (int);
   alloc_arg.n   = 10; /* arbitrary params */

   error_check (ezmalloc (array_alloc, &alloc_arg,
      array_cb,
      (do_free_t) ez_free_array) != 0)
      return EXIT_FAILURE;

   return EXIT_SUCCESS;
}
#endif
