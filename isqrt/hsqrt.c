static const char RCSID[]="@(#)$Id: hsqrt.c,v 1.1 2002/06/24 17:04:51 rk Exp $";
static const char AUTHOR[]="@(#)isqrt 1.0 1998/11/26 R.K.Owen,Ph.D.";
/* isqrt.c -
 * finds the the integer sqrt of the given integer.
 * returns 0 if a user error (such as the argument is non-positive)
 */
/** ** Copyright *********************************************************** **
 ** 									     **
 ** Copyright 2001 by R.K.Owen,Ph.D.		                      	     **
 ** last known email: librko@kooz.sj.ca.us				     **
 **                   rk@owen.sj.ca.us					     **
 ** see LICENSE.LGPL, which must be provided, for details		     **
 ** 									     **
 ** ************************************************************************ **/

#include "isqrt/isqrt_.h"

/* just like templates - almost */
_ISQRT(hsqrt, short)

