static const char RCSID[]="@(#)$Id: gcd.c,v 1.1 1998/11/19 22:58:42 rk Exp $";
static const char AUTHOR[]="@(#)gcd 1.0 11/17/98 R.K.Owen,Ph.D.";
/* gcd.c -
 * implements Euclid's greatest common divisor algorith
 * for a number of integer types.
 * returns 0 if a user error (such as one of the arguments is negative)
 */
/*
 *********************************************************************
 *
 *     This software is copyrighted by R.K.Owen,Ph.D. 1998
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

#define _GCD(NM, TYPE) \
TYPE NM(TYPE a, TYPE b) { TYPE t; \
	if (a < 0 || b < 0) return 0; \
	if (a < b) { /* swap */ t = a; a = b; b = t; } \
	while ((t = a%b) > 1) { a = b; b = t; } \
	if (t)	return t; else	return b; \
}

#define _UGCD(NM, TYPE) \
TYPE NM(unsigned TYPE a, unsigned TYPE b) { unsigned TYPE t; \
	if (a < b) { /* swap */ t = a; a = b; b = t; } \
	while ((t = a%b) > 1) { a = b; b = t; } \
	if (t)	return t; else	return b; \
}

/* just like templates - almost */
_GCD(cgcd,char)
_UGCD(ucgcd,char)
_GCD(hgcd,short)
_UGCD(uhgcd,short)
_GCD(igcd,int)
_UGCD(ugcd,int)
_GCD(lgcd,long)
_UGCD(ulgcd,long)
