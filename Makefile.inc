#
# Makefile.inc - define common macros
#
# by R.K. Owen, Ph.D.
#
#  customize for your own environment
#
PERM	= 644
#OWN	= rk
#GRP	= owen
ROOT	= $(HOME)
LIBDIR	= $(ROOT)/lib
INCDIR	= $(ROOT)/include
MANDIR	= $(ROOT)/man
ECHO	= echo
RANLIB	= $(ECHO)
SHELL	= /bin/sh
AR	= ar
ARFLAGS	= r
CC	= cc
CFLAGS	= -g -DRKOERROR
NROFF	= nroff
MAN	= -man
RM	= rm
CP	= cp
BSNM	= basename
INSTALL	= etc/cpset
SHAR	= etc/mkshar

