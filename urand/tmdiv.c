#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "urand.h"

/*
 * #define OPEQ to compare	/=	and	>>=
 * 			else	/	and	>>
 *
 * #define DOPT to print out	-DDIV if "/2" as fast as ">> 1"
 *			else	-DNODIV
 * if OPEQ			-DOPDIV if "/=2" as fast as ">>=1"
 *			else	-DNOOPDIV
 */

#ifndef MAXTRY
#  define MAXTRY 11
#endif

#ifndef MAXDIM
#  define MAXDIM 100000
#endif

int tcmp(const void *t1, const void *t2) {
	if (*(time_t *)t1 > *(time_t *)t2) return 1;
	else if (*(time_t *)t1 < *(time_t *)t2) return -1;
	else return 0;
}

int main() {
	int i, j, iflag = 0;
	int iarr1[MAXDIM], iarr2[MAXDIM];
	INTEGER myseed = sizeof(INTEGER);
	clock_t t0,t1[MAXTRY],t2[MAXTRY], t1m, t2m;

/* initialize seed */
	setseed(myseed);
	for (j = 1; j < MAXTRY; ++j) {
/* initialize arrays */
		for (i=0; i<MAXDIM; ++i) {
			iarr1[i] = iarr2[i] = irand();
		}
/* >> */
		t0 = clock();
		for (i=0; i<MAXDIM; ++i) {
#ifdef OPEQ
			iarr1[i] >>= 1;
#else
			iarr1[i] = (iarr1[i] >> 1);
#endif
		}
/* /2 */
		t1[j] = clock();
		for (i=0; i<MAXDIM; ++i) {
#ifdef OPEQ
			iarr2[i] /= 2;
#else
			iarr2[i] = (iarr2[i] / 2);
#endif
		}
		t2[j] = clock();
		t2[j] -= t1[j];
		t1[j] -= t0;
/* display results */
		for (i=0; i<MAXDIM; ++i) {
			if (iarr1[i] != iarr2[i]) {
				iflag++;
#ifndef DOPT
				printf("%d,%d: %d %d\n",j,i,iarr1[i],iarr2[i]);
#endif
			}
		}
#ifndef DOPT
		if (sizeof(clock_t) == sizeof(int)) {
			printf(">>1 clocks: %d\t\t", t1[j]);
			printf("/2  clocks: %d\n", t2[j]);
		} else if (sizeof(clock_t) == sizeof(long)) {
			printf(">>1 clocks: %ld\t\t", t1[j]);
			printf("/2  clocks: %ld\n", t2[j]);
		} else if (sizeof(clock_t) == sizeof(short)) {
			printf(">>1 clocks: %hd\t\t", t1[j]);
			printf("/2  clocks: %hd\n", t2[j]);
		}
#endif
	}
/* find median value */
	qsort(t1, MAXTRY, sizeof(clock_t), tcmp);
	qsort(t2, MAXTRY, sizeof(clock_t), tcmp);
	t1m = t1[MAXTRY/2];
	t2m = t2[MAXTRY/2];

#ifndef DOPT
        printf("Median value --\n");
	if (sizeof(clock_t) == sizeof(int)) {
		printf(">>1 clocks: %d\t\t", t1m);
		printf("/2  clocks: %d\n", t2m);
	} else if (sizeof(clock_t) == sizeof(long)) {
		printf(">>1 clocks: %ld\t\t", t1m);
		printf("/2  clocks: %ld\n", t2m);
	} else if (sizeof(clock_t) == sizeof(short)) {
		printf(">>1 clocks: %hd\t\t", t1m);
		printf("/2  clocks: %hd\n", t2m);
	}
#else
#  ifdef OPEQ
	if (iflag || t2m >= t1m) printf(" -DOPDIV");
	else printf(" -DNOOPDIV");
#  else
	if (iflag || t2m >= t1m) printf(" -DDIV");
	else printf(" -DNODIV");
#  endif
#endif

	return 0;
}
