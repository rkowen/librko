#!/bin/sh

IN=USMID
OUT=rkowen@ckns.net

for f in `find . -not \( -type d -name CVS -prune \) -a -type f -print`
do
	if istext $f ; then
		if grep -q $IN $f ;then
			echo $f
#			sed -e "s/$IN/$OUT/g" < $f > $f.xxx && mv $f.xxx $f
			sed -e "/$IN/d" < $f > $f.xxx && mv $f.xxx $f
		fi
	fi
done
exit
