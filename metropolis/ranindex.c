static const char USMID[]="%W%";
static const char RCSID[]="@(#)$Id: ranindex.c,v 1.1 1998/10/07 16:19:56 rk Exp $";
static const char AUTHOR[]="@(#)ranindex 02/10/93 1.0 R.K.Owen,PhD";
/*
 * ranindex - will randomly rearrange the integer sequence [low,high-1]
 *   in the integer index array.  The calling program must populate the
 *   index array with entries.
 *   The user must also give a pseudo-random number generator that
 *   produces a number in the interval [0.,1.)
 *
 *   ranindex returns 0 if successful, 1 otherwise.
 *
 * author	R.K.Owen,Ph.D.	02/10/93
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

#include <stdlib.h>	/* malloc, free */
#include <stdio.h>	/* fprintf */
#include "librko.h"	/* define PRECISION */

#ifdef __cplusplus
extern "C" {
#endif

int ranindex(int low, int high, int indexdim, int *index,
		PRECISION (*ranfn)(void)) {
	int tindexdim = high - low;
	int *tindex, *iindex;
	int nindex, tran;
	int i,j,k;

/* malloc an another index array */
	if(!(tindex = (int *) malloc(tindexdim*sizeof(int)))) {
		fprintf(stderr,"ranindex error: could not malloc space\n");
		return 1;
	}
	if(!(iindex = (int *) malloc(tindexdim*sizeof(int)))) {
		fprintf(stderr,"ranindex error: could not malloc more space\n");
		free(tindex);
		return 2;
	}
/* populate entries in tmp index array */
	for (i = 0; i < tindexdim; ++i)		tindex[i] = 1;

/* nindex is the number of intervals that are needed */
	nindex = tindexdim;
/* cycle through to generate all the random indices */
	for (j = 0; j < tindexdim; ++j) {
/* get a random integer on [0,nindex) */
		tran = ((int) ((PRECISION) nindex)*(*ranfn)());
/* find index that corresponds to this interval */
		k = 0;
		for (i = 0; i < tindexdim; ++i) {
			if(tindex[i]) {
				if(k == tran) {
					iindex[j] = i+low;
					tindex[i] = 0;
					break;
				}
				++k;
			}
		}
/* number of outstanding intervals */
		--nindex;
	}
/* rearrange index and place into tindex and then copy back again */
	for (i = 0; i < tindexdim; ++i) tindex[i] = index[iindex[i]];
	for (i = 0; i < tindexdim; ++i) index[i+low] = tindex[i];
/* free up memory */
	free(iindex);
	free(tindex);
		
	return 0;
}

#ifdef TEST

void tstprt(int indexdim, int *index) {
	int i,j, flag = 0, swap;
	for (i = 0; i < indexdim; ++i) printf(" %.2d", index[i]);
/* bubble sort */
	for (i = 1; i < indexdim; ++i) {
		for (j = 0; j < i; ++j) {
			if (index[i] < index[j]) {
				swap = index[j];
				index[j] = index[i];
				index[i] = swap;
			}
		}
	}
/* test whether sorted indices are out of order */
	for (i = 0; i < indexdim; ++i) {
		if (i != index[i]) {
			flag = 1;
			break;
		}
	}
/* print if passes test or not */
	if(flag) {
		printf("  FAIL\n\n");
	} else {
		printf("  OK\n\n");
	}
/* set up for next test */
	for (i = 0; i < indexdim; ++i) index[i] = i;
}

int main() {
	const int indexdim=10;
	int index[10];
	int low = 0, high = indexdim;
	int i;

	for (i = 0; i < indexdim; ++i) index[i] = i;
	if(ranindex(low, high, indexdim, index, urand)) {
		fprintf(stderr,"error return from ranindex\n");
	} else {
		tstprt(indexdim, index);
	}

	ranindex(2, high, indexdim, index, urand);
	tstprt(indexdim, index);

	ranindex(low, 8, indexdim, index, urand);
	tstprt(indexdim, index);

	ranindex(2, 8, indexdim, index, urand);
	tstprt(indexdim, index);

	return 0;
}

#endif /* TEST */

#ifdef __cplusplus
}
#endif

