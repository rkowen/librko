static const char USMID[]="%W%";
static const char RCSID[]="@(#)$Id: metropolis.c,v 1.1 1998/10/07 16:19:56 rk Exp $";
static const char AUTHOR[]="@(#)metropolis 02/11/93 1.0 R.K.Owen,PhD";
/*
 * metropolis - will perform the Metropolis algorithm to minimize the
 *   "cost" of the ordering of indices in index[].  All detailed
 *   structure of the data is hidden from this function.  It only
 *   uses the index array to communicate to the "cost" function.
 *
 *   flag = Mpls_RAN_INDEX	metropolis will generate a random index array
 *				unless Mpls_FIX_* is set,
 *				else the user must supply a "good" guess.
 *	| Mpls_RAN_SWAP		use swapping of indices.
 *	| Mpls_RAN_PERMUTE	randomly cut "string" of indices and permute.
 *	| Mpls_RAN_REVERSE	randomly cut "string" and reverse substring.
 *	| Mpls_FIX_INITIAL	keep the initial index[0] fixed
 *	| Mpls_FIX_FINAL	keep the final index[indexdim-1] fixed
 *	if flag = Mpls_RAN_* | Mpls_FIX_* are set metropolis only
 *				randomize with respect to the rest of the
 *				indices. The user will have to provide the
 *				fixed indices and populate the rest of the
 *				index array with appropriate values.
 *   maxstep > 0		Stops metropolis from performing more than
 *				maxstep steps.
 *   indexdim, index		defines index[] the array of indices.
 *
 * user supplied functions:
 *   cost(indexdim,index,swapmethod,swap1,swap2)
 *				returns the "cost" of the given configuration
 *				where swap1,swap2 are the indices for the last
 *				two entries in index[] that were swapped,
 *				permuted, or string reversed given by
 *				swapmethod.
 *				This may help cost() be more efficient, but
 *				need not be used. However, metropolis sets
 *				swap1=swap2=-1 for the first call to cost()
 *				and set swapmethod=_NONE.
 *   invtemp(maxstep,current step)	returns the "inverse temperature" for
 *				simulated annealing.  It should be a
 *				monotonically increasing function.
 *   ranfn(void)		a uniformly distributed pseudo-random
 *				generator.  Seed initialization must be
 *				done prior to invoking metropolis.
 *	
 *
 *   metropolis returns number of steps if successful, < 0 otherwise.
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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>		/* for exp( ) */
#include "librko.h"


#ifdef __cplusplus
extern "C" {
#endif

int metropolis(int flag, int maxstep, int indexdim, int *index,
		PRECISION (*cost)(int,int*,swaptype,int,int),
		PRECISION (*invtemp)(int,int),
		PRECISION (*ranfn)(void)) {

	int low = 0, high = indexdim;	/* index[] endpoints */
	int swap1 = -1, swap2 = -1;	/* indices of swapped entries */
	int step = 0;			/* current step */
	int *tindex = 0;		/* tmp index array */
	int swapflag = 0;		/* flag if doing swapping */
	int permflag = 0;		/* flag if doing permuting */
	int revrflag = 0;		/* flag if doing reversing */
	swaptype toggle[3];		/* toggle between swap, perm, & revr */
	int togbase = 0;		/* toggle base */
	int togindx = 0;		/* index to toggle */
	PRECISION tcostnew,tcostold;	/* current "cost" values */
	PRECISION tinvtemp;		/* current "inverse temperature" */
	PRECISION prob;			/* probability value to accept move */
	int i;

/* set special internal flags */
	if (maxstep <= 0) {
		fprintf(stderr,
			"metropolis: must have maxstep > 0\n");
		return -1;
	}
	if (flag & Mpls_FIX_INITIAL) low++;
	if (flag & Mpls_FIX_FINAL) high--;
	if (flag & Mpls_RAN_SWAP) {
		swapflag++;
		toggle[togbase++] = _SWAP;
	}
	if (flag & Mpls_RAN_PERMUTE) {
		permflag++;
		toggle[togbase++] = _PERM;
	}
	if (flag & Mpls_RAN_REVERSE) {
		revrflag++;
		toggle[togbase++] = _REVR;
	}
	if (!swapflag && !permflag && !revrflag) {
		fprintf(stderr,
		"metropolis: must specify swap, reverse and/or permute\n");
		return -1;
	}
/* randomize the index array */
	if (flag & Mpls_RAN_INDEX) {
/* populate index array if no fixed ends */
		if (!(flag & Mpls_FIX_INITIAL) || !(flag & Mpls_FIX_FINAL)) {
			for (i = 0; i < indexdim; ++i) index[i] = i;
		}
		if (ranindex(low,high,indexdim,index,ranfn)) {
			fprintf(stderr,
				"metropolis: failed in ranindex at line %d\n",
				__LINE__);
			return -2;
		}
	}
/* set up tmp index array if permuting */
	if (permflag) {
		if (!(tindex = (int *) malloc(indexdim*sizeof(int)))) {
			fprintf(stderr,
				"metropolis: failed to malloc at line %d\n",
				__LINE__);
			return -3;
		}
	}
/* minimize cost via the Metropolis algorithm */
	tcostold = (*cost)(indexdim,index,_NONE,swap1,swap2);
	do {
/* propose a move */
		switch (toggle[togindx]) {
		case _SWAP:
			ranswap(low,high,&swap1,&swap2,ranfn);
			idxswap(index,swap1,swap2);
			break;
		case _PERM:
			ranperm(low,high,&swap1,ranfn);
			idxperm(low,high,index,tindex,swap1);
			swap2 = swap1;
			break;
		case _REVR:
			ranswap(low,high,&swap1,&swap2,ranfn);
			idxrevr(index,swap1,swap2);
			break;
		}
		tcostnew = (*cost)(indexdim,index,toggle[togindx],swap1,swap2);
/* is new cost lower? then accept move else accept with some probability */
		if (tcostnew > tcostold) {
			tinvtemp = (*invtemp)(maxstep,step);
			prob = (PRECISION) 
				mpls_exp(tinvtemp*(tcostold - tcostnew));
			if ((*ranfn)() >= prob) {	/* don't accept */
				switch (toggle[togindx]) {
				case _SWAP:
					idxswap(index,swap1,swap2);
					break;
				case _PERM:
					idxperm(low,high,index,tindex,
						high - swap1 + low);
					break;
				case _REVR:
					idxrevr(index,swap1,swap2);
					break;
				}
			} else {		/* accept */
				tcostold = tcostnew;
			}
		} else {			/* accept */
			tcostold = tcostnew;
		}
/* just did a step */
		togindx++;
		togindx %= togbase;
		++step;

	} while (step < maxstep);

/* free memory if necessary */
	if (permflag) free(tindex);

	return step;
}

#ifdef TEST

/*
A traveling salesman problem with the following points

+------------------------------+
|        2             9       |
|                              |
|             1   0            |
|  3            +            8 |
|             5   6            |
|                              |
|        4             7       |
+------------------------------+

and should end up with cyclic strings (possible one will be reversed):
 1 2 3 4 5   +   6 7 8 9 0
*/

struct Point {
	PRECISION x,y;
};

typedef struct Point point;

/* global data */
point data[10] = {
	{ 1., 1.},{-1., 1.},{-4., 4.},{-8., 0.},{-4.,-4.},
	{-1.,-1.},{ 1.,-1.},{ 4.,-4.},{ 8., 0.},{ 4., 4.}};
PRECISION distance[10][10];

PRECISION cost(int indexdim, int *index,
			swaptype swapmethod, int swap1, int swap2) {
/* will ignore swap1,2 and swapmethod */
	int i,j;
	PRECISION sum = 0.;
/* first time through set up distances array */
	static int initial = 0;
	if (!initial) {
		initial = 1;
		for (i = 0; i < 10; ++i) {
			for (j = 0; j < 10; ++j) {
				distance[i][j] = hypot(
					data[i].x - data[j].x,
					data[i].y - data[j].y);
			}
		}
	}
/* compute total distance of circuit */
	for (i = 1; i < 10; ++i) {
		sum += distance[index[i]][index[i-1]];
	}
	sum += distance[index[0]][index[9]];

	return sum;
}

/* global variable */
int ibatch;

PRECISION invtemp(int maxstep, int step) {
	return .1*((PRECISION) (ibatch + 1));
}

void tstprt(int batch, int indexdim, int *index) {
	int i;
	PRECISION imindist;
	PRECISION mcost;
/* minimum distance = 4+28sqrt(2) */
	imindist = 1./(4.+28.*sqrt(2));
	printf("%.4d :", batch);
/* print out circuit */
	for (i = 0; i < 10; ++i) {
		printf(" %.2d",index[i]);
	}
	mcost = cost(indexdim,index,_NONE,-1,-1);
	printf(" : %10.4f : %10.4f \n", mcost, mcost*imindist);
	fflush(stdout);
}

int main() {
	int batch = 20;
	int maxstep = 2000;
	int indexdim = 10;
	int index[10];
	int steps = 0;
	int flag;
	int i;

	printf("\nno fixed ends\n");
	printf("swap\n");
	flag = Mpls_RAN_INDEX | Mpls_RAN_SWAP;
	for (ibatch = 0; ibatch < batch; ++ibatch) {
		steps = metropolis(flag, maxstep, indexdim, index,
			cost, invtemp, urand);
		printf("%.5d :",steps);
		tstprt(ibatch, indexdim, index);
		flag &= ~Mpls_RAN_INDEX;
	}

	printf("\nfixed ends\n");
	printf("swap\n");
	flag = Mpls_FIX_INITIAL | Mpls_FIX_FINAL |
		Mpls_RAN_INDEX | Mpls_RAN_SWAP;
	index[0] = 1;
	index[indexdim-1] = 0;
	for (i = 1; i < indexdim - 1; ++i) index[i]= i+1;
	for (ibatch = 0; ibatch < batch; ++ibatch) {
		steps = metropolis(flag, maxstep, indexdim, index,
			cost, invtemp, urand);
		printf("%.5d :",steps);
		tstprt(ibatch, indexdim, index);
		flag &= ~Mpls_RAN_INDEX;
	}

	printf("\nfixed ends\n");
	printf("reverse\n");
	flag = Mpls_FIX_INITIAL | Mpls_FIX_FINAL |
		Mpls_RAN_INDEX | Mpls_RAN_PERMUTE | Mpls_RAN_REVERSE;
	index[0] = 1;
	index[indexdim-1] = 0;
	for (i = 1; i < indexdim - 1; ++i) index[i]= i+1;
	for (ibatch = 0; ibatch < batch; ++ibatch) {
		steps = metropolis(flag, maxstep, indexdim, index,
			cost, invtemp, urand);
		printf("%.5d :",steps);
		tstprt(ibatch, indexdim, index);
		flag &= ~Mpls_RAN_INDEX;
	}

	printf("\nfixed ends\n");
	printf("permute & reverse\n");
	flag = Mpls_FIX_INITIAL | Mpls_FIX_FINAL |
		Mpls_RAN_INDEX | Mpls_RAN_PERMUTE | Mpls_RAN_REVERSE;
	index[0] = 1;
	index[indexdim-1] = 0;
	for (i = 1; i < indexdim - 1; ++i) index[i]= i+1;
	for (ibatch = 0; ibatch < batch; ++ibatch) {
		steps = metropolis(flag, maxstep, indexdim, index,
			cost, invtemp, urand);
		printf("%.5d :",steps);
		tstprt(ibatch, indexdim, index);
		flag &= ~Mpls_RAN_INDEX;
	}

	printf("\nfixed ends\n");
	printf("swap & permute\n");
	flag = Mpls_FIX_INITIAL | Mpls_FIX_FINAL |
		Mpls_RAN_INDEX | Mpls_RAN_PERMUTE | Mpls_RAN_SWAP;
	index[0] = 1;
	index[indexdim-1] = 0;
	for (i = 1; i < indexdim - 1; ++i) index[i]= i+1;
	for (ibatch = 0; ibatch < batch; ++ibatch) {
		steps = metropolis(flag, maxstep, indexdim, index,
			cost, invtemp, urand);
		printf("%.5d :",steps);
		tstprt(ibatch, indexdim, index);
		flag &= ~Mpls_RAN_INDEX;
	}

	printf("\nfixed ends\n");
	printf("swap & permute & reverse\n");
	flag = Mpls_FIX_INITIAL | Mpls_FIX_FINAL | Mpls_RAN_INDEX |
		Mpls_RAN_PERMUTE | Mpls_RAN_SWAP | Mpls_RAN_REVERSE;
	index[0] = 1;
	index[indexdim-1] = 0;
	for (i = 1; i < indexdim - 1; ++i) index[i]= i+1;
	for (ibatch = 0; ibatch < batch; ++ibatch) {
		steps = metropolis(flag, maxstep, indexdim, index,
			cost, invtemp, urand);
		printf("%.5d :",steps);
		tstprt(ibatch, indexdim, index);
		flag &= ~Mpls_RAN_INDEX;
	}

	return 0;
}

#endif /* TEST */

#ifdef __cplusplus
}
#endif

