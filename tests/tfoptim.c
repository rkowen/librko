#include <stdio.h>
#include <math.h>
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#include "librko.h"

#define hpi 1.57079632679490

double offset = 0.;
double maxmin = 1.0;

double tfun(double x) {
	return (maxmin * cos(hpi*(x - offset)));
}

static PRECISION _F_a_D_d_(PRECISION ad1, PRECISION ad2) {
	volatile /* static */ PRECISION tmp;
	tmp = (ad1 + ad2);	/* force the value out of the registers */
	return tmp;
}

void main(void) {
	PRECISION x, Ax, dx, scale, eps, tmp;
	const PRECISION half = 0.5, one = 1.0;
	int i;
	scale = 0.1;

/* compute machine eps */
	tmp = half;
	while (_F_a_D_d_(one,tmp) != one) {
		eps = tmp;
		tmp *= half;
	}
	printf("     Eps  = % 15.8e\n", eps);
	eps = sqrt(eps);
	printf("sqrt(Eps) = % 15.8e\n\n", eps);

/* find maximum value */
	printf("Finding maximum value\n");
	for (i = -10; i <= 10; i++) {
		offset = x = scale * i;
		Ax = (PRECISION) foptim(1,-1.0,1.0,tfun,0.0);
		dx = x - Ax;
		printf("% 5.3f % 15.8f % 15.8e\n", x, Ax, dx);
	}
/* find minimum value */
	printf("Finding minimum value\n");
	maxmin = -1.0;
	for (i = -10; i <= 10; i++) {
		offset = x = scale * i;
		Ax = (PRECISION) foptim(-1,-1.0,1.0,tfun,0.0);
		dx = x - Ax;
		printf("% 5.3f % 15.8f % 15.8e\n", x, Ax, dx);
	}
	return;
}
