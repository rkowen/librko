dnl
dnl File:	configure.ac
dnl Revision:	$Id: configure.ac,v 1.2 2002/06/24 19:20:09 rk Exp $
dnl
dnl Process this file with autoconf to produce a configure script.
dnl You need autoconf 2.52 or better
dnl
dnl ---------------------------------------------------------------------------
AC_INIT(librko, 1.0, librko@kooz.sj.ca.us)
AC_CONFIG_AUX_DIR(config)
AM_INIT_AUTOMAKE(librko,1.0)
AC_CONFIG_SRCDIR([version.c])
AM_CONFIG_HEADER([config.h])
AM_PROG_CC_C_O
AC_PREREQ(2.52)

dnl ---------------------------------------------------------------------------
# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_RANLIB
AC_PROG_MAKE_SET

dnl ---------------------------------------------------------------------------
# Checks for libraries.
# FIXME: Replace `main' with a function in `-ll':
# AC_CHECK_LIB([l], [main])
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [sqrt])
# FIXME: Replace `main' with a function in `-lrko':
# AC_CHECK_LIB([rko], [main])
# FIXME: Replace `main' with a function in `-lrko_m':
# AC_CHECK_LIB([rko_m], [main])

dnl ---------------------------------------------------------------------------
dnl Autoheader "templates" (was in acconfig.h)
AH_TOP([
/** ** Copyright *********************************************************** **
 ** 									     **
 ** Copyright 2001 by R.K.Owen,Ph.D.		                      	     **
 ** last known email: librko@kooz.sj.ca.us				     **
 **                   rk@owen.sj.ca.us					     **
 ** see LICENSE.LGPL, which must be provided, for details		     **
 ** 									     **
 ** ************************************************************************ **/

#  ifndef _PRECISION_
#    define _PRECISION_
     typedef double PRECISION;
#  endif

#  ifndef _INTEGER_
#    define _INTEGER_
     typedef int INTEGER;
#  endif

])
dnl ---------------------------------------------------------------------------
# Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS([arpa/inet.h fcntl.h limits.h netdb.h netinet/in.h stdlib.h string.h strings.h sys/socket.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STAT
AC_C_CONST
AC_TYPE_UID_T
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T
AC_CHECK_MEMBERS([struct stat.st_blksize])
AC_STRUCT_ST_BLOCKS
AC_CHECK_MEMBERS([struct stat.st_rdev])
AC_HEADER_TIME
AC_STRUCT_TM
AC_STRUCT_TIMEZONE

# Checks for library functions.
AC_FUNC_FORK
AC_FUNC_LSTAT
AC_FUNC_MALLOC
AC_FUNC_SETVBUF_REVERSED
AC_FUNC_STAT
AC_FUNC_STRTOD
AC_CHECK_FUNCS([alarm dup2 getcwd gethostbyaddr gethostbyname inet_ntoa memmove memset socket sqrt strcasecmp strchr strpbrk strrchr strstr strtol])

AC_CONFIG_FILES([Makefile
                 tests/Makefile])
AC_OUTPUT
