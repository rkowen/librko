/* tgcd.c -
 * tests the Euclid's greatest common divisor algorith implementation
 * for a number of integer types.
 */

#include "librko.h"

#define TESTOUT(NM,TYPE,arg1,arg2,result) \
if((res = NM(arg1,arg2)) != result) {\
	printf("FAIL:" #NM "(%4d,%4d)\t = %d != %d\n", arg1,arg2,res,result);\
	err++; } else \
	printf("OK  :" #NM "(%4d,%4d)\t = %d\n", arg1,arg2,res); tot++;

#define MTESTOUT(NM, TYPE) \
	TESTOUT(NM, TYPE, 2322, 654, 6); \
	TESTOUT(NM, TYPE, 654, 2322, 6); \
	TESTOUT(NM, TYPE, -654, 2322, 0); \
	TESTOUT(NM, TYPE, 18, 35, 1); \
	TESTOUT(NM, TYPE, 48, 42, 6); \
	TESTOUT(NM, TYPE, 100, 300, 100); \
	TESTOUT(NM, TYPE, 100, 325, 25);

#define UTESTOUT(NM, TYPE) \
	TESTOUT(NM, TYPE, 2322, 654, 6); \
	TESTOUT(NM, TYPE, 654, 2322, 6); \
	TESTOUT(NM, TYPE, 18, 35, 1); \
	TESTOUT(NM, TYPE, 48, 42, 6); \
	TESTOUT(NM, TYPE, 100, 300, 100); \
	TESTOUT(NM, TYPE, 100, 325, 25);

int main() {
	int err = 0, tot = 0, res;

	TESTOUT(scgcd, signed char, 48, 42, 6);
	TESTOUT(scgcd, signed char, 18, 35, 1);
	TESTOUT(scgcd, signed char, -18, 35, 0);
	TESTOUT(cgcd, char, 48, 42, 6);
	TESTOUT(cgcd, char, 18, 35, 1);
	TESTOUT(ucgcd, unsigned char, 48, 42, 6);
	TESTOUT(ucgcd, unsigned char, 18, 35, 1);
	MTESTOUT(hgcd,short)
	UTESTOUT(uhgcd,unsigned short)
	MTESTOUT(igcd,int)
	UTESTOUT(ugcd,unsigned int)
	MTESTOUT(lgcd,long)
	UTESTOUT(ulgcd,unsigned long)

	if (err)
		printf("%d test failures out of %d\n", err, tot);
	else
		printf("no test failures out of %d\n", err, tot);

	return 0;
}
