#!/bin/sh

cp init.fdtest.4out fdtest.4out
cp cmp.fdtest.3inout fdtest.3inout

tfails=0

./fdtest 2> fdtest.testout

if ! diff fdtest.testout cmp.fdtest.testout ; then
	echo failed add/process
	tfails=`expr $tfails + 1`
else
	rm fdtest.testout
fi
if ! diff fdtest.out cmp.fdtest.out ; then
	echo failed stderr/stdout redirection
	tfails=`expr $tfails + 1`
else
	rm fdtest.out
fi
if ! diff fdtest.3inout cmp.fdtest.3inout ; then
	echo failed read/write
	tfails=`expr $tfails + 1`
else
	rm fdtest.3inout
fi
if ! diff fdtest.4out cmp.fdtest.4out ; then
	echo failed append
	tfails=`expr $tfails + 1`
else
	rm fdtest.4out
fi

if [ $tfails -eq 0 ]; then
	echo fdtest OK
else
	echo fdtest FAILED $tfails tests
fi

exit
