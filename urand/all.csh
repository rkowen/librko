#!/bin/csh -f
#
# this script will just run the timing programs to
#  display which operations are faster
#  the first argument is the Ansi-C compiler to use
#
if ( $#argv < 1) then
	cat <<!

$0 - runs through all the timing programs
	but it requires that the first argument be the
	compiler to use.

!
	exit 1
endif
set echo
#
setenv CC	"$*"
setenv CFLAGS	"-I. -I.. -I../include"
#setenv LDFLAGS	"-L. -L.. -lrko"
setenv LDFLAGS
#
# pre-create urand.o
$CC $CFLAGS -c urand.c
# create timing tests
$CC -DOPEQ tmadd.c $CFLAGS $LDFLAGS ./urand.o ../clocker.o && ./a.out
$CC        tmadd.c $CFLAGS $LDFLAGS ./urand.o ../clocker.o && ./a.out
$CC -DOPEQ tmsub.c $CFLAGS $LDFLAGS ./urand.o ../clocker.o && ./a.out
$CC        tmsub.c $CFLAGS $LDFLAGS ./urand.o ../clocker.o && ./a.out
$CC -DOPEQ tmdiv.c $CFLAGS $LDFLAGS ./urand.o ../clocker.o && ./a.out
$CC        tmdiv.c $CFLAGS $LDFLAGS ./urand.o ../clocker.o && ./a.out
rm -f a.out
#
#
exit
