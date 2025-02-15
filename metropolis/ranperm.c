static const char RCSID[]="@(#)$Id: ranperm.c,v 1.5 2005/03/06 02:58:13 rk Exp $";
static const char AUTHOR[]="@(#)ranperm 02/15/93 1.0 R.K.Owen,PhD";
/*
 * ranperm - will return one random integer on the interval (low,high-1)
 *   in the variables cut.
 *   The user must also give a pseudo-random number generator that
 *   produces a number in the interval [0.,1.)
 *
 * author	R.K.Owen,Ph.D.	02/15/1993
 */
/* 
 *     This software is copyrighted by R.K.Owen,Ph.D. 1996
 * 
 * The author, R.K.Owen, of this software is not liable for any
 * problems WHATSOEVER which may result from use  or  abuse  of
 * this  software.
 * The author, R.K.Owen, licenses this software under the
 * GNU LESSER GENERAL PUBLIC LICENSE
 * which can be found at http://www.gnu.org
 *
 * last known email: librko@kooz.sj.ca.us
 *                   rk@owen.sj.ca.us
 */

/* define TEST to create a stand-alone program to test the routine */
#if 0
#  define TEST
#endif

#include "librko.h"	/* define PRECISION */

#ifdef __cplusplus
extern "C" {
#endif

void ranperm(int low, int high, int *cut,
		PRECISION (*ranfn)(void)) {

	int interval = high - low - 2;
/* get random number */
	*cut = ((PRECISION) interval)*(*ranfn)() + low + 1;
}

#ifdef TEST

#include <stdio.h>

void tstprt(int low, int high, int cut) {
	int flag = 0;
/* test whether out-of-range */
	if (cut <= low || cut >= high-1) flag++;
/* print if passes test or not */
	printf(" %.2d : ", cut);
	if(flag) {
		printf("  FAIL %d\n", flag);
	} else {
		printf("  OK\n");
	}
}

int main() {
	int low = 0, high = 10;
	int cut;
	int i;

	for (i=0; i < 20; ++i) {
		ranperm(low, high, &cut, urand);
		tstprt(low, high, cut);
	}

	return 0;
}

#endif /* TEST */

#ifdef __cplusplus
}
#endif

