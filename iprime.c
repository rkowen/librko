static const char RCSID[]="@(#)$Id: iprime.c,v 1.2 2002/02/08 23:10:22 rk Exp $";
static const char AUTHOR[]="@(#)iprime 1.0 09/09/99 R.K.Owen,Ph.D.";
/* iprime.c -
 * a simple minded routine to find out whether a positive number is prime.
 * returns 1 if prime, else it returns the smallest factor > 1 if not.
 * An invalid value causes the function to return 0;
 */
/*
 *********************************************************************
 *
 *     This software is copyrighted by R.K.Owen,Ph.D. 1999
 *
 * The author, R.K.Owen, of this software is not liable for any
 * problems WHATSOEVER which may result from use  or  abuse  of
 * this software. The author, R.K.Owen, grants unlimited rights
 * to anyone who uses and modifies this  software  for  private
 * non-commercial  use  as  long  as  this copyright and rights
 * notice remains in this software and is made available to all
 * recipients of this software.
 *
 * last known email: librko@kooz.sj.ca.us
 *                   rk@owen.sj.ca.us
 *
 *********************************************************************
 */

#include "isqrt.h"

/* a look-up table of the first few primes */
static unsigned char lut[54] = {
  2,     3,     5,     7,    11,    13,    17,    19,    23,
 29,    31,    37,    41,    43,    47,    53,    59,    61,
 67,    71,    73,    79,    83,    89,    97,   101,   103,
107,   109,   113,   127,   131,   137,   139,   149,   151,
157,   163,   167,   173,   179,   181,   191,   193,   197,
199,   211,   223,   227,   229,   233,   239,   241,   251};


/* this assumes that the type is larger than unsigned char */
#define _IPRIME(NM, TYPE, _SQRT) \
TYPE NM(TYPE in) { TYPE i; \
	if (in < 1) return 0; if (in == 1) return 1; \
	for (i=0; i < sizeof(lut)/sizeof(*lut); ++i) { \
		if (in == ((TYPE) lut[i])) return 1; \
		if (!(in%((TYPE) lut[i]))) return (TYPE) lut[i]; }  \
	for (i = lut[sizeof(lut)/sizeof(*lut)-1]+2; i<=_SQRT(in); i+= 2) { \
		if (!(in%i)) return i; } return 1; }

/* just like templates - almost */
_IPRIME(scprime, signed char, scsqrt)
_IPRIME(ucprime, unsigned char, ucsqrt)
_IPRIME(chprime, char, chsqrt)
_IPRIME(hprime, short, hsqrt)
_IPRIME(iprime, int, isqrt)
_IPRIME(lprime, long, lsqrt)
_IPRIME(uhprime, unsigned short, uhsqrt)
_IPRIME(uiprime, unsigned int, uisqrt)
_IPRIME(ulprime, unsigned long, ulsqrt)

