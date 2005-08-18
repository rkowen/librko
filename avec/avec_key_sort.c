static const char RCSID[]="@(#)$Id: avec_key_sort.c,v 1.1 2005/08/18 22:30:44 rk Exp $";
static const char AUTHOR[]="@(#)avec 1.1 2008/08/18 R.K.Owen,Ph.D.";
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
#ifdef MEMDEBUG
#  include "memdebug.h"
#endif
#include "avec.h"
#ifdef RKOERROR
#  include "rkoerror.h"
#endif

/* ---------------------------------------------------------------------- */
/* comparison functions */

int avec_key_cmp_ascend(void const *a, void const *b) {
	return strcmp(*(char **) a, *(char **) b);
}

int avec_key_cmp_descend(void const *a, void const *b) {
	return strcmp(*(char **) b, *(char **) a);
}

#ifdef HAVE_STRCASECMP
int avec_key_cmp_case_ascend(void const *a, void const *b) {
	return strcasecmp(*(char **) a, *(char **) b);
}

int avec_key_cmp_case_descend(void const *a, void const *b) {
	return strcasecmp(*(char **) b, *(char **) a);
}
#endif /* HAVE_STRCASECMP */

/* ---------------------------------------------------------------------- */
/* avec_key_sort - provides a way to sort a passed AVEC vector of keys
 * for later use
 * else returns NULL if an error
 */
char const * const *avec_key_sort(char const * const *keys,
	int (*cmp)(void const *a, void const *b)) {

	char const * const *	retval = (char const * const *) NULL;
	char const * const *	ptr = keys;
	size_t count = 0;

	if (!keys || !cmp) {
#ifdef RKOERROR
		(void) rkopsterror("avec_key_sort : ");
		(void) rkocpyerror("avec_key_sort : no keys or cmp fn!");
#endif
		return retval;
	}
	/* count number of keys in vec */
	while (*ptr++) count++;

	qsort((void *) keys, (size_t) count, sizeof(char *), cmp);

	retval = keys;

#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return retval;
}

