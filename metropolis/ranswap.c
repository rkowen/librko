static const char USMID[]="%W%";
static const char RCSID[]="@(#)$Id: ranswap.c,v 1.1 1998/10/07 16:19:56 rk Exp $";
static const char AUTHOR[]="@(#)ranswap 02/11/93 1.0 R.K.Owen,PhD";
/*
 * ranswap - will return two random integer on the interval [low,high-1]
 *   in the variables swap1 and swap2. (swap1 < swap2)
 *   The user must also give a pseudo-random number generator that
 *   produces a number in the interval [0.,1.)
 *
 * author	R.K.Owen,Ph.D.	02/11/93
 */
/* 
 *     This software is copyrighted by R.K.Owen,Ph.D. 1996
 * 
 * The author, R.K.Owen, of this software is not liable for any
 * problems WHATSOEVER which may result from use  or  abuse  of
 * this  software.  The author, R.K.Owen, grants limited rights
 * to anyone who uses and modifies this  software  for  private
 * non-commercial  use  as  long  as  this copyright and rights
 * notice remains in this software and is made available to all
 * recipients of this software.
 * 
 * Anyone  who  includes this software in a commercial package,
 * or intends  to,  must  contact  the  author,  R.K.Owen,  and
 * license this software.
 *
 * last known email: rkowen@kudonet.com
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

void ranswap(int low, int high, int *swap1, int *swap2,
		PRECISION (*ranfn)(void)) {

	int interval = high - low;
	int swap;
/* get first random number */
	*swap1 = ((PRECISION) interval)*(*ranfn)() + low;
/* get second random number */
	--interval;
	*swap2 = ((PRECISION) interval)*(*ranfn)() + low;
	if (*swap2 >= *swap1) (*swap2)++;
/* order the pair */
	if (*swap1 > *swap2) {
		swap = *swap1;
		*swap1 = *swap2;
		*swap2 = swap;
	}
}

#ifdef TEST

#include <stdio.h>

void tstprt(int low, int high, int swap1, int swap2) {
	int flag = 0;
/* test whether swapped numbers are = or out-of-range */
	if (swap1 >= swap2) flag++;
	if (swap1 < low || swap1 >= high) flag++;
	if (swap2 < low || swap2 >= high) flag++;
/* print if passes test or not */
	printf(" %.2d %.2d : ", swap1, swap2);
	if(flag) {
		printf("  FAIL %d\n", flag);
	} else {
		printf("  OK\n");
	}
}

int main() {
	int low = 0, high = 10;
	int swap1, swap2;
	int i;

	for (i=0; i < 20; ++i) {
		ranswap(low, high, &swap1, &swap2, urand);
		tstprt(low, high, swap1, swap2);
	}

	return 0;
}

#endif /* TEST */

#ifdef __cplusplus
}
#endif

