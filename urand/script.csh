#!/bin/csh -f
#
# this script will create and optimize urand.o
#  the first argument is the Ansi-C compiler to use
#
set echo
#
setenv CC "$1"
#
# run initial timing test
$CC -o testi -DTEST urand.c
./testi
#
# create object for optimization tests
$CC -c urand.c
#
# create timing tests
$CC -DDOPT        -o tmdiv tmdiv.c urand.o
$CC -DDOPT -DOPEQ -o tmsub tmsub.c urand.o
$CC -DDOPT -DOPEQ -o tmadd tmadd.c urand.o
#
# now for optimized options
echo -n "-DNOINTOVF -DNOADDMULT" >! .opt
tmdiv >> .opt
tmsub >> .opt
tmadd >> .opt
#
# run intermediate timing test
$CC -o testhf -DTEST `cat .opt` urand.c
./testhf
#
# run final timing test and inline urand
$CC -o testf -DTEST -DHACK_INLINE `cat .opt` urand.c
./testf
#
# create optimized object
$CC -c -DHACK_INLINE `cat .opt` urand.c
#
exit
