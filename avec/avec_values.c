static const char RCSID[]="@(#)$Id: avec_values.c,v 1.1 2002/06/24 21:55:06 rk Exp $";
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

#include <stdlib.h>
#include "config.h"
#ifdef MEMDEBUG
#  include "memdebug.h"
#endif
#include "avec.h"
#ifdef RKOERROR
#  include "rkoerror.h"
#endif

/* ---------------------------------------------------------------------- */
/* avec_values - returns all the values in a NULL terminated vector
 * else returns NULL
 */
void **avec_values(avec *av) {
	void **retval = (void **) NULL;
	void **ptr;
	avec_element **aeptr;

	if (!avec_exists(av)) {
#ifdef RKOERROR
		(void) rkopsterror("avec_values : ");
#endif
		return retval;
	}
	if (!(retval = (void **) calloc(av->number + 1, sizeof(void *)))) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_values : malloc error!");
		rkoerrno = RKOMEMERR;
#endif
		return retval;
	}

	if (!av->number) return retval;	/* empty vector */

	aeptr = (avec_element **) NULL;
	ptr = retval;
	while ((aeptr = avec_walk_r(av,aeptr))) {
		*ptr = (*aeptr)->data;
		ptr++;
	};

#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return retval;
}

