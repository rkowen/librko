#!/bin/csh -f
#
# runs the metropolis test suite
cat <<HERE

For the first test (tmet), just observe whether the the last column is
approaching 1.0000 or not.  It may occur that it becomes 1.0000, but then
get's "frozen" at a higher value...(not too high).
It will run the traveling saleman test for various permutations of
SWAP/PERMUTE/REVERSE for fixed & non-fixed endpoints.

The following tests will run various examples and compare to a "known"
solution and return "OK" or "FAIL" depending on the outcome.

HERE
echo -n " Hit Return >"
set xxx=$<
foreach file ( tmet tranindex transwap tranperm tidxswap tidxrevr \
		 tidxperm)
	echo ==== start of $file ====
	$file | more
	echo -n ==== end of $file ==== " Hit Return >"
	set xxx=$<
end
exit
