#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "urand.h"

/*
 * #define OPEQ to compare	+=	and	^=
 * 			else	+	and	^
 *
 * #define DOPT to print out	-DADD if "+m2" as fast as "^m"
 *			else	-DNOADD
 * if OPEQ			-DOPADD if "+=m2" as fast as "^=m"
 *			else	-DNOOPADD
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
	INTEGER mm2 = 0, mm = 1, notmm;

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
	for (j = 1; j < MAXTRY; ++j) {
/* initialize arrays */
		for (i=0; i<MAXDIM; ++i) {
			iarr1[i] = iarr2[i] = irand();
		}
/* + */
		t0 = clock();
		for (i=0; i<MAXDIM; ++i) {
#ifdef OPEQ
			iarr1[i] += mm2;
			iarr1[i] += mm2;
#else
			iarr1[i] = (iarr1[i] + mm2) + mm2;
#endif
		}
/* ^ */
		t1[j] = clock();
		for (i=0; i<MAXDIM; ++i) {
#ifdef OPEQ
			iarr2[i] ^= mm;
#else
			iarr2[i] = (iarr2[i] ^ mm);
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
				printf("%d,%d: %x %x : %d %d\n",j,i,
					iarr1[i],iarr2[i], iarr1[i],iarr2[i]);
#endif
			}
		}
#ifndef DOPT
		if (sizeof(clock_t) == sizeof(int)) {
			printf("+m2 clocks: %d\t\t", t1[j]);
			printf("^m  clocks: %d\n", t2[j]);
		} else if (sizeof(clock_t) == sizeof(long)) {
			printf("+m2 clocks: %ld\t\t", t1[j]);
			printf("^m  clocks: %ld\n", t2[j]);
		} else if (sizeof(clock_t) == sizeof(short)) {
			printf("+m2 clocks: %hd\t\t", t1[j]);
			printf("^m  clocks: %hd\n", t2[j]);
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
		printf("+m2 clocks: %d\t\t", t1m);
		printf("^m  clocks: %d\n", t2m);
	} else if (sizeof(clock_t) == sizeof(long)) {
		printf("+m2 clocks: %ld\t\t", t1m);
		printf("^m  clocks: %ld\n", t2m);
	} else if (sizeof(clock_t) == sizeof(short)) {
		printf("+m2 clocks: %hd\t\t", t1m);
		printf("^m  clocks: %hd\n", t2m);
	}
#else
#  ifdef OPEQ
	if (iflag || t2m >= t1m) printf(" -DOPADD");
	else printf(" -DNOOPADD");
#  else
	if (iflag || t2m >= t1m) printf(" -DADD");
	else printf(" -DNOADD");
#  endif
#endif

	return 0;
}
