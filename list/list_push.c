static const char RCSID[]="@(#)$Id: list_push.c,v 1.1 2002/06/27 22:07:46 rk Exp $";
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
/* list_push - add 1 element to the end of list */
int list_push(list *lst, char const *tag, ...) {

	int retval = 0;
	void *ptr = (void *) NULL;
	list_elem *eptr = (list_elem *) NULL;
	va_list args;

	if (!list_exists(lst, tag)) {
#ifdef RKOERROR
		(void) rkopsterror("list_push : ");
#endif
		return -1;
	}

	va_start(args, tag);	/* get ready to pass extra args */
	if ((retval = (lst->addfn)(&ptr, args)) != 0) {
#ifdef RKOERROR
		(void) rkocpyerror("list_push : user function error!");
		rkoerrno = RKOMEMERR;
#endif
		goto unwind;
	}
	if ((eptr = (list_elem *)malloc(sizeof(list_elem)))
	== (list_elem *) NULL) {
#ifdef RKOERROR
		(void) rkocpyerror("list_push : malloc error!");
		rkoerrno = RKOMEMERR;
#endif
		(void) (lst->delfn)(&ptr, args);
		goto unwind;
	}
	eptr->object = ptr;
	eptr->prev = (list_elem *) NULL;
	eptr->next = (list_elem *) NULL;

	if (lst->first == (list_elem *) NULL) {
		lst->first = eptr;
	}
	if (lst->last != (list_elem *) NULL) {
		eptr->prev = lst->last;
		lst->last->next = eptr;
	}
	lst->last = eptr;
	lst->number++;
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
unwind: va_end(args);
	return retval;
}
