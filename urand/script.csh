#!/bin/csh -f
#
# this script will create and optimize urand.o
#  the first argument is the Ansi-C compiler to use
#
if ( $#argv < 1) then
	cat <<!

$0 - runs through all the timing programs to create an optimized urand.o
	but it requires that the first argument be the Std-C
	compiler to use ... the successive arguments are just pasted in

!
	exit 1
endif
set echo
#
setenv CC	"$*"
setenv CFLAGS	"-I. -I.."
setenv LDFLAGS	"-L. -L.. -lrko"
#
# run initial timing test
$CC -o testi -DTEST urand.c $CFLAGS $LDFLAGS
./testi
#
# create object for optimization tests
$CC -c urand.c $CFLAGS
#
# create timing tests
$CC -DDOPT        -o tmdiv tmdiv.c urand.o $CFLAGS $LDFLAGS
$CC -DDOPT -DOPEQ -o tmsub tmsub.c urand.o $CFLAGS $LDFLAGS
$CC -DDOPT -DOPEQ -o tmadd tmadd.c urand.o $CFLAGS $LDFLAGS
#
# now for optimized options
echo -n "-DNOINTOVF -DNOADDMULT" >! .opt
tmdiv >> .opt
tmsub >> .opt
tmadd >> .opt
#
# run intermediate timing test
$CC -o testhf -DTEST `cat .opt` urand.c $CFLAGS $LDFLAGS
./testhf
#
# run final timing test and inline urand
$CC -o testf -DTEST -DHACK_INLINE `cat .opt` urand.c $CFLAGS $LDFLAGS
./testf
#
# create optimized object
$CC -c -DHACK_INLINE `cat .opt` urand.c $CFLAGS
#
exit
