#!/bin/sh

cp init.fdtest.4out fdtest.4out
cp cmp.fdtest.3inout fdtest.3inout

tfails=0

./fdtest 2> fdtest.testout

if diff fdtest.testout cmp.fdtest.testout ; then
	rm fdtest.testout
else
	echo failed add/process
	tfails=`expr $tfails + 1`
fi
if diff fdtest.out cmp.fdtest.out ; then
	rm fdtest.out
else
	echo failed stderr/stdout redirection
	tfails=`expr $tfails + 1`
fi
if diff fdtest.3inout cmp.fdtest.3inout ; then
	rm fdtest.3inout
else
	echo failed read/write
	tfails=`expr $tfails + 1`
fi
if diff fdtest.4out cmp.fdtest.4out ; then
	rm fdtest.4out
else
	echo failed append
	tfails=`expr $tfails + 1`
fi

if [ $tfails -eq 0 ]; then
	echo fdtest OK
else
	echo fdtest FAILED $tfails tests
fi

exit
