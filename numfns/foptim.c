static const char RCSID[]="@(#)$Id: foptim.c,v 1.3 1999/03/25 15:08:31 rk Exp $";
static const char AUTHOR[]="@(#)foptim 1.1 05/12/94 R.K.Owen,Ph.D.";
/*
 * foptim() finds the minimum or maximum of f(x) within the interval [ax,bx].
 *	iopt	=> 0 for maximum or < 0 for minimum
 *	ax	= left  endpoint of the initial interval
 *	bx	= right endpoint of the initial interval
 *	f	= function subprogram which evaluates f(x) for any x in
 *		  the interval [ax,bx]
 *	tol	= desired length of the interval of uncertainty of any
 *		  final result ( must be >= 0.)
 *	output:	= foptim ordinate approximating an optimum of f(x)
 *		  in the interval [ax,bx]
 *
 * foptim returns an optimum in the given interval [ax,bx] to within a
 * tolerance 3*sqrt(eps)*abs(x) + tol, where eps is the relative machine
 * precision such that eps is the smallest number + 1 that is different
 * from 1.
 *
 * This is based on the subroutine FMIN() in FMMLIB presented in
 * "Introduction to Numerical Analysis" by Forsythe, Malcolm, and Moler,
 * which is a slightly modified translation of the ALGOL 60 procedure LOCALMIN
 * given by Richard Brent, "Algorithms for Minimization without Derivatives",
 * Prentice-Hall, Inc. (1973).
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
 * last known email: rk@owen.sj.ca.us
 *                   rkowen@ckns.net
 *
 *********************************************************************
 */

#ifdef TEST
#  include <stdio.h>
#endif
#include <math.h>
#include "librko.h"

#define Abs(a) (((a)<0)?-(a):(a))
#define Signabs(a,b) ((b)<0?-(a):(a))
#define Sign(a,b) ((b)<0?-Abs(a):Abs(a))

/*
 * if _PRECISION_ is not defined then default to double
 */
#ifndef _PRECISION_
typedef double PRECISION;
#endif

#ifdef __cplusplus
extern "C" {
#endif

static PRECISION _F_a_D_d_(PRECISION ad1, PRECISION ad2) {
        static PRECISION tmp;
        tmp = (ad1 + ad2);      /* force the value out of the registers */
        return tmp;
}

PRECISION foptim(int iopt, PRECISION ax, PRECISION bx,
	PRECISION (*f)(PRECISION), PRECISION tol) {

	const PRECISION zero = 0.0, one = 1.0, two = 2.0, three = 3.0,
		half = 0.5, third = one/three, sqrtwo = 1.4142136;
	static PRECISION eps, sqreps, phi1;
	static char FIRST = (0 == 0);
	PRECISION a,b,d,e,xm,p,q,r,tol1,tol2,u,v,w, fu,fv,fw,fx,x, tmp1;

	if (FIRST) {
		FIRST = !FIRST;
#ifdef TEST
		printf("Initializing foptim with own eps & phi\n");
#endif
		tmp1 = half;
		sqreps = one;
		while (_F_a_D_d_(one, tmp1) != one) {
			eps = tmp1;
			sqreps /= sqrtwo;
			tmp1 *= half;
		}
		phi1 = half*(three - (PRECISION) sqrt(5.0));
	}

/*
 * initialization
 */
	a = ax;
	b = bx;
	x = w = v = a + phi1*(b-a);
	e = zero;
	if (iopt < zero) fw = fv = fx = (*f)(x);
	else fw = fv = fx = -(*f)(x);
/*
 * main loop starts here
 */
	while (1) {
		xm = half*(a + b);
		tol1 = sqreps*Abs(x) + tol*third;
		tol2 = two*tol1;
/*
 * check stopping criterion
 */
		tmp1 = x - xm;
		if (Abs(tmp1) <= (tol2 - half*(b-a))) return x;
/*
 * is Golden section necessary
 */
		if (Abs(e) > tol1) {
			do {
				r = (x - w)*(fx - fv);
				q = (x - v)*(fx - fw);
				p = (x - v)*q - (x - w)*r;
				q = two*(q - r);
				if (q > zero) p = -p;
				q = Abs(q);
				r = e;
				e = d;
/*
 * is parabola acceptable
 */
				tmp1 = half*q*r;
				if (Abs(p) > Abs(tmp1) || p <= q*(a - x)
					|| p >= q*(b - x)) {
/*
 * a Golden section step
 */
					if (x >= xm) e = a - x;
					else e = b - x;
					d = phi1 * e;
					break;
				}
/*
 * a parabolic interpolation step
 */
				d = p/q;
				u = x + d;
/*
 * f must not be evaluated too close to ax or bx
 */
				if ((u - a) < tol2) d = Sign(tol1, xm - x);
				if ((b - u) < tol2) d = Sign(tol1, xm - x);
			} while (0);	/* just once through loop */
		} else {
/*
 * a Golden section step
 */
			if (x >= xm) e = a - x;
			else e = b - x;
			d = phi1 * e;
		}
/*
 * f must not be evaluated too close to x
 */
		if (Abs(d) >= tol1) u = x + d;
		else u = x + Sign(tol1,d);
		if (iopt < zero) fu = (*f)(u);
		else fu = -(*f)(u);
/*
 * update a, b, v, w, and x
 */
		if (fu <= fx) {
			if (u < x) b = x;
			else a = x;
			v = w;
			fv = fw;
			w = x;
			fw = fx;
			x = u;
			fx = fu;
		} else {
			if (u < x) a = u;
			else b = u;
			if (fu <= fw || w == x) {
				v = w;
				fv = fw;
				w = u;
				fw = fu;
			} else if (fu <= fv || v == x || v == w) {
				v = u;
				fv = fu;
			}
		}
	}
}

#ifdef __cplusplus
}
#endif
