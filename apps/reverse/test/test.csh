#!/bin/csh -f
../reverse -- test2.dat -test1.dat
echo "Does the above output look reversed?"
../reverse test1.dat >! ttt1
../reverse -- -test1.dat >! ttt2
diff ttt1 ttt2
../reverse test1.dat test2.dat >! ttt3
cat test1.dat test2.dat | ../reverse >! ttt4
echo test of reading files from commandline and piping
diff ttt3 ttt4
echo "You can now check through and remove the ttt* files"
