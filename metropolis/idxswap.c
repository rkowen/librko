static const char USMID[]="%W%";
static const char RCSID[]="@(#)$Id: idxswap.c,v 1.2 1998/10/14 15:16:10 rk Exp $";
static const char AUTHOR[]="@(#)idxswap 02/15/93 1.0 R.K.Owen,PhD";
/*
 * idxswap - swaps the elements in index[] pointed to by swap1 & swap2.
 *   The user must ensure that swap1 & swap2 are within ranges.
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

void idxswap(int *index, int swap1, int swap2) {
	int swap;
	swap = index[swap1];
	index[swap1] = index[swap2];
	index[swap2] = swap;
}

#ifdef TEST

#include <stdio.h>

int main() {
	int indexdim = 10;
	int index[10];
	int i;
	int flag = 0;
/* initialize */
	for (i = 0; i < indexdim; ++i) {
		index[i] = i;
		printf(" %.2d", index[i]);
	}
	printf("\n");
/* invert order */
	for (i = 0; i < indexdim/2; ++i) {
		idxswap(index, i, indexdim - 1 - i);
	}
	for (i = 0; i < indexdim; ++i) {
		printf(" %.2d", index[i]);
	}
	printf("\n");
/* check */
	for (i = 0; i < indexdim; ++i) {
		if (index[i] != indexdim - 1 - i) flag ++;
	}
	if (flag) printf("FAIL");
	else printf("OK  ");
	printf(" : idxswap() has inverted index\n");

	return 0;
}

#endif /* TEST */

#ifdef __cplusplus
}
#endif

