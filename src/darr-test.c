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
   double dfactor = 2;
   size_t sfactor = 2;
   time_t t;
   int num;
   int nums[100];
   int tmps[ARRSZ (nums)];
   size_t k;
   size_t snum;
   /*darr_resize_cb_t cbs[3];*/

   t = time (NULL);
   srand ((unsigned int) t);

   /*cbs[0] = darr_resize_exact;
   cbs[1] = darr_resize_linear;
   cbs[2] = darr_resize_geometric;*/

   error_check (init_darr (&darr, sizeof (int),
      darr_resize_geometric, &dfactor) != 0) {
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
   error_check (inserts_front_darr (&darr, (size_t) 0,
      nums, ARRSZ (nums)) != 0) {
      puts ("error -6"); fflush (stdout);
      free_darr (&darr);
      return -6;
   }
   removes_front_darr (&darr, (size_t) 0, tmps, ARRSZ (tmps));
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

   switch (rand () % 4) {
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
      else k = rand () % darr.n;
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
      error_check (removes_front_darr (&darr, k, nums, snum) != 0) {
         puts ("error -14"); fflush (stdout);
         free_darr (&darr);
         return -14;
      }
      break;
   case 8:
      if (darr.n == 0); break;
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
         sfactor = (size_t) (rand () + 1);
         darr.cbargs = &sfactor;
         break;
      case 2:
         darr.resizecb = darr_resize_geometric;
         k = rand ();
         do num = rand ();
         while (num == 0);
         dfactor = (double) k / (double) num;
         darr.cbargs = &dfactor;
         break;
      default: __builtin_unreachable ();
      }
      darr.resizecb = cbs[num];
      darr.cbargs   = cbargs[?]; // TODO use random factor
      break;
   default: __builtin_unreachable ();
   }

   free_darr (&darr);

   puts ("success"); fflush (stdout);

   return EXIT_SUCCESS;
}
