static const char RCSID[]="@(#)$Id: avec.c,v 1.3 2002/02/12 06:03:02 rk Exp $";
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

#define 	AVECNEXTCAP(x)	(((x)*3)/2)
#define 	AVECPREVCAP(x)	(((x)*2)/3)

/* ---------------------------------------------------------------------- */
/* wrappers for the StdC string functions
 */
int stdc_malloc (void **data, va_list ap) {
	char const *str = va_arg(ap,char *);
	/* fprintf(stderr, "stdc malloc\n"); */
	if((data = calloc((strlen(str)+1),sizeof(char)))) {
		(void) strcpy((char *) *data, str);
		return 0;
	}
	return -1;
}
int stdc_free (void **data, va_list ap) {
	/* fprintf(stderr, "stdc free\n"); */
	free(*data);
	*data = (char *) NULL;
	return 0;
}

avec_fns stdc_fns = {
	AVEC_STDC,
	stdc_malloc,
	stdc_free
};

/* ---------------------------------------------------------------------- */
#ifdef HAVE_STRMALLOC
/* ---------------------------------------------------------------------- */
/* wrappers for the strmalloc string functions
 */
int str_malloc (void **data, va_list ap) {
	char const *str = va_arg(ap,char *);
	/* fprintf(stderr, "str_malloc\n"); */
	if((data = (void *) strmalloc(str))) return 0;
	return -1;
}

int str_free (void **data, va_list ap) {
	/* fprintf(stderr, "str_free\n"); */
	strfree((char **) data);
	return 0;
}

avec_fns strmalloc_fns = {
	AVEC_STRMALLOC,
	str_malloc,
	str_free
};

avec_fns default_fns = {
	AVEC_STRMALLOC,
	str_malloc,
	str_free
};
#else
avec_fns default_fns = {
	AVEC_STDC,
	stdc_malloc,
	stdc_free
};
#endif
/* set the default set of string functions to use
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
		*((*element)->key) = '\0';
		free((*element)->key);
	}
	free(element);
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
	strcpy((*element)->key, key);
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
enum avec_search {AVEC_MATCH, AVEC_NEXT};

/* avec_hash_search - finds and returns the address in the av->hash
 * if successful, else returns NULL
 * type=AVEC_MATCH - return the matching hash element else NULL
 * type=AVEC_NEXT  - return the next empty hash element else NULL
 */
static avec_element *avec_hash_search(avec_enum avec_search type,
		avec *av, char const *key) {
	avec_element *retval = (avec_element *) NULL;
	SETUP;
	unsigned int	hv,		/* hash value */
			inc = 0,	/* increment for quadratic hashing */
			tv;		/* boolean test result */
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
			if (type == AVEC_MATCH) return av->hash[hv];
			else if(type == AVEC_NEXT) return retval;
		}
	}
	if (av->hash[hv]) {
	}
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return retval;
}

/* ---------------------------------------------------------------------- */
/* - public interface routines                                          - */
/* ---------------------------------------------------------------------- */

/* ---------------------------------------------------------------------- */
/* avec_ctor_ - construct Unix vector to capacity cap and use the
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

/* avec_ctor - construct Unix vector to capacity cap
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
	av->fns.data_del = fns.data_del;
	return av;
}
/* avec_alloc - allocate an unitialized avec object
 * set to use the default string functions
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

	if (!(av->hash = (avec_element **)
		calloc(newcap, sizeof(avec_element)))) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_init : malloc error!");
		rkoerrno = RKOMEMERR;
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
 * use the default string functions
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
/* avec_increase - internal function to increase the size of vector
 * if newcap <= 0 then increase by default size else compute  new capacity
 */
int avec_increase(avec *av, int newcap) {
	int i;

	newcap = (newcap <= 0 ? AVECNEXTCAP(av->capacity) : newcap);
	if (!(av->hash = (avec_element **) realloc(av->hash,
		newcap*sizeof(char *)))) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_increase : realloc error!");
		rkoerrno = RKOMEMERR;
#endif
		return -1;
	}
	av->capacity = newcap;
	/* zero out extra capacity */
	for (i = av->number; i < av->capacity; ++i)
		av->hash[i] = (avec_element *) NULL;
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}
/* ---------------------------------------------------------------------- */
/* avec_decrease - internal function to decrease the size of vector
 * if newcap <= 0 then decrease by default size else compute  new capacity
 */
int avec_decrease(avec *av, int newcap) {

	newcap = (newcap <= 0 ? AVECPREVCAP(av->capacity) : newcap);
	if (!(av->hash = (avec_element **) realloc(av->hash,
		newcap*sizeof(char *)))) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_decrease : realloc error!");
		rkoerrno = RKOMEMERR;
#endif
		return -1;
	}
	av->capacity = newcap;
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

/* ---------------------------------------------------------------------- */
/* avec_insert - insert an element into the hash table,
 * returns 0 if no existing element is there and insert was successful.
 * Else it returns 1 if the key already exists,
 * and < 0 if an error occured.
 */
int avec_insert(avec *av, char const *key, ...) {
	va_list vargs;

	unsigned int	hv,		/* hash value */
			inc = 0,	/* increment for quadratic hashing */
			tv;		/* boolean test result */
	int		retval;		/* user data_add return value */

	if (!avec_exists(av)) {
#ifdef RKOERROR
		(void) rkopsterror("avec_insert : ");
#endif
		return -1;
	}
	if (! key  || ! *key) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_insert : NULL key string!");
		rkoerrno = RKOUSEERR;
#endif
		return -2;
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
			return 1;
		}
	}
	if (av->hash[hv] == (void *) NULL) {	/* add element */
		/* alloc element */
		if (avec_alloc_element(key, &(av->hash[hv]))) {
#ifdef RKOERROR
			rkopsterror("avec_insert : ");
#endif
			return -3;
		}
		/* get variable arg pointer */
		va_start(vargs,key);
		retval = (av->fns.data_add)(&(av->hash[hv]->data), vargs);
		if (retval) {
#ifdef RKOERROR
			if (! rkostrerror() ) {
				rkocpyerror("unspecified user data_add error");
				rkoerrno = RKOUSEERR;
			}
			rkopsterror("avec_insert : ");
#endif
			return retval - 128;
		}
	}
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	av->number++;
	return 0;
}

/* ---------------------------------------------------------------------- */
/* avec_delete - delete an element of the key
 */
int avec_delete(avec *av, char const *key, ...) {
	int retval;
	va_list vargs;

	if (!avec_exists(av)) {
#ifdef RKOERROR
		(void) rkopsterror("avec_insert : ");
#endif
		return -1;
	}
	if (! key  || ! *key) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_insert : NULL key string!");
		rkoerrno = RKOUSEERR;
#endif
		return -2;
	}
/* get the HASH of the key */
	HASH(key,hv, av->capacity);
	if (av->number <= 0) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_delete : empty vector!");
		rkoerrno = RKOUSEERR;
#endif
		return -3;
	}
	va_start(vargs, key);
	retval = (av->fns.data_del)(&(av->hash[av->number]->data),vargs);
	if (retval) {
#ifdef RKOERROR
		if (! rkostrerror() ) {
			rkocpyerror("unspecified user data_del error");
			rkoerrno = RKOUSEERR;
		}
		rkopsterror("avec_delete : ");
#endif
		return retval - 128;
	}
	av->hash[av->number]->data = (char *) NULL;
	(void) avec_dealloc_element(av->hash[hv]);
	av->number--;

#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}

/* ---------------------------------------------------------------------- */
/* avec_keys - returns all the keys in a NULL terminated vector
 */
int avec_keys(avec *av) {
	avec_element *elem = 

	if (! key  || ! *key) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_insert : NULL key string!");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
	if (av->number <= 0) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_delete : empty vector!");
		rkoerrno = RKOUSEERR;
#endif
		return -2;
	}
	(av->fns.data_del)(&(av->hash[av->number]->data),args);
	av->hash[av->number]->data = (char *) NULL;
	av->number--;

#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}

/* rudimentary test code */

#include <stdio.h>

#define _STEST(e, comm) \
	if ((comm) != e) {results++; printf("FAIL");} \
	else printf("OK  ");\
	count++; printf(" %s\n",#comm);

int main () {
	avec u,v, *x, *y;
	int count=0, results=0;
	int e;

	x = avec_ctor(100);
	printf ("num=%d\n", avec_number(x));
	printf ("cap=%d\n", avec_capacity(x));
	_STEST(0, avec_insert(x,"first","This is the first value"));
	printf ("num=%d\n", avec_number(x));
	_STEST(1, avec_insert(x,"first","This is not the first value"));
	printf ("num=%d\n", avec_number(x));
	_STEST(0, avec_insert(x,"second","This is the second value"));
	printf ("num=%d\n", avec_number(x));
	return 0;
}

