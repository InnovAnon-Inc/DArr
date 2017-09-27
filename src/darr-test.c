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

__attribute__ ((nothrow, warn_unused_result))
int main (void) {
   darr_t darr;
   double factor = 2;
   time_t t;
   int num;
   int nums[100];
   int tmps[ARRSZ (nums)];
   size_t k;
   darr_resize_cb_t cbs[3];

   t = time (NULL);
   srand ((unsigned int) t);

   cbs[0] = darr_resize_exact;
   cbs[1] = darr_resize_linear;
   cbs[2] = darr_resize_geometric;

   error_check (init_darr (&darr, sizeof (int),
      darr_resize_geometric, &factor) != 0) {
      puts ("error -1"); fflush (stdout);
      return -1;
   }

   for (k = 0; k != ARRSZ (nums); k++) {
      nums[k] = rand ();
      error_check (insert_rear_darr (&darr, nums + k) != 0) {
         puts ("error -2"); fflush (stdout);
         free_darr (&darr);
         return -2;
      }
   }

   for (k = 0; k != ARRSZ (nums); k++) {
      remove_rear_darr (&darr, &num);
      error_check (trim_cap_darr (&darr, ARRSZ (nums) - k - 1) != 0) {
         puts ("error -3"); fflush (stdout);
         free_darr (&darr);
         return -3;
      }
      error_check (num != nums[ARRSZ (nums) - k - 1]) {
         puts ("error -4"); fflush (stdout);
         free_darr (&darr);
         return -4;
      }
   }

   /*
   free_darr (&darr);

   error_check (init_darr (&darr, sizeof (int),
      darr_resize_geometric, &factor) != 0) {
      puts ("error -5"); fflush (stdout);
      free_darr (&darr);
      return -5;
   }
   */

   for (k = 0; k != ARRSZ (nums); k++)
      nums[k] = rand ();
   error_check (inserts_rear_darr (&darr, nums, ARRSZ (nums)) != 0) {
      puts ("error -6"); fflush (stdout);
      free_darr (&darr);
      return -6;
   }
   removes_rear_darr (&darr, tmps, ARRSZ (tmps));
   error_check (trim_cap_darr (&darr, (size_t) 0) != 0) {
      puts ("error -7"); fflush (stdout);
      free_darr (&darr);
      return -7;
   }
   error_check (memcmp (nums, tmps, ARRSZ (nums)) != 0) {
      puts ("error -8"); fflush (stdout);
      free_darr (&darr);
      return -8;
   }

   /*
   free_darr (&darr);

   error_check (init_darr (&darr, sizeof (int),
      darr_resize_geometric, &factor) != 0) {
      puts ("error -5"); fflush (stdout);
      free_darr (&darr);
      return -5;
   }
   */

   for (k = 0; k != ARRSZ (nums); k++) {
      size_t j;
      if (darr.n == 0) j = 0;
      else j = (size_t) rand () % darr.n;
      nums[k] = rand ();
      /*nums[k] = k;
      j = 0;*/
      error_check (insert_front_darr (&darr, j,
         nums + k) != 0) {
         puts ("error -2"); fflush (stdout);
         free_darr (&darr);
         return -2;
      }
   }

   for (k = 0; k != ARRSZ (nums); k++) {
      size_t j;
      if (darr.n == 0) j = 0;
      else j = (size_t) rand () % darr.n;
      remove_front_darr (&darr, j, &num);
      error_check (trim_cap_darr (&darr, ARRSZ (nums) - k - 1) != 0) {
         puts ("error -3"); fflush (stdout);
         free_darr (&darr);
         return -3;
      }
      /*error_check (num != nums[j]) {
         puts ("error -4"); fflush (stdout);
         free_darr (&darr);
         return -4;
      }*/
   }

   /*
   free_darr (&darr);

   error_check (init_darr (&darr, sizeof (int),
      darr_resize_geometric, &factor) != 0) {
      puts ("error -5"); fflush (stdout);
      free_darr (&darr);
      return -5;
   }
   */

   for (k = 0; k != ARRSZ (nums); k++)
      nums[k] = rand ();
   error_check (inserts_front_darr (&darr, 0, nums, ARRSZ (nums)) != 0) {
      puts ("error -6"); fflush (stdout);
      free_darr (&darr);
      return -6;
   }
   removes_front_darr (&darr, 0, tmps, ARRSZ (tmps));
   error_check (trim_cap_darr (&darr, (size_t) 0) != 0) {
      puts ("error -7"); fflush (stdout);
      free_darr (&darr);
      return -7;
   }
   error_check (memcmp (nums, tmps, ARRSZ (nums)) != 0) {
      puts ("error -8"); fflush (stdout);
      free_darr (&darr);
      return -8;
   }

   /*
   switch (rand () % 4) {
   case 0:
      insert_rear_darr ();
      break;
   case 1:
      inserts_rear_darr ();
      break;
   case 2:
      remove_rear_darr ();
      break;
   case 3:
      removes_rear_darr ();
      break;
   case 4:
      darr_trim_cap ();
      break;
   case 5:
      darr.resizecb = cbs[?];
      darr.cbargs   = cbargs[?]; // TODO use random factor
      break;
   default: __builtin_unreachable ();
   }
   */

   free_darr (&darr);

   puts ("success"); fflush (stdout);

   return EXIT_SUCCESS;
}
