static const char USMID[]="%W%";
static const char RCSID[]="@(#)$Id: idxrevr.c,v 1.2 1998/10/14 15:16:10 rk Exp $";
static const char AUTHOR[]="@(#)idxrevr 02/15/93 1.0 R.K.Owen,PhD";
/*
 * idxrevr - reverses the given sequences in index[].
 *   The sequence order index[low:cut1-1 cut1:cut2 cut2+1:high-1] becomes
 *   The sequence order index[low:cut1-1 cut2:cut1 cut2+1:high-1] becomes
 *   The user must ensure that cut is within range.
 *
 * author	R.K.Owen,Ph.D.	02/15/93
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
 * last known email: rkowen@ckns.net
 *                   rk@owen.sj.ca.us
 */

/* define TEST to create a stand-alone program to test the routine */
#if 0
#  define TEST
#endif

#ifdef __cplusplus
extern "C" {
#endif

void idxrevr(int *index, int cut1, int cut2) {
	int i;
	int swap;
	for (i = 0; i < (cut1 + cut2 + 1)/2 - cut1; ++i) {
		swap = index[i + cut1];
		index[i + cut1] = index[cut2 - i];
		index[cut2 - i] = swap;
	}
}

#ifdef TEST

#include <stdio.h>
#include "librko.h"

void tstprt(int low, int high, int indexdim, int *index, int cut1, int cut2) {
	int i;
	int flag = 0;
/* print */
	for (i = 0; i < indexdim; ++i) printf(" %.2d", index[i]);
/* reverse back - should be in sequence */
	idxrevr(index, cut1, cut2);
/* test whether sorted indices are out of order */
	for (i = 0; i < indexdim; ++i) {
		if (i != index[i]) {
			flag = 1;
			break;
		}
	}
	printf(" : %.2d %.2d : %.2d %.2d: ", low, high, cut1, cut2);
	if (flag) printf("FAIL\n");
	else printf("OK\n");
/* set up index for next shot */
	for (i = 0; i < indexdim; ++i) index[i] = i;
}

int main() {
	int indexdim = 10;
	int index[10];
	int i;
	int trials = 3;
	int low = 0;
	int high = indexdim;
	int cut1, cut2;
/* initialize */
	for (i = 0; i < indexdim; ++i) {
		index[i] = i;
		printf(" %.2d", index[i]);
	}
	printf("\n");

	for (i = 0; i < trials; ++i) {
		low = 0; high = indexdim;
		ranswap(low, high, &cut1, &cut2, urand);
		idxrevr(index, cut1, cut2);
		tstprt(low, high, indexdim, index, cut1, cut2);

		low = 2; high = indexdim;
		ranswap(low, high, &cut1, &cut2, urand);
		idxrevr(index, cut1, cut2);
		tstprt(low, high, indexdim, index, cut1, cut2);

		low = 1; high = indexdim;
		ranswap(low, high, &cut1, &cut2, urand);
		idxrevr(index, cut1, cut2);
		tstprt(low, high, indexdim, index, cut1, cut2);

		low = 0; high = 8;
		ranswap(low, high, &cut1, &cut2, urand);
		idxrevr(index, cut1, cut2);
		tstprt(low, high, indexdim, index, cut1, cut2);

		low = 0; high = 7;
		ranswap(low, high, &cut1, &cut2, urand);
		idxrevr(index, cut1, cut2);
		tstprt(low, high, indexdim, index, cut1, cut2);

		low = 1; high = 7;
		ranswap(low, high, &cut1, &cut2, urand);
		idxrevr(index, cut1, cut2);
		tstprt(low, high, indexdim, index, cut1, cut2);

		low = 2; high = 7;
		ranswap(low, high, &cut1, &cut2, urand);
		idxrevr(index, cut1, cut2);
		tstprt(low, high, indexdim, index, cut1, cut2);

		low = 2; high = 8;
		ranswap(low, high, &cut1, &cut2, urand);
		idxrevr(index, cut1, cut2);
		tstprt(low, high, indexdim, index, cut1, cut2);

		low = 3; high = 8;
		ranswap(low, high, &cut1, &cut2, urand);
		idxrevr(index, cut1, cut2);
		tstprt(low, high, indexdim, index, cut1, cut2);
	}

	return 0;
}

#endif /* TEST */

#ifdef __cplusplus
}
#endif

