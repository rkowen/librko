#
# Makefile.inc - define common macros
#
# by R.K. Owen, Ph.D.
#
#  customize for your own environment
#
LIB	= librko.a
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
TAR	= tar
AR	= ar
ARFLAGS	= r
CC	= cc
CFLAGS	= -g -DRKOERROR
LD	=cc -o
NROFF	= nroff
MAN	= -man
RM	= rm
CP	= cp
CD	= cd
BSNM	= basename
INSTALL	= etc/cpset
SHAR	= etc/mkshar
LEX	=flex
LEXFLAGS=
YACC	=yacc
YYFLAGS	=-v -d -t
YYSED	=../etc/yy-sed
#
