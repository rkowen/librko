const char USMID[]="%W%";
static const char RCSID[]="@(#)$Id: divdiff.c,v 1.2 1998/10/14 15:16:09 rk Exp $";
const char AUTHOR[]="@(#)divdiff 1.1 05/12/94 R.K.Owen,Ph.D.";
/*
 * divdiffcoef(opt,N,t,g,c) computes the divided difference coefficients
 *  for the Newton form of osculatory (kissing) interpolation.
 * Input:
 *	opt	= 0 then derivatives, d^n g(x) / dx^n are scaled by 1/n!
 *		> 0 derivatives are taken as unscaled
 *		< 0 free up memory allocated by divdiffcoef
 *	N	= number of points & derivatives to agree with
 *	t	= array of ordinate points
 *	g	= array of abscissa points and derivatives
 * Output:
 *	c	= array of divided difference coefficients [t1,t2,...,t_i]g
 * Return:
 *		= opt if opt < 0
 *		= 0 if successful and != 0 if not
 *
 * divdiffeval(N,t,g,c,x) evaluates the Newton form of osculatory interpolation
 *  given the coefficients computed by divdiffcoef().
 * Input:
 *	N	= number of points & derivatives to agree with.
 *	t	= array of ordinate points not necessarily in order, except
 *		  points with derivatives must be grouped together
 *	c	= array of divided difference coefficients [t1,t2,...,t_i]g
 *	x	= ordinate point to evaluate the interpolation polynomial at.
 * Return:
 *		= p_N(x) where p_N(x) agrees with g(x) at points given by t[].
 *
 * Example:	g(t) = 1/t
 *
 *		t[] = {  1/2,    1,     1,        1,       2,     2,       4}
 * opt = 0	g[] = {g(1/2), g(1), g'(1)/1!, g"(1)/2!, g(2), g'(2)/1!, g(4)}
 *		    = {   2,     1,    -1,        1,      1/2,  -1/4,    1/4}
 *
 * opt = 1	g[] = {g(1/2), g(1), g'(1),    g"(1),    g(2), g'(2),    g(4)}
 *		    = {   2,     1,    -1,        2,      1/2,  -1/4,    1/4}
 *
 *	Divided Difference Table
 *	t	[t]g	...	[t1,t2,...,tn]g		...
 *	1/2	2
 *			-2
 *	1	1		2
 *			-1		-2
 *	1	1	---	1		1
 *			-1	--	-1/2		-1/2
 *	1	1	---	1/2		1/4		1/8
 *			-1/2		-1/4		-1/16
 *	2	1/2		1/4		1/16
 *			-1/4		-1/16
 *	2	1/2	----	1/16
 *			-1/8
 *	4	1/4
 *
 * The underlined values represent scaled derivatives that were entered in
 * accordance with the definition:
 *
 *                      {  g^(k)(x)/k!     if t_i = ... = t_(i+k)
 *                      {
 * [t_i,...,t_(i+k)]g = { [t_i,...,t_(i+k-1)]g - [t_(i+1),...,t_(i+k)]g
 *                      { ---------------------------------------------
 *                      {                  t_i - t_(i+k)
 *                      {
 *                      {                  if t_i != t_(i+k)
 * where [t]g  =  g(t)
 * 
 * The upper edge of the triangle, [t1,t2,...,t_i]g, represents the coefs
 *   of the Newton interpolation form:
 * p_N(x) = [t1]g + [t1,t2]g*(x-t1) + [t1,t2,t3]g*(x-t1)*(x-t2) + ...
 *		[t1,t2,...,tN]g * (x-t1)*(x-t2)*...*(x-t(N-1))
 *	    _ N
 *	  = \
 *	    /   [t1,t2,...,t_(i-1)]g * (x-t1)*(x-t2)*...*(x-t_(i-1))
 *	    - i=1
 *
 * This polynomial agrees with g(x) at the ordinates t1,t2,...,tN
 * and if t_i = t_(i+1) = ... = t_(i+k) then the polynomial
 * agrees with g(t_i), g'(t_i), g"(t_i), ..., d^k/dx^k g(t_i)
 *
 * note that in the above example the arrays could have been arranged
 * equivalently as:
 *		t[] = { 1/2, 1, 1, 1, 2, 2, 4}
 *	or	t[] = { 1, 1, 1, 1/2, 2, 2, 4}
 *	or	t[] = { 2, 2, 4, 1, 1, 1, 1/2}
 *	etc.
 * as long as points with derivatives are grouped together with the
 * associated g[] array entries.
 *
 * See "A Practical Guide to Splines", by Carl De Boor,
 *		Springer-Verlag, (c)1978
 * for more information and theory on the divided difference method.
 *
 * by R.K.Owen,Ph.D.  05/12/94
 */
/*
 *********************************************************************
 *
 *     This software is copyrighted by R.K.Owen,Ph.D. 1997
 *
 * The author, R.K.Owen, of this software is not liable for any
 * problems WHATSOEVER which may result from use  or  abuse  of
 * this software. The author, R.K.Owen, grants unlimited rights
 * to anyone who uses and modifies this  software  for  private
 * non-commercial  use  as  long  as  this copyright and rights
 * notice remains in this software and is made available to all
 * recipients of this software.
 *
 * last known email: rkowen@ckns.net
 *                   rk@owen.sj.ca.us
 *
 *********************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#ifdef RKOERROR
#  include "librko.h"
#endif

/*
 * if _PRECISION_ is not defined then default to double
 */
#ifndef _PRECISION_
typedef double PRECISION;
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* opt != 0 then input n-th derivative without 1/n! factor */
/* if opt < 0 then free-up memory and exit */
int divdiffcoef(int opt, int N, PRECISION *t, PRECISION *g, PRECISION *c) {
	register int i, j, j1, ji, iji1;
	static int Nidx, *idx;
	PRECISION cj, cj1;

/* free memory */
	if (opt < 0) {
#ifdef TEST
		(void) printf("Freeing    Index array length = %d\n", Nidx);
#endif
		free(idx);
		return opt;
	}
/* free & allocate index array if need larger or 1st time through */
	if (N > Nidx) {
#ifdef TEST
		(void) printf("Allocating Index array length = %d\n", N);
#endif
		free(idx);
		idx = (int *) malloc(N*sizeof(int));
		if (idx == (int *) NULL) {
#ifdef RKOERROR
			rkocpyerror("divdiffcoef : allocation error");
			rkoerrno = RKOMEMERR;
#endif
			return -1;
		}
		Nidx = N;
	}

/* set up idx array and scale derivatives if opt!=0 and copy g to c */
	idx[0] = 0;
	c[0] = g[0];
#ifdef TEST
	if (opt) (void) printf("Using un-scaled derivatives\n");
	else (void) printf("Using scaled derivatives\n");
#endif
	for (i = 1; i < N; ++i) {
		if (t[i] == t[i-1]) {
			idx[i] = idx[i-1];
			if (opt) c[i] = g[i]/((PRECISION) ++j);
			else c[i] = g[i];
		} else {
			j = 0;
			idx[i] = i;
			c[i] = g[i];
		}
	}

/* compute [t1,t2,...,tN]g */
	for (i = 1; i < N; ++i) {
		for (j = N - 1; j >= i; --j) {
			ji = j - i;
			j1 = j - 1;
			
			/* part of derivative batch ... do nothing */
			if (idx[j] == idx[ji]) continue;

			if (idx[j] == j) {	/* an ordinary j-value */
				cj = c[j];
			} else {		/* get proper deriv */
				iji1 = idx[j] + i - 1;
				cj = c[iji1];
				/* derivs become regular values */
				if (iji1 == j) idx[j] = j;
			}

			if (idx[j1] == j1) {	/* an ordinary (j-1)-value */
				cj1 = c[j1];
			} else {		/* get proper deriv */
				cj1 = c[idx[j1] + i - 1];
			}

			/* compute divided difference */
			c[j] = cj - cj1;
			c[j] /= (t[j] - t[ji]);
		}
	}
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}

PRECISION divdiffeval(int N, PRECISION *t, PRECISION *c, PRECISION x) {

	int i = N - 1;
	PRECISION val = c[i];

	while (i > 0) {
		val *= (x - t[--i]);
		val += c[i];
	}
	return val;
}

#ifdef __cplusplus
}
#endif

#ifdef TEST

#  define N 7
#  ifndef Abs
#    define Abs(a) (((a)<0)?-(a):(a))
#  endif

int main(void) {
	int i, j, opt = 0;
	char eq;
	PRECISION
		t[N]	= { .5, 1., 1., 1., 2., 2.,   4.},
		g[N]	= { 2., 1.,-1., 2., .5,-.25, .25},
		gs[N]	= { 2., 1.,-1., 1., .5,-.25, .25},
		ct[N]	= { 2.,-2., 2.,-2., 1.,-.5 , .125},
		c2[N]	= { 1.,-1., 1.,-.5,.25,-.0625, 0.},
		c[N],
		x, p, gg, scale, tol = 1.0e-6;

	printf("6 points with scaled derivatives\n");
	if (divdiffcoef(0,6,t,gs,c)) fprintf(stderr,
		"Error in call to divdiffcoef 1");
	for (i = 0; i < 6; ++i) {
		printf("% 8.3f % 8.3f % 8.3f % 8.3f\n",
			t[i], gs[i], ct[i], c[i]);
	}

	printf("6 points with non-scaled derivatives\n");
	if (divdiffcoef(1,6,t,g,c)) fprintf(stderr,
		"Error in call to divdiffcoef 2");
	for (i = 0; i < 6; ++i) {
		printf("% 8.3f % 8.3f % 8.3f % 8.3f\n",
			t[i], g[i], ct[i], c[i]);
	}

	printf("5 points with scaled derivatives\n");
	if (divdiffcoef(0,5,t+1,gs+1,c+1)) fprintf(stderr,
		"Error in call to divdiffcoef 3");
	for (i = 1; i < 6; ++i) {
		printf("% 8.3f % 8.3f % 8.3f % 8.3f\n",
			t[i], gs[i], c2[i-1], c[i]);
	}

	printf("6 points with scaled derivatives\n");
	if (divdiffcoef(0,6,t+1,gs+1,c+1)) fprintf(stderr,
		"Error in call to divdiffcoef 4");
	for (i = 1; i < 7; ++i) {
		printf("% 8.3f % 8.3f % 8.3f % 8.3f\n",
			t[i], gs[i], c2[i-1], c[i]);
	}

	printf("7 points with scaled derivatives\n");
	if (divdiffcoef(0,7,t,gs,c)) fprintf(stderr,
		"Error in call to divdiffcoef 5");
	for (i = 0; i < 7; ++i) {
		printf("% 8.3f % 8.3f % 8.3f % 8.3f\n",
			t[i], gs[i], ct[i], c[i]);
	}

	if (divdiffcoef(-1,7,t,gs,c) != -1) fprintf(stderr,
		"Error in call to divdiffcoef - free index array");

	printf("Evaluate g(x)=1/x given the above interpolation coefs\n");
        scale = .25;
	for (i = 1; i < 19 ; ++i) {
		x = scale * i;
		p = divdiffeval(7,t,c,x);
		gg = 1./ x;
		eq = ' ';
		for (j = 0; j < 7; ++j) if (Abs(x - t[j]) < tol) {
			eq = '=';
			break;
		}
		printf("% 8.3f % 8.5f %c% 8.5f\n", x, gg, eq, p);
	}

	return 0;
}
#endif /* TEST */
