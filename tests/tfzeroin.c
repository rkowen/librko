#include <stdio.h>
#include <math.h>
#include "librko.h"

PRECISION tfun(PRECISION x) {
	PRECISION t = (x - 2.0);
	return t*t*t;
}

#define hpi 1.57079632679490

int main(void) {
	PRECISION x, ax, Ax, dx, scale;
	int i;
	scale = 0.1;

	printf("Finding simple \"zero\" of sin(x)-y0\n");
	for (i = -10; i <= 10; i++) {
		x = scale * i;
		if (i ==  10) x =  .999;
		if (i == -10) x = -.999;
		ax = (PRECISION) asin(x);
		Ax = (PRECISION) fzeroin(-hpi,hpi,x,sin,0.0);
		dx = ax - Ax;
		printf("% 5.3f % 15.8f % 15.8f % 15.8e\n", x, ax, Ax, dx);
		fflush(stdout);
	}
	printf("Finding cubic \"zero\" of (x-2)^3\n");
	ax = 2.0;
	for (i = 3; i <= 9; i+=2) {
		x = i;
		Ax = (PRECISION) fzeroin(0.0,x,0.0,tfun,0.0);
		dx = ax - Ax;
		printf("% 5.3f % 15.8f % 15.8f % 15.8e\n", x, ax, Ax, dx);
		fflush(stdout);
	}
	return 0;
}
