#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "librko.h"

#include "urand_.c"	/* includes common definitions & code too */

/*
 * #define OPEQ to compare	/=	and	>>=
 * 			else	/	and	>>
 *
 * #define DOPT to print out	-DDIV if "/2" as fast as ">> 1"
 *			else	-DNODIV
 * if OPEQ			-DOPDIV if "/=2" as fast as ">>=1"
 *			else	-DNOOPDIV
 */

int main() {
	int i, j, iflag = 0;
	int iarr1[MAXDIM], iarr2[MAXDIM];
	INTEGER myseed = sizeof(INTEGER);
	clocker_t t0;
	double t1[MAXTRY],t2[MAXTRY], t1m, t2m, t1a, t2a;
	INTEGER mm2 = 0, mm = 1, notmm;
#ifdef OPEQ
	char opeq[] = "=";
#else
	char opeq[] = "";
#endif

/*  COMPUTE MACHINE INTEGER WORD LENGTH */
	do {
		mm2 = mm;
		mm = 2 * mm2;
	} while (mm > mm2);
	notmm = ~mm;
#ifndef DOPT
	printf("m2    = %x,%d\n", mm2,mm2);
	printf("m     = %x,%d\n", mm,mm);
	printf("notm  = %x,%d\n", notmm,notmm);
#endif

/* initialize seed */
	setseed(myseed);
	for (j = 0; j < MAXTRY; ++j) {
/* initialize arrays */
		for (i=0; i<MAXDIM; ++i) {
			iarr1[i] = iarr2[i] = irand();
		}
/* >> */
		(void) clocker(&t0, _SET);
		for (i=0; i<MAXDIM; ++i) {
#ifdef OPEQ
			iarr1[i] >>= 1;
#else
			iarr1[i] = (iarr1[i] >> 1);
#endif
		}
/* /2 */
		t1[j] = clocker(&t0, _RESET);
		for (i=0; i<MAXDIM; ++i) {
#ifdef OPEQ
			iarr2[i] /= 2;
#else
			iarr2[i] = (iarr2[i] / 2);
#endif
		}
		t2[j] = clocker(&t0, _RESET);
/* display results */
		for (i=0; i<MAXDIM; ++i) {
			if (iarr1[i] != iarr2[i]) {
				iflag++;
#ifndef DOPT
				printf("%d,%d: %x %x : %d %d\n",j+1,i,
					iarr1[i],iarr2[i], iarr1[i],iarr2[i]);
#endif
			}
		}
#ifndef DOPT
		printf(">>%s1 clocks: %8.4f\t\t", opeq, t1[j]);
		printf("/%s2  clocks: %8.4f\n",   opeq, t2[j]);
#endif
	}
/* find median/average values */
	qsort(t1, MAXTRY, sizeof(double), tcmp);
	qsort(t2, MAXTRY, sizeof(double), tcmp);
	t1m = t1[MAXTRY/2];
	t2m = t2[MAXTRY/2];
	t1a = tave(t1, MAXTRY);
	t2a = tave(t2, MAXTRY);

#ifndef DOPT
        printf("Median value --\n");
	printf(">>%s1 clocks: %8.4f\t\t", opeq, t1m);
	printf("/%s2  clocks: %8.4f\n", opeq, t2m);
        printf("Average value --\n");
	printf(">>%s1 clocks: %8.4f\t\t", opeq, t1a);
	printf("/%s2  clocks: %8.4f\n", opeq, t2a);
	printf("\nshould give the option:");
#endif
#ifdef OPEQ
	if (iflag || t2a <= t1a) printf(" -DOPDIV");
	else printf(" -DNOOPDIV");
#else
	if (iflag || t2a <= t1a) printf(" -DDIV");
	else printf(" -DNODIV");
#endif

#ifndef DOPT
	printf("\n\n");
#endif

	return 0;
}
