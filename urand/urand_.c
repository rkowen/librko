#ifndef _URAND__H
#  define _URAND__H

#  ifndef MAXTRY
#    define MAXTRY 10
#  endif

#  ifndef MAXDIM
#    define MAXDIM 100000
#  endif

int tcmp(const void *t1, const void *t2) {
	if (*(double *)t1 > *(double *)t2) return 1;
	else if (*(double *)t1 < *(double *)t2) return -1;
	else return 0;
}

double tave(double t[], size_t maxt) {
	double sum = 0.0;
	size_t i;
	for (i = 0; i < maxt; ++i) {
		sum += t[i];
	}
	sum /= ((double) maxt);
	return sum;
}

#endif /* _URAND__H */
