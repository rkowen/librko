static const char RCSID[]="@(#)$Id: list_last.c,v 1.2 2002/07/18 05:10:36 rk Exp $";
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
#include "list_.h"

list_elem *list_last(list const *lst, const char *tag) {
	if (list_exists(lst, tag)) {
#ifdef RKOERROR
		rkoerrno = RKO_OK;
#endif
		return lst->last;
	} else {
#ifdef RKOERROR
		(void) rkopsterror("list_last : ");
#endif
		return (list_elem *) NULL;
	}
}
