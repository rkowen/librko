static const char RCSID[]="@(#)$Id: list_pop.c,v 1.1 2002/06/27 22:07:46 rk Exp $";
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
#include "list.h"
#ifdef MEMDEBUG
#  include "memdebug.h"
#endif
#ifdef RKOERROR
#  include "rkoerror.h"
#endif

/* ---------------------------------------------------------------------- */
/* list_pop - delete off 1 element at the end of list */
int list_pop(list *lst, char const *tag, ...) {
	int retval = 0;
	void *ptr = (void *) NULL;
	list_elem *eptr = (list_elem *) NULL;
	va_list args;

	if (!list_exists(lst, tag)) {
#ifdef RKOERROR
		(void) rkopsterror("list_pop : ");
#endif
		return -1;
	}

	if (lst->last == (list_elem *) NULL) {		/* already empty */
		return 1;				/* not an error */
	}

	eptr = lst->last;
	ptr = eptr->object;

	va_start(args, tag);	/* get ready to pass extra args */
	if ((retval = (lst->delfn)(&ptr, args))) {
#ifdef RKOERROR
		(void) rkocpyerror("list_pop : user function error!");
		rkoerrno = RKOMEMERR;
#endif
		goto unwind;
	}

	lst->last = eptr->prev;
	if (lst->last == (list_elem *) NULL) {
		lst->first = (list_elem *) NULL;
	} else {
		lst->last->next = (list_elem *) NULL;
	}
	lst->number--;
	free(eptr);
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
unwind: va_end(args);
	return retval;
}
