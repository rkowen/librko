/* tclocker - just a simple test for the clocker function */

#include "clocker.h"

#define ABS(a) ((a) < 0.0 ? -(a) : (a))

#define MAXLOOP 2000
#define FMT		"7.3"

int looptest() {
	int i, j, x;
	for (i = 0; i < MAXLOOP; ++i)
		for (j = 0; j < MAXLOOP; ++j) x = i - j;
	return x - 1;
}

#include <stdio.h>

int main(int argc, char *argv[]) {

	double t1, t2, t3, small;
	clocker_t cc1, cc2, tick;

/* make sure things run */
	t1 = clocker(&cc1, _SET);
	if ( t1 != 0.0 ) {
		printf("FAIL: clocker() not valid on this platform "
			"- %%" FMT "f\n");
		return 1;
	}
	(void) looptest();
	t2 = clocker(&cc1, _READ);
	tick = clocker_tick(&cc1, _READ);
	printf("OK  : clocker() time = %" FMT "f\n", t2);
	printf("OK  : clocker_tick() time = %ld\n", (long) tick);
	tick = clocker_tick(&cc1, _PER_SEC);
	printf("OK  : clocker() ticks per sec = %" FMT "g\n",
		(double) tick);
/* do null test */
	(void) clocker(&cc1, _SET);
	small =  clocker(&cc1, _RESET);
	if (small > t1 / MAXLOOP) {
		printf("FAIL: clocker() null test = %" FMT "g\n", small);
	} else {
		printf("OK  : clocker() null test = %" FMT "g\n", small);
	}

/* sum test */
	(void) clocker(&cc1, _SET);
	(void) clocker(&cc2, _SET);
	(void) looptest();
	t1 = clocker(&cc1, _RESET);
	(void) looptest();
	t2 = clocker(&cc1, _READ);
	t3 = clocker(&cc2, _READ);
	printf("OK  : clocker() time = %" FMT "f\n", t1);
	printf("OK  : clocker() time = %" FMT "f\n", t2);
	printf("OK  : clocker() time = %" FMT "f\n", t3);
	if (ABS(t3 - t2 - t1) <= 10*small) {
		printf("OK  : clocker() sum\n");
	} else {
		printf("FAIL: clocker() sum\n");
	}

	return 0;
}
