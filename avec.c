static const char RCSID[]="@(#)$Id: avec.c,v 1.6 2002/02/14 21:11:58 rk Exp $";
static const char AUTHOR[]="@(#)avec 1.0 2002/02/08 R.K.Owen,Ph.D.";
/* avec.c -
 * This could have easily been made a C++ class, but is
 * confined to C so that it can be maximally portable.
 *
 * This is an associative vector package (a hash array)
 * that uses quadratic hashing.
 */
/*
 *********************************************************************
 *
 *     This software is copyrighted by R.K.Owen,Ph.D. 2002
 *
 * The author, R.K.Owen, of this software is not liable for any
 * problems WHATSOEVER which may result from use  or  abuse  of
 * this software. The author, R.K.Owen, grants unlimited rights
 * to anyone who uses and modifies this  software  for  private
 * non-commercial  use  as  long  as  this copyright and rights
 * notice remains in this software and is made available to all
 * recipients of this software.
 *
 * last known email: librko@kooz.sj.ca.us
 *                   rk@owen.sj.ca.us
 *
 *********************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include "avec.h"
#include "iprime.h"
#ifdef HAVE_STRMALLOC
#  include "strmalloc.h"
#endif
#ifdef RKOERROR
#  include "rkoerror.h"
#endif

/* make these as macros so they can be "inlined" when used */
/* must use the SETUP macro to define the need variables
 *  ... don't use _i,_s,_t otherwise.
 */
#define SETUP	\
unsigned int _i; \
char const *_s, *_t

#define HASH(ss,v,Size) \
	{for(_s=ss,_i=0; *_s; _s++) _i=131 * _i + *_s;v=_i % Size;}

#define STRCMP(ss,tt,v) {v=0; _s=ss; _t=tt; while(*_s & *_t){\
			if(*_s==*_t){_s++;_t++;} else {v = 1; break;}}}


static char TAG[5] = "AVEC";

/* ---------------------------------------------------------------------- */
/* wrappers for the StdC string functions
 */
int stdc_malloc (void **data, va_list ap) {
	char const *str = va_arg(ap,char *);
	if (!data) return -1;
	if (*data) return 1;
	if((*data = calloc((strlen(str)+1),sizeof(char)))) {
		(void) strcpy((char *) *data, str);
		return 0;
	}
	return -2;
}
int stdc_free (void **data, va_list ap) {
	free(*data);
	*data = (char *) NULL;
	return 0;
}

avec_fns stdc_fns = {
	AVEC_STDC,
	stdc_malloc,
	stdc_free,
	stdc_free
};

/* ---------------------------------------------------------------------- */
#ifdef HAVE_STRMALLOC
/* ---------------------------------------------------------------------- */
/* wrappers for the strmalloc string functions
 */
int str_malloc (void **data, va_list ap) {
	char const *str = va_arg(ap,char *);
	if (!data) return -1;
	if (*data) return 1;
	if((*data = (void *) strmalloc(str))) return 0;
	return -2;
}

int str_free (void **data, va_list ap) {
	strfree((char **) data);
	return 0;
}

avec_fns strmalloc_fns = {
	AVEC_STRMALLOC,
	str_malloc,
	str_free,
	str_free
};

avec_fns default_fns = {
	AVEC_STRMALLOC,
	str_malloc,
	str_free,
	str_free
};
#else
avec_fns default_fns = {
	AVEC_STDC,
	stdc_malloc,
	stdc_free
};
#endif
/* set the default set of data functions to use
 */
int avec_set_fns(enum avec_def_fns type, avec_fns *fns) {
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	if (type == AVEC_DEFAULT) {
#ifdef HAVE_STRMALLOC
		type = AVEC_STRMALLOC;
#else
		type = AVEC_STDC;
#endif
	}
	if (type == AVEC_STDC) {
		default_fns.data_add = stdc_fns.data_add;
		default_fns.data_del = stdc_fns.data_del;
#ifdef HAVE_STRMALLOC
	} else if (type == AVEC_STRMALLOC) {
		default_fns.data_add = stdc_fns.data_add;
		default_fns.data_del = stdc_fns.data_del;
#endif
	} else if (type == AVEC_USER) {
		if (fns == (avec_fns*) NULL
		|| fns->data_add == NULL
		|| fns->data_del == NULL) {
#ifdef RKOERROR
			(void) rkocpyerror(
				"avec_set_fns : null data functions!");
			rkoerrno = RKOUSEERR;
#endif
			return -1;
		}
		default_fns.data_add = fns->data_add;
		default_fns.data_del = fns->data_del;
	}
	default_fns.type = type;
	return 0;
}
/* return what type of data functions are currently default
 */
enum avec_def_fns avec_get_fns(void) {
	return default_fns.type;
}

/* ---------------------------------------------------------------------- */
/* - internal routines only                                             - */
/* ---------------------------------------------------------------------- */
/* avec_hash_cap - returns the next largest prime to the given value
 */
static int avec_hash_cap(int cap) {
/* next larger prime */
	while (iprime(cap) != 1) ++cap;
	return cap;
}

/* avec_do_resize - returns 1 if a avec_resize should be done,
 * else 0;
 */
static int avec_do_resize(avec const *av) {
	if (av->percentage < 0) return 0;
	if (av->percentage * av->capacity < 100 * av->number) return 1;
	return 0;
}


/* avec_alloc_hash - does the memory allocation of the hash array
 * returns NULL if an error
 */
static avec_element **avec_alloc_hash(int cap) {
	avec_element **hash = (avec_element **) NULL;

	if (cap < 0) return hash;

	if (!(hash = (avec_element **)
		calloc(cap, sizeof(avec_element)))) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_alloc_hash : malloc error!");
		rkoerrno = RKOMEMERR;
#endif
		return hash;
	}
	return hash;
}

/* avec_dealloc_element - deallocates the memory for an element and returns 0
 * if successful, else < 0;
 */
static int avec_dealloc_element(avec_element **element) {
	if ((! element) || (! *element)) {
#ifdef RKOERROR
		rkoerrno = RKOUSEERR;
		rkocpyerror("avec_dealloc_element : already NULL");
#endif
		return -1;
	}
	if ((*element)->key) {
		free((void *)(*element)->key);
		(char *)((*element)->key) = '\0';
	}
	free((void *) *element);
	*element = (avec_element *) NULL;
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}

/* avec_alloc_element - allocates memory for an element and returns 0
 * if successful, else < 0;
 */
static int avec_alloc_element(char const *key, avec_element **element) {
	int retval = 0;
/* check if key is OK */
	if ((!key) || (!*key)) {
#ifdef RKOERROR
		rkoerrno = RKOUSEERR;
		rkocpyerror("avec_alloc_element : NULL key");
#endif
		retval = -1;
		goto unwind1;
	}
/* allocate an element array */
	if (!(*element = (avec_element *) calloc(sizeof(avec_element), 1))) {
#ifdef RKOERROR
		rkoerrno = RKOMEMERR;
		rkocpyerror("avec_alloc_element : element memory error");
#endif
		retval = -2;
		goto unwind1;
	}
/* allocate the key string space */
	if (!((*element)->key = malloc(strlen(key)+1))) {
#ifdef RKOERROR
		rkoerrno = RKOMEMERR;
		rkocpyerror("avec_alloc_element : key memory error");
#endif
		retval = -3;
		goto unwind2;
	}
/* copy key over */
	strcpy((char *)(*element)->key, key);
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return retval;
unwind2:
	free(element);
unwind1:
	return retval;
}

/* type of key search */
enum avec_search {AVEC_MATCH, AVEC_NEXT, AVEC_INSERT};

/* avec_hash_search - finds and returns the address in the av->hash
 * if successful, else returns NULL
 * type=AVEC_MATCH  - return the matching hash element else NULL
 * type=AVEC_NEXT   - return the next empty hash element else NULL
 * type=AVEC_INSERT - return the matching or empty hash element else NULL
 */
static avec_element **avec_hash_search(enum avec_search type,
		avec *av, char const *key) {
	avec_element **retval = (avec_element **) NULL;
	SETUP;
	unsigned int	hv,		/* hash value */
			inc = 0,	/* increment for quadratic hashing */
			tv;		/* boolean test result */
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	if (!avec_exists(av)) {
#ifdef RKOERROR
		(void) rkopsterror("avec_hash_search : ");
#endif
		return retval;
	}
/* check if key is OK */
	if ((!key) || (!*key)) {
#ifdef RKOERROR
		rkoerrno = RKOUSEERR;
		rkocpyerror("avec_hash_search : NULL key");
#endif
		return retval;
	}

/* get the HASH of the key */
	HASH(key,hv, av->capacity);
/* start search */
	while (inc < av->capacity && av->hash[hv]) {
	/* there is an entry here ... see if it matches */
		STRCMP(av->hash[hv]->key, key, tv);
		if (tv) {	/* no match - keep going */
			hv += ++inc;
			hv %= av->capacity;
			inc++;
		} else {	/* found match */
			if (type == AVEC_MATCH || type == AVEC_INSERT)
				return &(av->hash[hv]);
		}
	}
	/* found empty slot */
	if (type == AVEC_NEXT || type == AVEC_INSERT) return &(av->hash[hv]);

	return retval;
}

/* ---------------------------------------------------------------------- */
/* - public interface routines                                          - */
/* ---------------------------------------------------------------------- */

/* ---------------------------------------------------------------------- */
/* avec_ctor_ - construct associative vector to capacity cap and use the
 * given data functions.
 * returns NULL if an error, else the memory location  if OK.
 * avec_ctor_ will call avec_init_() to set things up.
 */
avec *avec_ctor_(int cap, avec_fns fns) {
	avec *av = (avec *) NULL;
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif

	if ((avec *) NULL == (av = avec_alloc_(fns))) {
#ifdef RKOERROR
		if (rkoerrno == RKO_OK) rkoerrno = RKOGENERR;
		(void) rkopsterror("avec_ctor_ : ");
#endif
		return av;
	}
	if (avec_init_(av,cap,fns)) {
#ifdef RKOERROR
		if (rkoerrno == RKO_OK) rkoerrno = RKOGENERR;
		(void) rkopsterror("avec_ctor_ : ");
#endif
		(void) avec_dealloc(&av);
		av = (avec *) NULL;
	}
	return av;
}

/* avec_ctor - construct associative vector to capacity cap
 * avec_ctor will call avec_ctor_() to set things up.
 */
avec *avec_ctor(int cap) {
	return avec_ctor_(cap, default_fns);
}

/* ---------------------------------------------------------------------- */
/* avec_alloc_ - allocate an unitialized avec object and use the
 * given data functions.
 * returns NULL if an error, else the memory location  if OK.
 */
avec *avec_alloc_(avec_fns fns) {
	avec *av = (avec *) NULL;

	if (!(av = (avec *) malloc(sizeof(avec)))) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_alloc_ : avec malloc error!");
		rkoerrno = RKOMEMERR;
#endif
		return av;
	}
	av->fns.data_add = fns.data_add;
	av->fns.data_add = fns.data_add;
	*(av->tag) = '\0';
	av->number = 0;
	av->capacity = 0;
	av->percentage = -1;
	return av;
}
/* avec_alloc - allocate an unitialized avec object
 * set to use the default data functions
 * avec_alloc will call avec_alloc_() to set things up.
 */
avec *avec_alloc(void) {
	return avec_alloc_(default_fns);
}
/* ---------------------------------------------------------------------- */
/* avec_init_ - construct associative vector to capacity cap use the
 * given data functions.
 * returns <0 if an error, else 0 if OK as well as all the other functions
 */
int avec_init_(avec *av, int cap, avec_fns fns) {
	int newcap;
	if (av == (avec *) NULL) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_init : null pointer!");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
/* can't guarantee that struct will be initialized to 0 hence use "tag" */
	if (!strncmp(av->tag,TAG, 5)) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_init : already initialized!");
		rkoerrno = RKOUSEERR;
#endif
		return -2;
	}
	if (cap < 1) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_init : invalid capacity!");
		rkoerrno = RKOUSEERR;
#endif
		return -3;
	}

	newcap = avec_hash_cap(cap);

	if (!(av->hash = avec_alloc_hash(newcap))) {
#ifdef RKOERROR
		(void) rkopsterror("avec_init : ");
#endif
		return -4;
	}
	(void) strcpy(av->tag, TAG);
	av->capacity = newcap;
	av->number = 0;
	av->fns.data_add = fns.data_add;
	av->fns.data_del = fns.data_del;
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}
/* avec_init_ - construct associative vector to capacity cap
 * use the default data functions
 * avec_init will call avec_init_() to set things up.
 */
int avec_init(avec *av, int cap) {
	return avec_init_(av, cap, default_fns);
}
/* ---------------------------------------------------------------------- */
/* avec_dtor - destroy the avec (calls avec_close also) */
int avec_dtor(avec **av, ...) {
	int retval = 0;
	if ((retval = avec_close(*av))) return retval;
	if ((retval = avec_dealloc(av))) return retval;
	return retval;
}
/* ---------------------------------------------------------------------- */
/* avec_dealloc - deallocate the unitialized avec  */
int avec_dealloc(avec **av) {
	int retval = 0;
	free (*av);
	*av = (avec *) NULL;
	return retval;
}
/* ---------------------------------------------------------------------- */
/* avec_close - destroy the avec contents */
int avec_close(avec *av, ...) {
	int i;
	va_list args;

	if (av == (avec *) NULL) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_dtor : null pointer!");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
	*(av->tag) = '\0';
	for (i = 0; i < av->number; ++i) {
		(av->fns.data_del)(&(av->hash[i]->data), args);
		av->hash[i]->data = (char *) NULL;
	}
	free(av->hash);
	av->hash = (avec_element **) NULL;
	av->capacity = 0;
	av->number = 0;
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}

/* ---------------------------------------------------------------------- */
/* accessor functions */
int avec_exists(avec const *av) {
	int retval = 0;
	if (av == (avec *) NULL) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_exists : null pointer!");
		rkoerrno = RKOUSEERR;
#endif
		retval = 0;
	} else {
		if (strncmp(av->tag,TAG, 5)) {
#ifdef RKOERROR
			(void) rkocpyerror("avec_exists : avec doesn't exist!");
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

int avec_capacity(avec const *av) {
	if (avec_exists(av)) {
#ifdef RKOERROR
		rkoerrno = RKO_OK;
#endif
		return av->capacity;
	} else {
#ifdef RKOERROR
		(void) rkopsterror("avec_capacity : ");
#endif
		return -1;
	}
}

int avec_number(avec const *av) {
	if (avec_exists(av)) {
#ifdef RKOERROR
		rkoerrno = RKO_OK;
#endif
		return av->number;
	} else {
#ifdef RKOERROR
		(void) rkopsterror("avec_number : ");
#endif
		return -1;
	}
}

/* avec_resize_percentage - sets/gets the percentage size of number/capacity
 *	that will cause an automatic resize larger.
 *
 *	Set percentage 0 to return the current value.
 *	It will return 0 if no resizing is done.
 *	Set percentage < 0 to disable resizing.
 *	And don't set less than 10 or more than 70.
 *
 *	An automatic resize will approximately double the capacity.
 */

int avec_resize_percentage(avec *av, int percentage) {
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	if (!avec_exists(av)) {
#ifdef RKOERROR
		(void) rkopsterror("avec_resize_percentage : ");
#endif
		return -1;
	}
	if (percentage == 0) {
		return (av->percentage < 0 ? 0 : av->percentage);
	} else if (percentage < 10 || percentage > 70) {
#ifdef RKOERROR
		(void) rkocaterror("avec_resize_percentage : out of range");
		rkoerrno = RKOUSEERR;
#endif
		return -2;
	} else {
		av->percentage = percentage;
	}
	return 0;
}

/* ---------------------------------------------------------------------- */
/* avec_insert - insert an element into the hash table,
 * returns 0 if the insert was successful.
 * Else it returns 1 if the key already exists and the default data fns
 * are used.
 * and < 0 if an error occured.
 */
int avec_insert(avec *av, char const *key, ...) {
	va_list vargs;
	int		  retval;	/* user data_add return value */
	avec_element	**elem;		/* place to insert value */

	if (!(elem = avec_hash_search(AVEC_INSERT, av,key))) {
#ifdef RKOERROR
		rkopsterror("avec_insert : ");
#endif
		return -1;
	}

	/* get variable arg pointer */
	va_start(vargs,key);

	if (*elem == (avec_element *) NULL) {	/* add element */
		/* alloc element */
		if (avec_alloc_element(key, elem)) {
#ifdef RKOERROR
			rkopsterror("avec_insert : ");
#endif
			return -2;
		}
		retval = (av->fns.data_add)(&((*elem)->data), vargs);
		if (retval) {
#ifdef RKOERROR
			if (! rkostrerror() ) {
				rkocpyerror("unspecified user data_add error");
				rkoerrno = RKOUSEERR;
			}
			rkopsterror("avec_insert : ");
#endif
			return retval;
		}
		av->number++;
		if (avec_do_resize(av)) {
			if ((retval = avec_resize(av,0)) < 0) {
#ifdef RKOERROR
				rkopsterror("avec_insert : ");
#endif
				return retval;
			}
		}
		return 0;
	} else { /* value already exists - pass off to data_add anyways */
		retval = (av->fns.data_add)(&((*elem)->data), vargs);
#ifdef RKOERROR
		if (retval < 0) {
			if (! rkostrerror() ) {
				rkocpyerror("unspecified user data_add error2");
				rkoerrno = RKOUSEERR;
			}
			rkopsterror("avec_insert : ");
		}
#endif
		return retval;
	}
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 1;
}

/* ---------------------------------------------------------------------- */
/* avec_delete - delete an element of the key and returns 0 if successful
 * else returns < 0 if an error
 */
int avec_delete(avec *av, char const *key, ...) {
	va_list vargs;
	int retval;
	avec_element	**elem;		/* place to insert value */

	if (!(elem = avec_hash_search(AVEC_MATCH, av,key))) {
#ifdef RKOERROR
		rkopsterror("avec_delete : ");
#endif
		return -1;
	}
	va_start(vargs, key);
	retval = (av->fns.data_del)(&((*elem)->data),vargs);
	if (retval) {
#ifdef RKOERROR
		if (! rkostrerror() ) {
			rkocpyerror("unspecified user data_del error");
			rkoerrno = RKOUSEERR;
		}
		rkopsterror("avec_delete : ");
#endif
		return retval;
	}
	(*elem)->data = (char *) NULL;
	(void) avec_dealloc_element(elem);
	av->number--;

#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}

/* ---------------------------------------------------------------------- */
/* avec_resize - expands the hash array to the new capacity.
 * If the capacity is given as 0, then will automatically set it about
 * twice the current capacity.
 * Returns 0 if successful else returns < 0 if an error
 */
int avec_resize(avec *av, int newcap) {
	int		  oldcap;	/* old capacity */
	avec_element	**hash,		/* hash to store elements */
			**ptrptr,	/* marching ptr */
			**elem;		/* the new place for the element */

#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	if (!avec_exists(av)) {
#ifdef RKOERROR
		(void) rkopsterror("avec_resize : ");
#endif
		return -1;
	}

	/* keep the old hash here */
	hash = av->hash;
	if (!hash) {
#ifdef RKOERROR
		rkoerrno = RKOGENERR;
		(void) rkocpyerror("avec_resize : null hash!");
		return -2;
#endif
	}
	oldcap = av->capacity;

	/* determine the new capacity */
	if (newcap <= 0) {
		newcap = avec_hash_cap(2*av->capacity);
	} else if (newcap < av->number) {
#ifdef RKOERROR
		rkoerrno = RKOUSEERR;
		(void) rkocpyerror("avec_resize : "
			"new capacity can not handle current data");
#endif
		return -3;
	}
	newcap = avec_hash_cap(newcap);

	/* set up new hash */
	if (!(av->hash = avec_alloc_hash(newcap))) {
#ifdef RKOERROR
		(void) rkopsterror("avec_resize : ");
#endif
		return -4;
	}
	av->capacity = newcap;

	/* start walk through old hash and reassign to new hash */
	ptrptr = hash;
	ptrptr--;	/* backup one */
	while ((++ptrptr - hash) < oldcap) {
		if (*ptrptr) {		/* found an element */
			if (!(elem = avec_hash_search(AVEC_NEXT, av,
				(*ptrptr)->key))) {
#ifdef RKOERROR
				rkopsterror("avec_resize : ");
#endif
				return -4;
			} else {		/* got a place */
				*elem = *ptrptr;
			}
		}
	}

	return 0;
}

/* ---------------------------------------------------------------------- */
/* avec_increase - increase the size of the hash
 * if newcap <= 0 then increase by default size else compute  new capacity
 * returns 0 if OK, else < 0
 */
int avec_increase(avec *av, int newcap) {

#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	if (!avec_exists(av)) {
#ifdef RKOERROR
		(void) rkopsterror("avec_increase : ");
#endif
		return -1;
	}

	if (0 < newcap && newcap <= av->capacity) {
#ifdef RKOERROR
		rkoerrno = RKOUSEERR;
		(void) rkocpyerror("avec_increase : can not decrease capacity");
#endif
		return -2;
	}
	return avec_resize(av, newcap);
}

/* ---------------------------------------------------------------------- */
/* avec_decrease - decrease the size of the hash
 * returns 0 if OK, else < 0
 */
int avec_decrease(avec *av, int newcap) {

#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	if (!avec_exists(av)) {
#ifdef RKOERROR
		(void) rkopsterror("avec_decrease : ");
#endif
		return -1;
	}

	if (newcap >= av->capacity) {
#ifdef RKOERROR
		rkoerrno = RKOUSEERR;
		(void) rkocpyerror("avec_decrease : can not decrease capacity");
#endif
		return -2;
	} else if (newcap <= 0) {
#ifdef RKOERROR
		rkoerrno = RKOUSEERR;
		(void) rkocpyerror("avec_decrease : no default capacity");
#endif
		return -2;
	}
	return avec_resize(av, newcap);
}

/* ---------------------------------------------------------------------- */
/* avec_lookup - returns a pointer to the data associated with the key
 * else returns NULL
 */
void *avec_lookup(avec *av, char const *key) {
	avec_element	**elem;		/* place to insert value */
	void *retval = (void *) NULL;

	if (!(elem = avec_hash_search(AVEC_MATCH, av,key))) {
#ifdef RKOERROR
		rkopsterror("avec_lookup : ");
#endif
		return retval;
	}
	return (*elem)->data;
}


/* ---------------------------------------------------------------------- */
/* avec_walk_r - returns all the keys in a NULL terminated vector
 * else returns NULL
 * Must pass a NULL ptrptr on first call.
 */
avec_element **avec_walk_r(avec *av, avec_element **ptrptr) {
	avec_element **retval = (avec_element **) NULL;

#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	if (!avec_exists(av)) {
#ifdef RKOERROR
		(void) rkopsterror("avec_walk_r : ");
#endif
		return retval;
	}

	/* make sure have a value in ptrptr */
	if (ptrptr == (avec_element **) NULL) {
		ptrptr = av->hash;
		ptrptr--;	/* backup one if at start */
	}

	while ((++ptrptr - av->hash) < av->capacity) {
		if (ptrptr && *ptrptr) {	/* found an element */
			return ptrptr;
		}
	}
	
	return retval;
}

/* ---------------------------------------------------------------------- */
/* avec_walk - returns all the keys in a NULL terminated vector
 * else returns NULL, but is not thread safe
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

/* ---------------------------------------------------------------------- */
/* avec_keys - returns all the keys in a NULL terminated vector
 * else returns NULL
 */
char const * const *avec_keys(avec *av) {
	char const * const *retval = (char const * const *) NULL;
	char const **ptr;
	avec_element **aeptr;

	if (!avec_exists(av)) {
#ifdef RKOERROR
		(void) rkopsterror("avec_keys : ");
#endif
		return retval;
	}
	if (!(retval = (char const * const *)
			calloc(av->number + 1, sizeof(char const *)))) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_keys : malloc error!");
		rkoerrno = RKOMEMERR;
#endif
		return retval;
	}

	if (!av->number) return retval;	/* empty vector */

	aeptr = (avec_element **) NULL;
	ptr = (char const **) retval;
	while ((aeptr = avec_walk_r(av,aeptr))) {
		*ptr = (*aeptr)->key;
		ptr++;
	};

#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return retval;
}

/* ---------------------------------------------------------------------- */
/* avec_values - returns all the keys in a NULL terminated vector
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

/* ---------------------------------------------------------------------- */
/* avec_hash - returns a restrictive pointer to the hash
 * else returns NULL
 */
avec_element const * const *avec_hash(avec *av) {
	if (!avec_exists(av)) {
#ifdef RKOERROR
		(void) rkopsterror("avec_hash : ");
#endif
	}
	return (avec_element const * const *) av->hash;
}

/* rudimentary test code */

#include <stdio.h>
#include "uvec.h"

char testbuf[512];

int printout(avec *av, char const *head, int err, char const *ans) {
	char const * const * vec;
	uvec *uv;
	sprintf(testbuf,"e:%d c:%d n:%d",
		err, avec_capacity(av), avec_number(av));
	if (avec_exists(av)) {
		strcat(testbuf," k:");
		vec = avec_keys(av);
		uv = uvec_alloc();
		uvec_copy_vec(uv, vec, 0);
		strcat(testbuf, uvec2str(uv, "|"));
		free((void *)vec);
		uvec_dealloc(&uv);

		strcat(testbuf," v:");
		vec = (char const * const *) avec_values(av);
		uv = uvec_alloc();
		uvec_copy_vec(uv, vec, 0);
		strcat(testbuf, uvec2str(uv, "|"));
		free((void *)vec);
		uvec_dealloc(&uv);
	}
	if (strcmp(testbuf, ans)) {
		printf("FAIL:%-20s=\n    <\t%s\n    >\t%s\n",head,testbuf,ans);
		return 1;
	} else {
		printf("OK  :%-20s=\n\t%s\n",head,testbuf);
		return 0;
	}
# ifdef RKOERROR
	if (rkoerrno != RKO_OK) {
		printf("%s\n", rkostrerror());
	}
# endif
}

int verify(avec *av, char const *head) {
	int    retval = 0;
	avec_element **aeptr;
	char **ptr;
	int    index;
	char  *keys[] = {
		"first",
		"second",
		"third",
		"fourth",
		"fifth",
		"sixth",
		"seventh",
		(char *) NULL
	};
	char  *values[] = {
		"=1=",
		(char *) NULL,
		"=3=",
		"=4=",
		"=5=",
		"=6=",
		(char *) NULL,
		(char *) NULL
	};
	char *key, *val;

	printf("========================\n%s\n========================\n",
		head);
	aeptr = avec_walk(av);
	do {
		key = (char *)(*aeptr)->key;
		val = (char *)(*aeptr)->data;
		ptr = keys;
		index = 0;
		while (*ptr) {	/* find the associatiate value */
			if (!strcmp(*ptr, key)) break;
			ptr++;
			index++;
		}
		if (!strcmp(values[index], val)) {
			printf("OK  >");
		} else {
			printf("FAIL>");
			retval = 1;
		}
		printf("\tkey: %s val: %s\n", key, val);
	} while ((aeptr = avec_walk(NULL)));

	ptr = keys;
	index = 0;
	while (*ptr) {
		key = *ptr;
		val = avec_lookup(av, *ptr);
		if (values[index] == (char *)NULL
		||  val == (char *) NULL) {
			if (values[index] == (char *)NULL
			&&  val == (char *) NULL) {
				printf("OK  >");
				val = "(NULL)";
			} else {
				printf("FAIL>");
				retval = 1;
			}
		} else if (!strcmp(values[index], val)) {
			printf("OK  >");
		} else {
			printf("FAIL>");
			retval = 1;
		}
		printf("\t%s -> %s\n", *ptr, val);
		ptr++;
		index++;
	}
	printf("========================\n");
	return retval;
}

#define _CHECK(c, v, a) \
	count++; estat = c; \
	results += printout(&(v), #c, estat, a);

int str_Xmalloc (void **data, va_list ap) {
	char *newstr;
	char const *str = va_arg(ap,char *);
	if (!data) return -1;
	if (*data) {
		newstr = strnmalloc((char const *)*data,
			strlen((char const *)*data) + strlen(str) + 1);
		strcat(newstr, str);
		strfree((char **)data);
		*data = (void *) newstr;
		return 1;
	}
	if((*data = (void *) strmalloc(str))) return 0;
	return -2;
}

avec_fns user_fns = {
	AVEC_USER,
	str_Xmalloc,
	str_free
};

int main () {
	avec *x, *y;
	int count=0, results=0;
	int estat;

	x = avec_ctor(10);
	_CHECK(avec_number(x),*x,
		"e:0 c:11 n:0 k: v:");
	_CHECK(avec_capacity(x),*x,
		"e:11 c:11 n:0 k: v:");
	_CHECK(avec_resize_percentage(x,0),*x,
		"e:0 c:11 n:0 k: v:");
	_CHECK(avec_resize_percentage(x,40),*x,
		"e:0 c:11 n:0 k: v:");
	_CHECK(avec_resize_percentage(x,0),*x,
		"e:40 c:11 n:0 k: v:");
	_CHECK(avec_insert(x,"first","=1="),*x,
		"e:0 c:11 n:1 k:first v:=1=");
	_CHECK(avec_insert(x,"second","=2="),*x,
		"e:0 c:11 n:2 k:second|first v:=2=|=1=");
	_CHECK(avec_insert(x,"second","#2#"),*x,
		"e:1 c:11 n:2 k:second|first v:=2=|=1=");
	_CHECK(avec_insert(x,"third","=3="),*x,
		"e:0 c:11 n:3 k:second|third|first v:=2=|=3=|=1=");
	_CHECK(avec_delete(x,"second"),*x,
		"e:0 c:11 n:2 k:third|first v:=3=|=1=");
	_CHECK(avec_insert(x,"second","=2="),*x,
		"e:0 c:11 n:3 k:second|third|first v:=2=|=3=|=1=");
	_CHECK(avec_insert(x,"fourth","=4="),*x,
	"e:0 c:11 n:4 k:second|third|first|fourth v:=2=|=3=|=1=|=4=");
	_CHECK(avec_insert(x,"fifth","=5="),*x,
	"e:0 c:23 n:5 k:first|second|third|fourth|fifth v:=1=|=2=|=3=|=4=|=5=");
	_CHECK(avec_insert(x,"sixth","=6="),*x,
	"e:0 c:23 n:6 k:first|sixth|second|third|fourth|fifth v:=1=|=6=|=2=|=3=|=4=|=5=");
	_CHECK(avec_delete(x,"forth"),*x,
	"e:-1 c:23 n:6 k:first|sixth|second|third|fourth|fifth v:=1=|=6=|=2=|=3=|=4=|=5=");
	_CHECK(avec_delete(x,"second"),*x,
	"e:0 c:23 n:5 k:first|sixth|third|fourth|fifth v:=1=|=6=|=3=|=4=|=5=");
	_CHECK(avec_resize(x,4),*x,
	"e:-3 c:23 n:5 k:first|sixth|third|fourth|fifth v:=1=|=6=|=3=|=4=|=5=");
	_CHECK(avec_increase(x,4),*x,
	"e:-2 c:23 n:5 k:first|sixth|third|fourth|fifth v:=1=|=6=|=3=|=4=|=5=");
	_CHECK(avec_decrease(x,4),*x,
	"e:-3 c:23 n:5 k:first|sixth|third|fourth|fifth v:=1=|=6=|=3=|=4=|=5=");
	_CHECK(avec_decrease(x,25),*x,
	"e:-2 c:23 n:5 k:first|sixth|third|fourth|fifth v:=1=|=6=|=3=|=4=|=5=");
	_CHECK(avec_resize(x,14),*x,
	"e:0 c:17 n:5 k:fourth|first|third|fifth|sixth v:=4=|=1=|=3=|=5=|=6=");
	_CHECK(avec_resize(x,6),*x,
	"e:0 c:7 n:5 k:first|fourth|fifth|third|sixth v:=1=|=4=|=5=|=3=|=6=");

	count++;
	results += verify(x, "Default avec_fns");

	y = avec_ctor_(10, user_fns);
	_CHECK(avec_number(y),*y,
		"e:0 c:11 n:0 k: v:");
	_CHECK(avec_capacity(y),*y,
		"e:11 c:11 n:0 k: v:");
	_CHECK(avec_resize_percentage(y,0),*y,
		"e:0 c:11 n:0 k: v:");
	_CHECK(avec_resize_percentage(y,40),*y,
		"e:0 c:11 n:0 k: v:");
	_CHECK(avec_resize_percentage(y,0),*y,
		"e:40 c:11 n:0 k: v:");
	_CHECK(avec_insert(y,"first","=1="),*y,
		"e:0 c:11 n:1 k:first v:=1=");
	_CHECK(avec_insert(y,"second","=2="),*y,
		"e:0 c:11 n:2 k:second|first v:=2=|=1=");
	_CHECK(avec_insert(y,"second","#2#"),*y,
		"e:1 c:11 n:2 k:second|first v:=2=#2#|=1=");
	_CHECK(avec_insert(y,"third","=3="),*y,
		"e:0 c:11 n:3 k:second|third|first v:=2=#2#|=3=|=1=");
	_CHECK(avec_delete(y,"second"),*y,
		"e:0 c:11 n:2 k:third|first v:=3=|=1=");
	_CHECK(avec_insert(y,"second","=2="),*y,
		"e:0 c:11 n:3 k:second|third|first v:=2=|=3=|=1=");
	_CHECK(avec_insert(y,"fourth","=4="),*y,
	"e:0 c:11 n:4 k:second|third|first|fourth v:=2=|=3=|=1=|=4=");
	_CHECK(avec_insert(y,"fifth","=5="),*y,
	"e:0 c:23 n:5 k:first|second|third|fourth|fifth v:=1=|=2=|=3=|=4=|=5=");
	_CHECK(avec_insert(y,"sixth","=6="),*y,
	"e:0 c:23 n:6 k:first|sixth|second|third|fourth|fifth v:=1=|=6=|=2=|=3=|=4=|=5=");
	_CHECK(avec_delete(y,"forth"),*y,
	"e:-1 c:23 n:6 k:first|sixth|second|third|fourth|fifth v:=1=|=6=|=2=|=3=|=4=|=5=");
	_CHECK(avec_delete(y,"second"),*y,
	"e:0 c:23 n:5 k:first|sixth|third|fourth|fifth v:=1=|=6=|=3=|=4=|=5=");
	_CHECK(avec_resize(y,4),*y,
	"e:-3 c:23 n:5 k:first|sixth|third|fourth|fifth v:=1=|=6=|=3=|=4=|=5=");
	_CHECK(avec_increase(y,4),*y,
	"e:-2 c:23 n:5 k:first|sixth|third|fourth|fifth v:=1=|=6=|=3=|=4=|=5=");
	_CHECK(avec_decrease(y,4),*y,
	"e:-3 c:23 n:5 k:first|sixth|third|fourth|fifth v:=1=|=6=|=3=|=4=|=5=");
	_CHECK(avec_decrease(y,25),*y,
	"e:-2 c:23 n:5 k:first|sixth|third|fourth|fifth v:=1=|=6=|=3=|=4=|=5=");
	_CHECK(avec_resize(y,14),*y,
	"e:0 c:17 n:5 k:fourth|first|third|fifth|sixth v:=4=|=1=|=3=|=5=|=6=");
	_CHECK(avec_resize(y,6),*y,
	"e:0 c:7 n:5 k:first|fourth|fifth|third|sixth v:=1=|=4=|=5=|=3=|=6=");

	count++;
	results += verify(y, "User avec_fns");

	if (results) {
		printf("There were %d failures in %d tests\n", results, count);
	} else {
		printf("There were no failures in %d tests\n", count);
	}
	return results;
}
