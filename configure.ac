#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([2048], [1], [github.com/wtfbbqhax/Twenty-Fourty-Eight/issues])
AC_CONFIG_SRCDIR([main.cc])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

AM_CXXFLAGS="-std=c++11"
AX_CXX_COMPILE_STDCXX_11
# Checks for libraries.
# FIXME: Replace `main' with a function in `-lao':
#AC_CHECK_LIB([ao], [main])

# Checks for header files.
AC_FUNC_ALLOCA
AC_CHECK_HEADERS([fcntl.h limits.h malloc.h stddef.h stdint.h stdlib.h string.h strings.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset rint strcasecmp strchr strdup strtol])

AC_SUBST(AM_CXXFLAGS)
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
