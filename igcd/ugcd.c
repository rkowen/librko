static const char RCSID[]="@(#)$Id: ugcd.c,v 1.1 2002/06/24 19:20:09 rk Exp $";
static const char AUTHOR[]="@(#)gcd 1.0 1998/11/17 R.K.Owen,Ph.D.";
/* igcd.c -
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

#include "igcd/igcd_.h"

/* just like templates - almost */
_UGCD(ugcd,int)

