/* gcd.c -
 * implements Euclid's greatest common divisor algorithm
 * for a number of integer types.
 * returns 0 if a user error (such as one of the arguments is non-positive)
 */
/** ** Copyright *********************************************************** **
 ** 									     **
 ** Copyright 1998 by R.K.Owen,Ph.D.		                      	     **
 ** last known email: librko@kooz.sj.ca.us				     **
 **                   rk@owen.sj.ca.us					     **
 ** see LICENSE.LGPL, which must be provided, for details		     **
 ** 									     **
 ** ************************************************************************ **/

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#define _GCD(NM, TYPE) \
TYPE NM(TYPE a, TYPE b) { TYPE t; \
	if (a <= 0 || b <= 0) return 0; \
	if (a < b) { /* swap */ t = a; a = b; b = t; } \
	while ((t = a%b) > 1) { a = b; b = t; } \
	if (t)	return t; else	return b; \
}

#define _UGCD(NM, TYPE) \
TYPE NM(unsigned TYPE a, unsigned TYPE b) { unsigned TYPE t; \
	if (a == 0 || b == 0) return 0; \
	if (a < b) { /* swap */ t = a; a = b; b = t; } \
	while ((t = a%b) > 1) { a = b; b = t; } \
	if (t)	return t; else	return b; \
}

