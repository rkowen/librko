static const char RCSID[]="@(#)$Id: avec_delete.c,v 1.1 2002/06/24 21:55:06 rk Exp $";
static const char AUTHOR[]="@(#)avec 1.0 2002/02/08 R.K.Owen,Ph.D.";
/* avec.c -
 * This could have easily been made a C++ class, but is
 * confined to C so that it can be maximally portable.
 *
 * This is an associative vector package (a hash array)
 * that uses quadratic hashing.
 */
/** ** Copyright *********************************************************** **
 ** 									     **
 ** Copyright 2002 by R.K.Owen,Ph.D.		                      	     **
 ** last known email: librko@kooz.sj.ca.us				     **
 **                   rk@owen.sj.ca.us					     **
 ** see LICENSE.LGPL, which must be provided, for details		     **
 ** 									     **
 ** ************************************************************************ **/

#include <stdarg.h>
#include "config.h"
#include "avec.h"
#include "avec_.h"
#ifdef RKOERROR
#  include "rkoerror.h"
#endif

/* ---------------------------------------------------------------------- */
/* avec_delete - delete an element of the key and returns 0 if successful
 * else returns < 0 if an error
 * and should only return > 0 to signify the key is not gone
 */
int avec_delete(avec *av, char const *key, ...) {
	va_list vargs;
	int retval;
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif

	va_start(vargs, key);

	retval = avec_data_delrm(AVEC_DELETE, av, key, vargs);

	if (retval < 0) {
#ifdef RKOERROR
		rkopsterror("avec_delete : ");
#endif
		return retval;
	}

	return retval;
}

