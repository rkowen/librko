static const char RCSID[]="@(#)$Id: list_push.c,v 1.3 2002/07/18 05:10:36 rk Exp $";
static const char AUTHOR[]="@(#)list 1.0 1998/10/31 R.K.Owen,Ph.D.";
/* list.c -
 * This could have easily been made a C++ class, but is
 * confined to C for easy portability.
 *
 * a "list" is similar to the C++ STL deque, but implemented
 * using (void *), and where the user needs to create & pass
 * certain helper functions.
 */
/** ** Copyright *********************************************************** **
 ** 									     **
 ** Copyright 1998 by R.K.Owen,Ph.D.		                      	     **
 ** last known email: librko@kooz.sj.ca.us				     **
 **                   rk@owen.sj.ca.us					     **
 ** see LICENSE.LGPL, which must be provided, for details		     **
 ** 									     **
 ** ************************************************************************ **/

#include <stdlib.h>
#include <stdarg.h>
#include "list_.h"

/* ---------------------------------------------------------------------- */
/* list_push - add 1 element to the end of list */
int list_push(list *lst, char const *tag, ...) {

	int retval = 0;
	va_list vargs;

	va_start(vargs, tag);	/* get ready to pass extra args */

	retval = list_append_(lst, tag, NULL, vargs);

	va_end(vargs);
	return retval;
}
