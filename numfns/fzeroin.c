static const char RCSID[]="@(#)$Id: fzeroin.c,v 1.3 1999/03/25 15:08:31 rk Exp $";
static const char AUTHOR[]="@(#)fzeroin 1.1 04/18/94 R.K.Owen,Ph.D.";
/*
 * fzeroin() finds the zero of (f(x) - y0) within the interval [ax,bx].
 *	ax	= left  endpoint of the initial interval
 *	bx	= right endpoint of the initial interval
 *	y0	= the abcissa value to match
 *	f	= function subprogram which evaluates f(x) for any x in
 *		  the interval [ax,bx]
 *	tol	= desired length of the interval of uncertainty of any
 *		  final result ( must be >= 0.)
 *	output:	= fzeroin ordinate approximating a zero of (f(x) - y0)
 *		  in the interval [ax,bx]
 *
 * It is assumed that f(ax) - y0 and f(bx) - y0 have opposite signs without
 * checking.
 * fzeroin returns a zero x in the given interval [ax,bx] to within a
 * tolerance 2*eps*abs(x) + tol, where eps is the relative machine precision
 * such that eps is the smallest number + 1 that is different from 1.
 *
 * This is based on the subroutine ZEROIN() in FMMLIB presented in
 * "Introduction to Numerical Analysis" by Forsythe, Malcolm, and Moler,
 * which is a slightly modified translation of the ALGOL 60 procedure ZERO
 * given by Richard Brent, "Algorithms for Minimization without Derivatives",
 * Prentice-Hall, Inc. (1973).
 *
 * by R.K.Owen,Ph.D.  04/18/94
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
	tmp = (ad1 + ad2);	/* force the value out of the registers */
	return tmp;
}

PRECISION fzeroin(PRECISION ax, PRECISION bx,
	PRECISION y0, PRECISION (*f)(PRECISION), PRECISION tol) {

	const PRECISION zero = 0.0, one = 1.0, two = 2.0, three = 3.0,
		half = 0.5;
	static PRECISION eps;
	static char FIRST = (0 == 0);
	PRECISION a,b,c,e,d,fa,fb,fc,tol1,xm,p,q,r,s,tmp1,tmp2;

	if (FIRST) {
		FIRST = !FIRST;
#ifdef TEST
		printf("Initializing fzeroin with own eps\n");
#endif
		tmp1 = half;
		while (_F_a_D_d_(one,tmp1) != one) {
			eps = tmp1;
			tmp1 *= half;
		}
	}

/*
 * initialization
 */
	a = ax;
	b = bx;
	fa = (*f)(a) - y0;
	fb = (*f)(b) - y0;
/*
 * begin step
 */
	c = a;
	fc = fa;
	d = b - a;
	e = d;

	while (1) {
/*
 * tighten interval
 */
		if (Abs(fc) < Abs(fb)) {
			a = b;
			b = c;
			c = a;
			fa = fb;
			fb = fc;
			fc = fa;
		}
/*
 * convergence test
 */
		tol1 = eps*Abs(b) + half*tol;
		xm = half*(c - b);
		if (Abs(xm) < tol1 || fb == zero) return (b);	/* end */

		if (Abs(e) < tol1 || Abs(fa) <= Abs(fb)) {
/*
 * bisection
 */
			d = xm;
			e = d;
		} else {
			if (a == c) {
/*
 * linear interpolation
 */
				s = fb/fa;
				p = two*xm*s;
				q = one - s;
			} else {
/*
 * inverse quadratic interpolation
 */
				q = fa/fc;
				r = fb/fc;
				s = fb/fa;
				p = s*(two*xm*q*(q - r) - (b - a)*(r - one));
				q = (q - one)*(r - one)*(s - one);
			}
/*
 * adjusts sign
 */
			if (p > zero) q = -q;
			p = Abs(p);

			tmp1 = tol1*q;
			tmp2 = half*e*q;
			if ((two*p) >= (three*xm*q - Abs(tmp1)) ||
				p >= Abs(tmp2)) {
/*
 * bisection
 */
				d = xm;
				e = d;
			} else {
/*
 * interpolation is acceptable
 */
				e = d;
				d = p/q;
			}
		}
/*
 * complete step
 */
		a = b;
		fa = fb;
		if (Abs(d) > tol1) b += d;
		else b += Signabs(tol1,xm);
		fb = (*f)(b) - y0;

		if ((fb*Signabs(one,fc)) > zero) {
/*
 * begin step again
 */
			c = a;
			fc = fa;
			d = b - a;
			e = d;
		}
	}
}

#ifdef __cplusplus
}
#endif
