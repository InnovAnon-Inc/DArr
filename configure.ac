AC_INIT([libdarr], [0.0], [laurence.a.maddox@gmail.com])
AC_CONFIG_AUX_DIR([build-aux])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CC
AM_PROG_AR
LT_INIT([])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile
])

AC_ARG_ENABLE([use-submodules],
    AS_HELP_STRING([--enable-use-submodules], [Enable use submodules]))
AM_CONDITIONAL([USE_SUBMODULES], [test x"$enable_use_submodules" = "xyes"])

AC_CONFIG_MACRO_DIR([m4])
#AC_CONFIG_SUBDIRS([glitter Array])

AC_CHECK_HEADERS([stdlib.h sys/types.h])
#AC_CHECK_HEADERS([math.h])
AC_CHECK_HEADERS([glitter.h array.h])

#AC_CHECK_LIB([stdlib],[realloc])
AC_CHECK_LIB([array],[init_array,alloc_array,free_array])
AC_CHECK_LIB([array],[get_array,set_array,gets_array,sets_array])
#AC_CHECK_LIB([math],[fabs])

#srcdir = @srcdir@
#LIBDARR_LIBADD="`PATH=$(srcdir)src:$PATH libdarr_config --libs`"
LIBDARR_LIBADD="`PATH=src:$PATH libdarr_config --libs`"
AC_SUBST(LIBDARR_LIBADD)

#LIBDARR_RESIZE_LIBADD="`PATH=$(srcdir)src:$PATH libdarr_resize_config --libs`"
LIBDARR_RESIZE_LIBADD="`PATH=src:$PATH libdarr_resize_config --libs`"
AC_SUBST(LIBDARR_RESIZE_LIBADD)

#DARR_TEST_LIBADD="libdarr.la `PATH=src:$PATH libdarr_config --libs` libdarr-resize.la `PATH=src:$PATH libdarr_resize_config --libs`"
#DARR_TEST_LIBADD="`PATH=$(srcdir)src:$PATH darr_test_config --libs`"
DARR_TEST_LIBADD="`PATH=src:$PATH darr_test_config --libs`"
AC_SUBST(DARR_TEST_LIBADD)

INNOVANON_WARNINGS([])

AC_OUTPUT
