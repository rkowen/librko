static const char RCSID[]="@(#)$Id: avec_key_walk_r.c,v 1.3 2005/08/19 15:51:05 rk Exp $";
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
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#include "avec.h"
#include "avec_.h"
#ifdef RKOERROR
#  include "rkoerror.h"
#endif

/* ---------------------------------------------------------------------- */
/* avec_key_walk_r - walks through the entries of the object hash array
 * given by the key vector (NULL terminated list of keys)
 * advances the key vector ptr by one key.
 * else returns NULL when through.
 */
avec_element *avec_key_walk_r(avec *av, char ***keyvec) {
	avec_element *retval = (avec_element *) NULL;

#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	if (!avec_exists(av)) {
#ifdef RKOERROR
		(void) rkopsterror("avec_key_walk_r : ");
#endif
		return retval;
	}

	/* must have key vector */
	if (!keyvec || !*keyvec) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_key_walk_r : must give key vector");
#endif
		return retval;
	}
	/* if value is NULL then finished */
	if (!**keyvec) {
		return retval;
	}

	retval = *(avec_hash_search(AVEC_MATCH, av, **keyvec));
	(*keyvec)++;

	return retval;
}

