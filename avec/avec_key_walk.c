static const char RCSID[]="@(#)$Id: avec_key_walk.c,v 1.1 2005/08/19 05:39:18 rk Exp $";
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
#ifdef RKOERROR
#  include "rkoerror.h"
#endif

/* ---------------------------------------------------------------------- */
/* avec_key_walk - walks through all the entries given by the vector
 * of keys returning an avec_elem * pointer,
 * else returns NULL when through, but is not thread safe
 * First call gives the avec and vector, but subsequent calls need to
 * have NULL passed in for one or the other arguments (best if both).
 */
avec_element *avec_key_walk(avec *av, char const * const *keys) {
	static avec *av_;
	static char **ptrptr_;
	if (av && keys) {	/* first time through */
		av_ = av;
		ptrptr_ = (char **) keys;
	}

	return avec_key_walk_r(av_, &ptrptr_);
}

