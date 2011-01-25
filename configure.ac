#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([cacampc], [0.1], [mcfiredrill@gmail.com])
AC_CONFIG_SRCDIR([main.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL

# Checks for libraries.
AC_SEARCH_LIBS([caca_create_canvas], [caca])
AC_SEARCH_LIBS([mpd_connection_new], [mpdclient])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h caca.h mpd/client.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
