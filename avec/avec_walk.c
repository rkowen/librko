static const char RCSID[]="@(#)$Id: avec_walk.c,v 1.3 2005/08/18 22:30:44 rk Exp $";
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
/* avec_walk - walks through all the enries in the hash object array
 * else returns NULL when through, but is not thread safe
 * First call gives the avec, subsequent calls need to have NULL passed in.
 */
avec_element **avec_walk(avec *av) {
	static avec *av_;
	static avec_element **ptrptr_;
	if (av) {	/* first time through */
		av_ = av;
		ptrptr_ = (avec_element **) NULL;
	}

	ptrptr_ = avec_walk_r(av_, ptrptr_);
	return ptrptr_;
}

