static const char RCSID[]="@(#)$Id: list.c,v 1.2 1998/10/26 15:47:31 rk Exp $";
static const char AUTHOR[]="@(#)list 1.0 10/31/98 R.K.Owen,Ph.D.";
/* list.c -
 * This could have easily been made a C++ class, but is
 * confined to C for easy portability.
 *
 * a "list" is similar to the C++ STL deque, but implemented
 * using (void *), and where the user needs to create & pass
 * certain helper functions.
 */
/*
 *********************************************************************
 *
 *     This software is copyrighted by R.K.Owen,Ph.D. 1998
 *
 * The author, R.K.Owen, of this software is not liable for any
 * problems WHATSOEVER which may result from use  or  abuse  of
 * this software. The author, R.K.Owen, grants unlimited rights
 * to anyone who uses and modifies this  software  for  private
 * non-commercial  use  as  long  as  this copyright and rights
 * notice remains in this software and is made available to all
 * recipients of this software.
 *
 * last known email: rkowen@ckns.net
 *                   rk@owen.sj.ca.us
 *
 *********************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "librko.h"

/* warning sizeof(list) does not necessarily give the correct memory size
 * since we make it "expandable" to handle a variable size tag
 */
typedef struct list list;		/* forward declaration */
typedef struct list_elem list_elem;

struct list {
	list_elem *first;		/* head of list */
	list_elem *last;		/* last in list */
	int number;			/* number in list */
	int (*addfn)(void **, va_list);	/* user fn to add user data */
	int (*delfn)(void **, va_list);	/* user fn to del user data */
	char tag[1];			/* name tag for list */
/* followed by expanded memory allocation to contain rest of tag with
 * terminating NULL.  A list object must only be defined as "list *"
 * and set = to list_ctor(TAG,INIT);
 */
};

struct list_elem {
	list_elem *prev;		/* previous one in list */
	list_elem *next;		/* next one in list */
	void *object;			/* pointer to object of interest */
};

/* shield users from  certain internal details */
#define LIST_OBJECT(le)	((le)->object)
#define LIST_NEXT(le)	((le)->next)
#define LIST_PREV(le)	((le)->prev)

/* ---------------------------------------------------------------------- */
/* list_ctor - construct a "list" object
 * returns NULL if an error, else the memory location  if OK.
 * list_ctor will call list_init() to set things up.
 */
list *list_ctor(const char *tag,
int (addfn)(void **, va_list), int (delfn)(void **, va_list)) {

	list *lst = (list *) NULL;

	if (!(lst = (list *) malloc(sizeof(list)+sizeof(tag)))) {
#ifdef RKOERROR
		(void) rkocpyerror("list_ctor : list malloc error!");
		rkoerrno = RKOMEMERR;
#endif
		return lst;
	}
	/* copy in tag to structure and initialize pointers */
	lst->first = (void *) NULL;
	lst->last = (void *) NULL;
	lst->number = 0;
	lst->addfn = addfn;
	lst->delfn = delfn;
	(void) strcpy(lst->tag, tag);

	return lst;
}
/* ---------------------------------------------------------------------- */
/* list_dtor - destroy the list object (needs user delfn if not empty).
 */
int list_dtor(list **lst, char const *tag, ...) {
	int retval = 0;
	list_elem *eptr = (list_elem *) NULL;
	list_elem *nptr = (list_elem *) NULL;
	void *ptr = (void *) NULL;
	va_list args;

	if (*lst == (void *) NULL) return 0;	/* already nulled */
	if (!list_exists(*lst, tag)) {
#ifdef RKOERROR
		(void) rkopsterror("list_dtor : ");
#endif
		return -1;
	}

	if ((*lst)->first != (void *) NULL && (*lst)->last != (void *) NULL) {
		eptr = (*lst)->first;
		ptr = eptr->object;
		va_start(args, tag);	/* get ready to pass extra args */
		while ((eptr != (list_elem *) NULL)) {
			if ((retval = ((*lst)->delfn)(&ptr, args))) {
#ifdef RKOERROR
				(void) rkocpyerror(
					"list_dtor : user function error!");
				rkoerrno = RKOMEMERR;
#endif
				goto unwind;
			}
			nptr = eptr->next;
			free(eptr);
			eptr = nptr;
		}
	}
	(*lst)->first = (*lst)->last = (list_elem *) NULL;
	(*lst)->number = -1;
	(*lst)->tag[0] = '\0';
	free (*lst);
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
unwind: va_end(args);
	return retval;
}
/* ---------------------------------------------------------------------- */
/* accessor functions */
int list_exists(list const *lst, const char *tag) {
	int retval = 0;
	if (lst == (list *) NULL) {
#ifdef RKOERROR
		(void) rkocpyerror("list_exists : null pointer!");
		rkoerrno = RKOUSEERR;
#endif
		retval = 0;
	} else {
		if (strncmp(lst->tag,tag, strlen(tag) + 1)) {
#ifdef RKOERROR
			(void) rkocpyerror("list_exists : list doesn't exist!");
			rkoerrno = RKOUSEERR;
#endif
			retval = 0;
		} else {
#ifdef RKOERROR
			rkoerrno = RKO_OK;
#endif
			retval = 1;
		}
	}
	return retval;
}

int list_number(list const *lst, const char *tag) {
	if (list_exists(lst, tag)) {
#ifdef RKOERROR
		rkoerrno = RKO_OK;
#endif
		return lst->number;
	} else {
#ifdef RKOERROR
		(void) rkopsterror("list_number : ");
#endif
		return -1;
	}
}

list_elem *list_first(list const *lst, const char *tag) {
	if (list_exists(lst, tag)) {
#ifdef RKOERROR
		rkoerrno = RKO_OK;
#endif
		return lst->first;
	} else {
#ifdef RKOERROR
		(void) rkopsterror("list_first : ");
#endif
		return (list_elem *) NULL;
	}
}

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

/* ---------------------------------------------------------------------- */
/* list_add - add 1 element to the end of list */
int list_add(list *lst, char const *tag, ...) {

	int retval = 0;
	void *ptr = (void *) NULL;
	list_elem *eptr = (list_elem *) NULL;
	va_list args;

	if (!list_exists(lst, tag)) {
#ifdef RKOERROR
		(void) rkopsterror("list_add : ");
#endif
		return -1;
	}

	va_start(args, tag);	/* get ready to pass extra args */
	if ((retval = (lst->addfn)(&ptr, args)) != 0) {
#ifdef RKOERROR
		(void) rkocpyerror("list_add : user function error!");
		rkoerrno = RKOMEMERR;
#endif
		goto unwind;
	}
	if ((eptr = (list_elem *)malloc(sizeof(list_elem)))
	== (list_elem *) NULL) {
#ifdef RKOERROR
		(void) rkocpyerror("list_add : malloc error!");
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
/* ---------------------------------------------------------------------- */
/* list_del - delete off 1 element at the end of list */
int list_del(list *lst, char const *tag, ...) {
	int retval = 0;
	void *ptr = (void *) NULL;
	list_elem *eptr = (list_elem *) NULL;
	va_list args;

	if (!list_exists(lst, tag)) {
#ifdef RKOERROR
		(void) rkopsterror("list_del : ");
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
		(void) rkocpyerror("list_del : user function error!");
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
/* ---------------------------------------------------------------------- */
/* list_push - add 1 element to the beginning of list */
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

	if (lst->first != (list_elem *) NULL) {
		eptr->next = lst->first;
		lst->first->prev = eptr;
	}
	lst->first = eptr;
	if (lst->last == (list_elem *) NULL) {
		lst->last = eptr;
	}
	lst->number++;
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
unwind: va_end(args);
	return retval;
}
/* ---------------------------------------------------------------------- */
/* list_pop - delete off 1 element at the beginning of list */
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

	if (lst->first == (list_elem *) NULL) {		/* already empty */
		return 1;				/* not an error */
	}

	eptr = lst->first;
	ptr = eptr->object;

	va_start(args, tag);	/* get ready to pass extra args */
	if ((retval = (lst->delfn)(&ptr, args)) != 0) {
#ifdef RKOERROR
		(void) rkocpyerror("list_pop : user function error!");
		rkoerrno = RKOMEMERR;
#endif
		goto unwind;
	}

	lst->first = eptr->next;
	if (lst->first == (list_elem *) NULL) {
		lst->last = (list_elem *) NULL;
	} else {
		lst->first->prev = (list_elem *) NULL;
	}
	lst->number--;
	free(eptr);
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
unwind: va_end(args);
	return retval;
}

#ifdef TEST

typedef struct mix mix;
struct mix {
	int ival;
	float fval;
};

int addint(void **ptr, va_list ap) {
	int *iptr;
	if ((iptr = malloc(sizeof(int))) == (int *) NULL)
		return -1;
	*iptr = va_arg(ap, int);
	*ptr = iptr;
	return 0;
}

int delint(void **ptr, va_list ap) {
	free(*ptr);
	*ptr = (void *) NULL;
	return 0;
}

enum test_types {int_type, flt_type, mix_type};

void comptest(list *lst, const char *tag, enum test_types ttypes,
	char *expected, char *command) {

	char buffer[512];
	char ebuffer[64];
	list_elem *eptr;

	*buffer = '\0';
	sprintf(buffer,"=%d=", list_number(lst, tag));
	for (eptr = list_first(lst,tag);
	eptr != (list_elem *) NULL; eptr = eptr->next) {
		switch (ttypes) {
		case (int_type):
			sprintf(ebuffer,"%d:", *(int *)LIST_OBJECT(eptr));
			break;
		case (flt_type):
			sprintf(ebuffer,"%2.1f:", *(float *)LIST_OBJECT(eptr));
			break;
		case (mix_type):
			sprintf(ebuffer,"%d|%2.1f:",
				(((mix *)LIST_OBJECT(eptr))->ival),
				(((mix *)LIST_OBJECT(eptr))->fval));
			break;
		}
		strcat(buffer,ebuffer);
	}
	if (strcmp(buffer,expected)) {
		printf("    >%s\nFAIL:%s\n     %s <- expected\n",
			command, buffer, expected);
	} else {
		printf("    >%s\nOK  :%s\n", command, buffer);
	}
}

#  define _ZTEST(l, tg, tt, e, comm) \
	if (comm) rkoperror("test"); \
	comptest(l, tg, tt, e, #comm);

#  define _NTEST(l, tg, tt, e, comm) \
	if (!comm) rkoperror("test"); \
	comptest(l, tg, tt, e, #comm);

int main() {
	list *listint = (list *) NULL;
	const char tagint[] = "INTS";

	list *listflt = (list *) NULL;
	const char tagflt[] = "FLOATS";

	list *listmix = (list *) NULL;
	const char tagmix[] = "MIXED";
	list_elem *eptr, *first, *last;

	int i = 0;

/* add elements to int list */
	_ZTEST(listint, tagint, int_type, "=-1=",
		list_exists(listint, tagint))
	_NTEST(listint, tagint, int_type, "=0=",
		(listint = list_ctor(tagint, addint, delint)))
	_NTEST(listint, tagint, int_type, "=0=",
		list_exists(listint, tagint))
	_ZTEST(listint, tagint, int_type, "=1=3:",
		list_add(listint, tagint, 3))
	_ZTEST(listint, tagint, int_type, "=2=3:2:",
		list_add(listint, tagint, 2))
	_ZTEST(listint, tagint, int_type, "=3=3:2:1:",
		list_add(listint, tagint, 1))
	_ZTEST(listint, tagint, int_type, "=4=2:3:2:1:",
		list_push(listint, tagint, 2))
	_ZTEST(listint, tagint, int_type, "=5=1:2:3:2:1:",
		list_push(listint, tagint, 1))
	
	_ZTEST(listint, tagint, int_type, "=4=1:2:3:2:",
		list_del(listint, tagint))
	_ZTEST(listint, tagint, int_type, "=3=2:3:2:",
		list_pop(listint, tagint))
	_ZTEST(listint, tagint, int_type, "=2=2:3:",
		list_del(listint, tagint))
	_ZTEST(listint, tagint, int_type, "=1=3:",
		list_pop(listint, tagint))
	_ZTEST(listint, tagint, int_type, "=0=",
		list_del(listint, tagint))
	_NTEST(listint, tagint, int_type, "=0=",
		list_pop(listint, tagint))

	_ZTEST(listint, tagint, int_type, "=1=1:",
		list_push(listint, tagint, 1))
	_ZTEST(listint, tagint, int_type, "=0=",
		list_pop(listint, tagint))
	_NTEST(listint, tagint, int_type, "=0=",
		list_del(listint, tagint))
	
	_ZTEST(listint, tagint, int_type, "=1=3:",
		list_add(listint, tagint, 3))
	_ZTEST(listint, tagint, int_type, "=2=3:2:",
		list_add(listint, tagint, 2))
	_ZTEST(listint, tagint, int_type, "=-1=",
		list_dtor(&listint, tagint))

	return 0;
}
#endif /* NO_URAND */
