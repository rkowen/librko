static const char RCSID[]="@(#)$Id: isqrt.c,v 1.1 1998/11/24 05:57:03 rk Exp $";
static const char AUTHOR[]="@(#)isqrt 1.0 11/26/98 R.K.Owen,Ph.D.";
/* isqrt.c -
 * finds the the integer sqrt of the given integer.
 * returns 0 if a user error (such as the argument is non-positive)
 */
/*
 *********************************************************************
 *
 *     This software is copyrighted by R.K.Owen,Ph.D. 1998
 *
 * The author, R.K.Owen, of this software is not liable for any
 * problems WHATSOEVER which may result from use  or  abuse  of
 * this software. The author, R.K.Owen, grants unlimited rights
 * to anyone who uses and modifies this  software  for  private
 * non-commercial  use  as  long  as  this copyright and rights
 * notice remains in this software and is made available to all
 * recipients of this software.
 *
 * last known email: rkowen@ckns.net
 *                   rk@owen.sj.ca.us
 *
 *********************************************************************
 */

/* a look-up table of lut[i] = 256*sqrt(i/256) */
static unsigned char lut[256] = {
    0,   16,   22,   27,   32,   35,   39,   42,
   45,   48,   50,   53,   55,   57,   59,   61,
   64,   65,   67,   69,   71,   73,   75,   76,
   78,   80,   81,   83,   84,   86,   87,   89,
   90,   91,   93,   94,   96,   97,   98,   99,
  101,  102,  103,  104,  106,  107,  108,  109,
  110,  112,  113,  114,  115,  116,  117,  118,
  119,  120,  121,  122,  123,  124,  125,  126,
  128,  128,  129,  130,  131,  132,  133,  134,
  135,  136,  137,  138,  139,  140,  141,  142,
  143,  144,  144,  145,  146,  147,  148,  149,
  150,  150,  151,  152,  153,  154,  155,  155,
  156,  157,  158,  159,  160,  160,  161,  162,
  163,  163,  164,  165,  166,  167,  167,  168,
  169,  170,  170,  171,  172,  173,  173,  174,
  175,  176,  176,  177,  178,  178,  179,  180,
  181,  181,  182,  183,  183,  184,  185,  185,
  186,  187,  187,  188,  189,  189,  190,  191,
  192,  192,  193,  193,  194,  195,  195,  196,
  197,  197,  198,  199,  199,  200,  201,  201,
  202,  203,  203,  204,  204,  205,  206,  206,
  207,  208,  208,  209,  209,  210,  211,  211,
  212,  212,  213,  214,  214,  215,  215,  216,
  217,  217,  218,  218,  219,  219,  220,  221,
  221,  222,  222,  223,  224,  224,  225,  225,
  226,  226,  227,  227,  228,  229,  229,  230,
  230,  231,  231,  232,  232,  233,  234,  234,
  235,  235,  236,  236,  237,  237,  238,  238,
  239,  240,  240,  241,  241,  242,  242,  243,
  243,  244,  244,  245,  245,  246,  246,  247,
  247,  248,  248,  249,  249,  250,  250,  251,
  251,  252,  252,  253,  253,  254,  254,  255};

#define _ISQRT(NM, TYPE, OP) \
TYPE NM(TYPE a) { \
	TYPE t = a, i=0,r; if (a OP 0) return 0; \
	while (t & ~((TYPE) 0xFF)) { t >>=2; i++; } \
	r = lut[t]; if (i < 4) r >>= (4-i); else r <<= (i-4); \
	while ((t = r*r, (t > a) || ((a - 2*r - 1) >= t))) { \
	r+=(a/r); r>>=1; } return r; }

unsigned int uisqrt(unsigned int a) {
	unsigned int t = a, i=0,r;
	if (a == 0) return 0;
	while (t & ~((unsigned int) 0xFF)) {
		t >>=2;
		i++;
	}
	r = lut[t];
	if (i < 4) r >>= (4-i);
	else r <<= (i-4);
	while ((t = r*r, (t > a) || ((a - 2*r - 1) >= t))) {
		r+=(a/r);
		r>>=1;
	}
	return r;
}

/* just like templates - almost */
_ISQRT(hsqrt, short, <=)
_ISQRT(isqrt, int, <=)
_ISQRT(lsqrt, long, <=)
#ifdef _UISQRT
_ISQRT(chsqrt, char, <=)
_ISQRT(scsqrt, signed char, <=)
_ISQRT(ucsqrt, unsigned char, ==)
_ISQRT(uhsqrt, unsigned short, ==)
_ISQRT(uisqrt, unsigned int, ==)
_ISQRT(ulsqrt, unsigned long, ==)
#endif

#include <stdio.h>
#include <math.h>
#include <limits.h>

#define TESTOUT(NM, TYPE, arg) \
tot++; if((res1 = (long) NM(arg)) != (res2 = (long) sqrt((double) arg))) {\
	printf("FAIL:" #NM "(%ld)\t= %ld != %ld\n", (long) arg, res1, res2);\
	err++; } else \
	printf("OK  :" #NM "(%ld)\t= %ld\n", (long) arg, res1);

#define STESTOUT(NM, TYPE, arg) \
tot++; if((res1 = (long) NM(arg)) != (long) 0) { \
	printf("FAIL:" #NM "(%ld)\t= %ld != %ld\n", (long) arg, res1, (long) 0);\
	err++; } else \
	printf("OK  :" #NM "(%ld)\t= %ld\n", (long) arg, res1);

#define TESTRANGE(NM, TYPE, lo, hi) \
for (errsum = 0, i = (long) lo; i <= (long) hi; ++i, ++tot) { \
	if(((long) NM(i)) != ((long) sqrt((double) i))) errsum++; }\
	if (errsum) {\
	printf("FAIL:" #NM "(%ld:%ld)\t=> %d errs\n", (long) lo, (long) hi);\
	err+=errsum; } else \
	printf("OK  :" #NM "(%ld:%ld)\t=> no errs\n", (long) lo, (long) hi);


#define MTESTOUT(NM, TYPE) \
	TESTOUT(NM, TYPE, 0)\
	TESTOUT(NM, TYPE, 1)\
	TESTOUT(NM, TYPE, 2)\
	TESTOUT(NM, TYPE, 99)\
	TESTOUT(NM, TYPE, 100)\
	TESTOUT(NM, TYPE, 101)

int main() {
	int err=0, tot = 0, errsum=0;
	long i, res1, res2;

	(void) setvbuf(stdout, (char *)NULL, _IOLBF, 0);

#ifdef _UISQRT
	TESTRANGE(chsqrt, char, 0, 127)
	TESTOUT(chsqrt, char, CHAR_MAX)
	STESTOUT(scsqrt, signed char, -64)
	TESTRANGE(scsqrt, signed char, 0, 127)
	TESTOUT(scsqrt, signed char, SCHAR_MAX)
	TESTRANGE(ucsqrt, unsigned char, 0, 255)
	TESTOUT(ucsqrt, unsigned char, UCHAR_MAX)
#endif

	STESTOUT(hsqrt, short, -64)
	TESTRANGE(hsqrt, short, 0, 32767)
	TESTOUT(hsqrt, short, SHRT_MAX)
#ifdef _UISQRT
	TESTRANGE(uhsqrt, unsigned short, 0, 65535)
	TESTOUT(uhsqrt, unsigned short, SHRT_MAX)
#endif

	STESTOUT(isqrt, int, -64)
	TESTRANGE(isqrt, int, 0, 32767)
	TESTOUT(isqrt, int, INT_MAX)
#ifdef _UISQRT
	TESTRANGE(uisqrt, unsigned int, 0, 65535)
	TESTOUT(uisqrt, unsigned int, UINT_MAX)
#endif

	MTESTOUT(lsqrt, long)
	STESTOUT(lsqrt, long, -64)
	TESTRANGE(lsqrt, long, 0, 32767)
	TESTOUT(lsqrt, long, LONG_MAX)
#ifdef _UISQRT
	TESTRANGE(ulsqrt, unsigned long, 0, 65535)
	TESTOUT(ulsqrt, unsigned long, ULONG_MAX)
#endif

	if (err)
		printf("%d test failures out of %d\n", err, tot);
	else
		printf("no test failures out of %d\n", tot);

	return 0;
}
