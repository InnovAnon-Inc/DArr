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

#include <darr.h>
#include <darr-resize.h>

__attribute__ ((nothrow, warn_unused_result))
int main (void) {
   darr_t darr;
   double factor = 2;
   time_t t;
   int num;
   int nums[100];
   int tmps[ARRSZ (nums)];
   size_t k;

   t = time (NULL);
   srand ((unsigned int) t);

   error_check (init_darr (&darr, sizeof (int),
      darr_resize_geometric, &factor) != 0) {
      puts ("error -1"); fflush (stdout);
      return -1;
   }

   for (k = 0; k != ARRSZ (nums); k++) {
      nums[k] = rand ();
      error_check (insert_rear_darr (&darr, nums + k) != 0) {
         puts ("error -2"); fflush (stdout);
         return -2;
      }
   }

   for (k = 0; k != ARRSZ (nums); k++) {
      remove_rear_darr (&darr, &num);
      error_check (trim_cap_darr (&darr, ARRSZ (nums) - k) != 0) {
         puts ("error -3"); fflush (stdout);
         return -3;
      }
      error_check (num != nums[ARRSZ (nums) - k - 1]) {
         puts ("error -4"); fflush (stdout);
         return -4;
      }
   }

   free_darr (&darr);
   error_check (init_darr (&darr, sizeof (int),
      darr_resize_geometric, &factor) != 0) {
      puts ("error -5"); fflush (stdout);
      return -5;
   }

   for (k = 0; k != ARRSZ (nums); k++)
      nums[k] = rand ();
   error_check (inserts_rear_darr (&darr, nums, ARRSZ (nums)) != 0) {
      puts ("error -6"); fflush (stdout);
      return -6;
   }
   removes_rear_darr (&darr, tmps, ARRSZ (tmps));
   error_check (trim_cap_darr (&darr, (size_t) 0) != 0) {
      puts ("error -7"); fflush (stdout);
      return -7;
   }
   error_check (memcmp (nums, tmps, ARRSZ (nums)) != 0) {
      puts ("error -8"); fflush (stdout);
      return -8;
   }

   free_darr (&darr);

   puts ("success"); fflush (stdout);

   return EXIT_SUCCESS;
}
