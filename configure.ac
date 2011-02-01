#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([cacampc], [0.1], [mcfiredrill@gmail.com])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
PKG_PROG_PKG_CONFIG

#variables
AC_SUBST(AM_CPPFLAGS)

AC_SUBST(CACAMPC_LIBS)
CACAMPC_LIBS=""

# Checks for libraries.
PKG_CHECK_MODULES([caca], [caca], [
	AC_SUBST(caca_LIBS)
	AC_SUBST(caca_CFLAGS)
	CPPFLAGS="$CPPFLAGS $caca_CFLAGS"
	AC_CHECK_HEADERS([caca.h],
	LDFLAGS="$LDFLAGS $caca_LIBS", AC_MSG_ERROR([missing caca.h header]))],
	AC_MSG_ERROR([libcaca is required!])
)
PKG_CHECK_MODULES([libmpdclient], [libmpdclient >= 2.1], [
	AC_SUBST(libmpdclient_LIBS)
	AC_SUBST(libmpdclient_CFLAGS)
	CPPFLAGS="$CPPFLAGS $libmpdclient_CFLAGS"
	AC_CHECK_HEADERS([mpd/client.h],
	LDFLAGS="$LDFLAGS $libmpdclient_LIBS", AC_MSG_ERROR([missing mpd/client.h header]))],
	AC_MSG_ERROR([libmpdclient >= 2.1 is required!])
)

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h caca.h mpd/client.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
