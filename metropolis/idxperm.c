static const char USMID[]="%W%";
static const char RCSID[]="@(#)$Id: idxperm.c,v 1.2 1998/10/14 15:16:10 rk Exp $";
static const char AUTHOR[]="@(#)idxperm 02/15/93 1.0 R.K.Owen,PhD";
/*
 * idxperm - permutes the sequences in index[].
 *   The sequence order index[low:cut-1 cut:high-1] becomes
 *   the sequence order index[cut:high-1 low:cut-1]
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

void idxperm(int low, int high, int *index, int *work, int cut) {
	int i;
	if (cut > low && cut < high) {
		for (i = low; i < cut; ++i) {
			work[high - cut + i] = index[i];
		}
		for (i = cut; i < high; ++i) {
			work[i - cut + low] = index[i];
		}
		for (i = low; i < high; ++i) index[i] = work[i];
	}
}

#ifdef TEST

#include <stdio.h>

void tstprt(int low, int high, int indexdim, int *index, int *trial, int cut) {
	int i, j, swap;
	int flag = 0;
/* print */
	for (i = 0; i < indexdim; ++i) printf(" %.2d", index[i]);
/* permute back - should be in sequence */
	idxperm(low, high, index, trial, high - cut + low);
/* test whether sorted indices are out of order */
	for (i = 0; i < indexdim; ++i) {
		if (i != index[i]) {
			flag = 1;
			break;
		}
	}
	printf(" : %.2d %.2d : %.2d : ", low, high, cut);
	if (flag) printf("FAIL\n");
	else printf("OK\n");
/* set up index for next shot */
	for (i = 0; i < indexdim; ++i) index[i] = i;
}

int main() {
	int indexdim = 10;
	int index[10], trial[10];
	int i;
	int low = 0;
	int high = indexdim;
	int flag = 0;
/* initialize */
	for (i = 0; i < indexdim; ++i) {
		index[i] = i;
		printf(" %.2d", index[i]);
	}
	printf("\n");

	idxperm(low, high, index, trial, 3);
	tstprt(low, high, indexdim, index, trial, 3);
	idxperm(low, high, index, trial, 8);
	tstprt(low, high, indexdim, index, trial, 8);
	idxperm(low, high, index, trial, low);
	tstprt(low, high, indexdim, index, trial, low);
	idxperm(low, high, index, trial, high);
	tstprt(low, high, indexdim, index, trial, high);
	idxperm(2, high, index, trial, 3);
	tstprt(2, high, indexdim, index, trial, 3);
	idxperm(2, high, index, trial, 8);
	tstprt(2, high, indexdim, index, trial, 8);
	idxperm(2, high, index, trial, 2);
	tstprt(2, high, indexdim, index, trial, 2);
	idxperm(2, high, index, trial, high);
	tstprt(2, high, indexdim, index, trial, high);
	idxperm(low, 8, index, trial, 3);
	tstprt(low, 8, indexdim, index, trial, 3);
	idxperm(low, 8, index, trial, 6);
	tstprt(low, 8, indexdim, index, trial, 6);
	idxperm(low, 8, index, trial, low);
	tstprt(low, 8, indexdim, index, trial, low);
	idxperm(low, 8, index, trial, 8);
	tstprt(low, 8, indexdim, index, trial, 8);
	idxperm(1, 8, index, trial, 3);
	tstprt(1, 8, indexdim, index, trial, 3);
	idxperm(1, 8, index, trial, 6);
	tstprt(1, 8, indexdim, index, trial, 6);
	idxperm(1, 8, index, trial, 1);
	tstprt(1, 8, indexdim, index, trial, 1);
	idxperm(1, 8, index, trial, 8);
	tstprt(1, 8, indexdim, index, trial, 8);

	return 0;
}

#endif /* TEST */

#ifdef __cplusplus
}
#endif

