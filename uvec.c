static const char RCSID[]="@(#)$Id: uvec.c,v 1.14 2002/02/15 23:01:55 rk Exp $";
static const char AUTHOR[]="@(#)uvec 1.1 10/31/2001 R.K.Owen,Ph.D.";
/* uvec.c -
 * This could have easily been made a C++ class, but is
 * confined to C so that it can be used to interact with the
 * exec() group of POSIX function calls for basic C programs.
 *
 * A Unix vector is a list of char strings, terminated by a
 * (char *) NULL entry.  An example of this is char **argv
 * and char **environ.
 */
/*
 *********************************************************************
 *
 *     This software is copyrighted by R.K.Owen,Ph.D. 2001
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
#ifdef MEMDEBUG
#  include "memdebug.h"
#endif
#include "uvec.h"
#ifdef HAVE_STRMALLOC
#  include "strmalloc.h"
#endif
#ifdef RKOERROR
#  include "rkoerror.h"
#endif

static char TAG[5] = "UVEC";
#define 	UVECNEXTLENGTH(x)	(((x)*3)/2)
#define 	UVECPREVLENGTH(x)	(((x)*2)/3)

/* ---------------------------------------------------------------------- */
/* wrappers for StdC rand functions to conform to librko - urand
 *	if not available
 */
#if !defined(HAVE_URAND)
static double urand (void) {
	return ((double) rand())/(((double) RAND_MAX) + 1);
}
static void setseed(int seed) {
	srand((unsigned int) seed);
}
#endif
/* ---------------------------------------------------------------------- */
/* wrappers for the StdC string functions
 */
static char *stdc_nmalloc (char const *str, size_t n) {
	char *out;
	/* fprintf(stderr, "stdc malloc\n"); */
	if((out = calloc((n+1),sizeof(char))))
		(void) strncpy(out, str, n);
	return out;
}
static int stdc_free (char **str) {
	/* fprintf(stderr, "stdc free\n"); */
	free(*str);
	*str = (char *) NULL;
	return 0;
}

uvec_str stdc_str_fns = {
	UVEC_STDC,
	stdc_nmalloc,
	stdc_free
};

/* ---------------------------------------------------------------------- */
#ifdef HAVE_STRMALLOC
uvec_str strmalloc_str_fns = {
	UVEC_STRMALLOC,
	strnmalloc,
	strfree
};

uvec_str default_str_fns = {
	UVEC_STRMALLOC,
	strnmalloc,
	strfree
};
#else
uvec_str default_str_fns = {
	UVEC_STDC,
	stdc_nmalloc,
	stdc_free
};
#endif
/* set the default set of string functions to use
 */
int uvec_set_strfns(enum uvec_def_str_fns type, uvec_str *strfns) {
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	if (type == UVEC_DEFAULT) {
#ifdef HAVE_STRMALLOC
		type = UVEC_STRMALLOC;
#else
		type = UVEC_STDC;
#endif
	}
	if (type == UVEC_STDC) {
		default_str_fns.str_alloc = stdc_str_fns.str_alloc;
		default_str_fns.str_free = stdc_str_fns.str_free;
#ifdef HAVE_STRMALLOC
	} else if (type == UVEC_STRMALLOC) {
		default_str_fns.str_alloc = stdc_str_fns.str_alloc;
		default_str_fns.str_free = stdc_str_fns.str_free;
#endif
	} else if (type == UVEC_USER) {
		if (strfns == (uvec_str*) NULL
		|| strfns->str_alloc == NULL
		|| strfns->str_free == NULL) {
#ifdef RKOERROR
			(void) rkocpyerror(
				"uvec_set_strfns : null string functions!");
			rkoerrno = RKOUSEERR;
#endif
			return -1;
		}
		default_str_fns.str_alloc = strfns->str_alloc;
		default_str_fns.str_free = strfns->str_free;
	}
	default_str_fns.type = type;
	return 0;
}
/* return what type of string functions are currently default
 */
enum uvec_def_str_fns uvec_get_strfns(void) {
	return default_str_fns.type;
}
/* ---------------------------------------------------------------------- */
/* uvec_ctor_ - construct Unix vector to capacity cap and use the
 * given string functions.
 * returns NULL if an error, else the memory location  if OK.
 * uvec_ctor_ will call uvec_init_() to set things up.
 */
uvec *uvec_ctor_(int cap, uvec_str str_fns) {
	uvec *uv = (uvec *) NULL;
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif

	if ((uvec *) NULL == (uv = uvec_alloc_(str_fns))) {
#ifdef RKOERROR
		if (rkoerrno == RKO_OK) rkoerrno = RKOGENERR;
		(void) rkopsterror("uvec_ctor_ : ");
#endif
		return uv;
	}
	if (uvec_init_(uv,cap,str_fns)) {
#ifdef RKOERROR
		if (rkoerrno == RKO_OK) rkoerrno = RKOGENERR;
		(void) rkopsterror("uvec_ctor_ : ");
#endif
		(void) uvec_dealloc(&uv);
		uv = (uvec *) NULL;
	}
	return uv;
}

/* uvec_ctor - construct Unix vector to capacity cap
 * uvec_ctor will call uvec_ctor_() to set things up.
 */
uvec *uvec_ctor(int cap) {
	return uvec_ctor_(cap, default_str_fns);
}

/* ---------------------------------------------------------------------- */
/* uvec_alloc_ - allocate an unitialized uvec object and use the
 * given string functions.
 * returns NULL if an error, else the memory location  if OK.
 */
uvec *uvec_alloc_(uvec_str strfns) {
	uvec *uv = (uvec *) NULL;

	if (!(uv = (uvec *) malloc(sizeof(uvec)))) {
#ifdef RKOERROR
		(void) rkocpyerror("uvec_alloc_ : uvec malloc error!");
		rkoerrno = RKOMEMERR;
#endif
		return uv;
	}
	uv->str_fns.str_alloc = strfns.str_alloc;
	uv->str_fns.str_free = strfns.str_free;
	return uv;
}
/* uvec_alloc - allocate an unitialized uvec object
 * set to use the default string functions
 * uvec_alloc will call uvec_alloc_() to set things up.
 */
uvec *uvec_alloc(void) {
	return uvec_alloc_(default_str_fns);
}
/* ---------------------------------------------------------------------- */
/* uvec_init_ - construct Unix vector to capacity cap use the
 * given string functions.
 * returns <0 if an error, else 0 if OK as well as all the other functions
 */
int uvec_init_(uvec *uv, int cap, uvec_str strfns) {
	if (uv == (uvec *) NULL) {
#ifdef RKOERROR
		(void) rkocpyerror("uvec_init : null pointer!");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
/* can't guarantee that struct will be initialized to 0 hence use "tag" */
	if (!strncmp(uv->tag,TAG, 5)) {
#ifdef RKOERROR
		(void) rkocpyerror("uvec_init : already initialized!");
		rkoerrno = RKOUSEERR;
#endif
		return -2;
	}
	if (cap < 1) {
#ifdef RKOERROR
		(void) rkocpyerror("uvec_init : invalid capacity!");
		rkoerrno = RKOUSEERR;
#endif
		return -3;
	}
	if (!(uv->vector = (char **) calloc(cap, sizeof(char *)))) {
#ifdef RKOERROR
		(void) rkocpyerror("uvec_init : malloc error!");
		rkoerrno = RKOMEMERR;
#endif
		return -4;
	}
	(void) strcpy(uv->tag, TAG);
	uv->capacity = cap;
	uv->number = 0;
	uv->str_fns.str_alloc = strfns.str_alloc;
	uv->str_fns.str_free = strfns.str_free;
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}
/* uvec_init_ - construct Unix vector to capacity cap
 * use the default string functions
 * uvec_init will call uvec_init_() to set things up.
 */
int uvec_init(uvec *uv, int cap) {
	return uvec_init_(uv, cap, default_str_fns);
}
/* ---------------------------------------------------------------------- */
/* uvec_dtor - destroy the uvec (calls uvec_close also) */
int uvec_dtor(uvec **uv) {
	int retval = 0;
	if ((retval = uvec_close(*uv))) return retval;
	if ((retval = uvec_dealloc(uv))) return retval;
	return retval;
}
/* ---------------------------------------------------------------------- */
/* uvec_dealloc - deallocate the unitialized uvec  */
int uvec_dealloc(uvec **uv) {
	int retval = 0;
	free (*uv);
	*uv = (uvec *) NULL;
	return retval;
}
/* ---------------------------------------------------------------------- */
/* uvec_close - destroy the uvec contents */
int uvec_close(uvec *uv) {
	int i;

	if (uv == (uvec *) NULL) {
#ifdef RKOERROR
		(void) rkocpyerror("uvec_dtor : null pointer!");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
	*(uv->tag) = '\0';
	for (i = 0; i < uv->number; ++i) {
		(uv->str_fns.str_free)(&(uv->vector[i]));
		uv->vector[i] = (char *) NULL;
	}
	free(uv->vector);
	uv->vector = (char **) NULL;
	uv->capacity = 0;
	uv->number = 0;
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}
/* ---------------------------------------------------------------------- */
/* uvec_increase - internal function to increase the size of vector
 * if newcap <= 0 then increase by default size else compute  new capacity
 */
static int uvec_increase(uvec *uv, int newcap) {
	int i;

	newcap = (newcap <= 0 ? UVECNEXTLENGTH(uv->capacity) : newcap);
	if (!(uv->vector = (char **) realloc(uv->vector,
		newcap*sizeof(char *)))) {
#ifdef RKOERROR
		(void) rkocpyerror("uvec_increase : realloc error!");
		rkoerrno = RKOMEMERR;
#endif
		return -1;
	}
	uv->capacity = newcap;
	/* zero out extra capacity */
	for (i = uv->number; i < uv->capacity; ++i)
		uv->vector[i] = (char *) NULL;
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}
/* ---------------------------------------------------------------------- */
/* uvec_decrease - internal function to decrease the size of vector
 * if newcap <= 0 then decrease by default size else compute  new capacity
 */
static int uvec_decrease(uvec *uv, int newcap) {

	newcap = (newcap <= 0 ? UVECPREVLENGTH(uv->capacity) : newcap);
	if (!(uv->vector = (char **) realloc(uv->vector,
		newcap*sizeof(char *)))) {
#ifdef RKOERROR
		(void) rkocpyerror("uvec_decrease : realloc error!");
		rkoerrno = RKOMEMERR;
#endif
		return -1;
	}
	uv->capacity = newcap;
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}

/* uvec_nmalloc - str_alloc & copy a string to element "place"
 * assume no element exists there yet and you have the capacity.
 */
static int uvec_nmalloc(uvec *uv, char const *str, size_t n, int place) {
	if (place < 0 || place >= uv->capacity) {
#ifdef RKOERROR
		(void) rkocpyerror("uvec_nmalloc : invalid place!");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
	if (!(uv->vector[place] = (uv->str_fns.str_alloc)(str,n))) {
#ifdef RKOERROR
		(void) rkopsterror("uvec_nmalloc : ");
#endif
		return -2;
	}
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}

/* uvec_malloc - calls uvec_nmalloc given a null terminated string
 */
#if 0
/* Not used in uvec.c */
static int uvec_malloc(uvec *uv, char const *str, int place) {

	if (! str ) {
#ifdef RKOERROR
		(void) rkocpyerror("uvec_malloc : NULL string!");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
	return uvec_nmalloc(uv, str, strlen(str), place);
}
#endif

/* uvec_shift_vec - internal function to shift part of the vector */
/* if start <= 0, then set start=0  (start of vector)
 * if end   <= 0, then set end=number (end of vector)
 * newstart = where to put the range of vector
 * everything between start & newstart is zeroed and assumed to be
 *
 * note 'end' = first element after the last element to move
 */
static int uvec_shift_vec(uvec *uv, int start, int end, int newstart) {
	register int i;
	int n;
	int rstat;

	if (start <= 0) start = 0;
	if (end <= 0) end = uv->number;
	if (newstart < 0) {
#ifdef RKOERROR
		(void) rkocpyerror("uvec_shift_vec : invalid newstart!");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
	if (start > uv->number) {
#ifdef RKOERROR
		(void) rkocpyerror("uvec_shift_vec : invalid start!");
		rkoerrno = RKOUSEERR;
#endif
		return -2;
	}
	if (end > uv->number || end < start) {
#ifdef RKOERROR
		(void) rkocpyerror("uvec_shift_vec : invalid end!");
		rkoerrno = RKOUSEERR;
#endif
		return -3;
	}
	if (newstart == start) {	/* do nothing */
#ifdef RKOERROR
		rkoerrno = RKO_OK;
#endif
		return 0;
	}
	if (newstart < start) {			/* shift upwards */
		for (i = newstart; i < start; ++i) {
			/* dealloc elements */
			(uv->str_fns.str_free)(&(uv->vector[i]));
		}
		for (i = start; i < end; ++i) {
			uv->vector[newstart++] = uv->vector[i];
			uv->vector[i] = (char *) NULL;
		}
	} else {				/* shift downwards */
		n = newstart + end - start;
		if (n >= uv->capacity) {
			/* increase capacity */
			if ((rstat = uvec_increase(uv,
				(n>UVECNEXTLENGTH(uv->capacity)
				? n :  UVECNEXTLENGTH(uv->capacity))))) {
#ifdef RKOERROR
				(void) rkopsterror("uvec_shift_vec : ");
#endif
				return rstat - 128;
			}
		}
		for (i = end; i < n; ++i) {
			/* dealloc elements */
			(uv->str_fns.str_free)(&(uv->vector[i]));
			uv->vector[i] = (char *) NULL;
		}
		for (i = end - 1; i >= start; --i) {
			uv->vector[--n] = uv->vector[i];
			uv->vector[i] = (char *) NULL;
		}
	}
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}
/* ---------------------------------------------------------------------- */
/* accessor functions */
int uvec_exists(uvec const *uv) {
	int retval = 0;
	if (uv == (uvec *) NULL) {
#ifdef RKOERROR
		(void) rkocpyerror("uvec_exists : null pointer!");
		rkoerrno = RKOUSEERR;
#endif
		retval = 0;
	} else {
		if (strncmp(uv->tag,TAG, 5)) {
#ifdef RKOERROR
			(void) rkocpyerror("uvec_exists : uvec doesn't exist!");
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

int uvec_capacity(uvec const *uv) {
	if (uvec_exists(uv)) {
#ifdef RKOERROR
		rkoerrno = RKO_OK;
#endif
		return uv->capacity;
	} else {
#ifdef RKOERROR
		(void) rkopsterror("uvec_capacity : ");
#endif
		return -1;
	}
}

int uvec_number(uvec const *uv) {
	if (uvec_exists(uv)) {
#ifdef RKOERROR
		rkoerrno = RKO_OK;
#endif
		return uv->number;
	} else {
#ifdef RKOERROR
		(void) rkopsterror("uvec_number : ");
#endif
		return -1;
	}
}

int uvec_end(uvec const *uv) {
	if (uvec_exists(uv)) {
#ifdef RKOERROR
		rkoerrno = RKO_OK;
#endif
		return uv->number - 1;
	} else {
#ifdef RKOERROR
		(void) rkopsterror("uvec_end : ");
#endif
		return -1;
	}
}

char ** uvec_vector(uvec const *uv) {
	if (uvec_exists(uv)) {
#ifdef RKOERROR
		rkoerrno = RKO_OK;
#endif
		return uv->vector;
	} else {
#ifdef RKOERROR
		(void) rkopsterror("uvec_vector : ");
#endif
		return (char **) NULL;
	}
}

/* ---------------------------------------------------------------------- */
/* uvec_ninsert - insert an element of length n before element "place"
 */
int uvec_ninsert(uvec *uv, char const *str, size_t n, int place) {
	int rstat;

	if (place < 0 || place > uv->number) {
#ifdef RKOERROR
		(void) rkocpyerror("uvec_ninsert : invalid place!");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
	if ((rstat = uvec_shift_vec(uv, place, 0, place + 1))) {
#ifdef RKOERROR
		(void) rkopsterror("uvec_ninsert : ");
#endif
		return rstat - 128;
	}
	if ((rstat = uvec_nmalloc(uv, str, n, place))) {
#ifdef RKOERROR
		(void) rkopsterror("uvec_ninsert : ");
#endif
		return rstat - 128;
	}
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	++(uv->number);
	return 0;
}
/* ---------------------------------------------------------------------- */
/* uvec_insert - insert an element before element "place"
 */
int uvec_insert(uvec *uv, char const *str, int place) {

	if (! str ) {
#ifdef RKOERROR
		(void) rkocpyerror("uvec_insert : NULL string!");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
	return uvec_ninsert(uv, str, strlen(str), place);
}

/* ---------------------------------------------------------------------- */
/* uvec_delete - delete an element at element "place"
 */
int uvec_delete(uvec *uv, int place) {
	int rstat;

	if (uv->number <= 0) {
#ifdef RKOERROR
		(void) rkocpyerror("uvec_delete : empty vector!");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
	if (place < 0 || place > uv->number - 1) {
#ifdef RKOERROR
		(void) rkocpyerror("uvec_delete : invalid place!");
		rkoerrno = RKOUSEERR;
#endif
		return -2;
	}
	if ((rstat = uvec_shift_vec(uv, place + 1, 0, place))) {
#ifdef RKOERROR
		(void) rkopsterror("uvec_delete : ");
#endif
		return rstat - 128;
	}
	--(uv->number);
	(uv->str_fns.str_free)(&(uv->vector[uv->number]));
	uv->vector[uv->number] = (char *) NULL;

	if (uv->number < uv->capacity/2) {
		if ((rstat = uvec_decrease(uv, 0))) {
#ifdef RKOERROR
			(void) rkopsterror("uvec_delete : ");
#endif
			return rstat - 128;
		}
	}
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}
/* ---------------------------------------------------------------------- */
/* uvec_nadd - add 1 element of size n to end of vector */
int uvec_nadd(uvec *uv, char const *str, size_t n) {
	int rstat;
	if ((rstat = uvec_ninsert(uv, str, n, uv->number))) {
#ifdef RKOERROR
		(void) rkopsterror("uvec_nadd : ");
#endif
		return rstat - 128;
	}

#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
} 
/* ---------------------------------------------------------------------- */
/* uvec_add - add 1 element to end of vector */
int uvec_add(uvec *uv, char const *str) {

	if (! str ) {
#ifdef RKOERROR
		(void) rkocpyerror("uvec_add : NULL string!");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
	return uvec_nadd(uv, str, strlen(str));
}

/* uvec_push - add 1 element to end of vector (same as uvec_add) */
int uvec_push(uvec *uv, char const *str) {
	return uvec_add(uv,str);
}
/* ---------------------------------------------------------------------- */
/* uvec_addl - add a variable number of elements (terminated by a NULL
 *	argument) to end of vector
 */
int uvec_addl(uvec *uv, ...) {
	int rstat;
	const char *str;
	va_list ap;

	va_start(ap, uv);
	while ((str = va_arg(ap, const char *))) {
		if ((rstat = uvec_add(uv, str))) {
#ifdef RKOERROR
			(void) rkopsterror("uvec_addl : ");
#endif
			return rstat - 128;
		}
	}
	va_end(ap);
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}
/* ---------------------------------------------------------------------- */
/* uvec_pop - pop off 1 element at end of vector
 *	and delete it
 */ 	
int uvec_pop(uvec *uv) {
	int rstat;
	if ((rstat = uvec_delete(uv, uv->number - 1))) {
#ifdef RKOERROR
		(void) rkopsterror("uvec_pop : ");
#endif
		return rstat - 128;
	}

#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}
/* ---------------------------------------------------------------------- */
/* uvec_unshift - add 1 element to the beginning of vector */
int uvec_unshift(uvec *uv, char const *str) {
	int rstat;

	if (! str ) {
#ifdef RKOERROR
		(void) rkocpyerror("uvec_unshift : NULL string!");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
	if ((rstat = uvec_ninsert(uv, str, strlen(str), 0))) {
#ifdef RKOERROR
		(void) rkopsterror("uvec_unshift : ");
#endif
		return rstat - 128;
	}

#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}
/* ---------------------------------------------------------------------- */
/* uvec_shift - shift off 1 element at the beginning of vector
 *	and delete it
 */ 	
int uvec_shift(uvec *uv) {
	int rstat;
	if ((rstat = uvec_delete(uv, 0))) {
#ifdef RKOERROR
		(void) rkopsterror("uvec_shift : ");
#endif
		return rstat - 128;
	}

#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}
/* ---------------------------------------------------------------------- */
/* some useful functions ...
 * note that all these use no direct access to the structure
 * and form an example set of how to use uvec */
/* ---------------------------------------------------------------------- */

/* uvec_count_tok - returns the number of token delimited elements
 * 	in a string (trailing following token is optional)
 * 	returns -1 if an error
 * 	'\' before the token escapes it
 * 	... but it's still possible to confuse the count
 */
int uvec_count_tok(char const *token, char const *string) {
	int num = 0;
	int toklen = 0;
	const char *ptr = string;
	const char *tokptr;

	if (! string)	return 0;
	if (! *string)	return 0;
	if (! token)	return -1;	/* gotta supply a token */
	if (! *token)	return -1;
	toklen = strlen(token);
	if (! toklen)	return -1;
	while (*ptr) {
		num++;
		tokptr = strstr(ptr, token);
		if (! tokptr ) break;
		if ( tokptr > string) {
			if (*(tokptr-1) == '\\') num--;
		}
		ptr = tokptr + toklen;
	}

	return num;
}

/* uvec_copy_str - create a uvec from a token delimited string
 * 	'\' before the token escapes it
 * 	... but it's still possible to confuse the count
 */
int uvec_copy_str(uvec *u, char const *token, char const *string) {
	int num = uvec_count_tok(token,string);
	const char *ptr = string;
	const char *tokptr;
	int toklen = strlen(token);
	int tokfound = 0;
	int rstat;

	if (num < 0) {
#ifdef RKOERROR
		if (rkoerrno == RKO_OK) rkoerrno = RKOGENERR;
		rkocpyerror("uvec_copy_str : token count error!");
#endif
		return -1;
	}

#if 0
	if (! string) {
#ifdef RKOERROR
		rkoerrno = RKOUSEERR;
		rkocpyerror("uvec_copy_str : NULL string!");
#endif
		return -2;
	}
#endif

	if ((rstat = uvec_init(u, num + 1))) {
#ifdef RKOERROR
		(void) rkopsterror("uvec_copy_str : ");
#endif
		return rstat - 128;
	}

	while (ptr && *ptr) {
		tokptr = ptr;
		while (! tokfound) {
			tokptr = strstr(tokptr, token);
			if (tokptr && (tokptr > string)) {
				if (*(tokptr-1) != '\\') {
					tokfound = 1;
				} else {
					tokptr++;
					continue;
				}
			}
			if ((tokptr == string) || !tokptr) tokfound = 1;
		}
		tokfound = 0;
		if (! tokptr ) {
			if ((rstat = uvec_add(u, ptr))) {
#ifdef RKOERROR
				(void) rkopsterror("uvec_copy_str : ");
#endif
				return rstat - 128;
			}
			break;
		} else {
			uvec_nadd(u, ptr, tokptr - ptr);
		}
		ptr = tokptr + toklen;
	}

	return 0;
}

/* uvec_copy_vec - copy an existing vector to an unitialized uvec */
int uvec_copy_vec(uvec *u, char const * const *vec, int number) {
	int rstat;
	char  const * const *ptr = vec;
	int num = 0;
	if (number <= 0) {
/* count number in vector (add 1) */
		while (*ptr++) ++num;
		number = ++num;
	}

	if ((rstat = uvec_init(u, number))) {
#ifdef RKOERROR
		(void) rkopsterror("uvec_copy_vec : ");
#endif
		return rstat - 128;
	}
	for (ptr = vec; *ptr != (char *)NULL; ++ptr) {
		if ((rstat = uvec_add(u, *ptr))) {
#ifdef RKOERROR
			(void) rkopsterror("uvec_copy_vec : ");
#endif
			return rstat - 128;
		}
	}
	return 0;
}

/* uvec_copy - copy one uvec to another unitialized one */
int uvec_copy(uvec *u, uvec const *v) {
	int rstat;

	if ((rstat = uvec_copy_vec(u, (char const * const *) uvec_vector(v),
					uvec_capacity(v)))) {
#ifdef RKOERROR
		(void) rkopsterror("uvec_copy : ");
#endif
		return rstat - 128;
	}
	return 0;
}

/* ---------------------------------------------------------------------- */
/* uvec_sort - sort the vector */

static int uvec_sort_cmp_ascend(void const *a, void const *b) {
	return strcmp(*(char **) a, *(char **) b);
}

static int uvec_sort_cmp_descend(void const *a, void const *b) {
	return strcmp(*(char **) b, *(char **) a);
}

#ifdef HAVE_STRCASECMP
static int uvec_sort_cmp_case_ascend(void const *a, void const *b) {
	return strcasecmp(*(char **) a, *(char **) b);
}

static int uvec_sort_cmp_case_descend(void const *a, void const *b) {
	return strcasecmp(*(char **) b, *(char **) a);
}
#endif /* HAVE_STRCASECMP */

int uvec_sort(uvec *uv, enum uvec_order type) {
	int (*cmp)(void const *, void const *);
	if (!uvec_exists(uv)) {
#ifdef RKOERROR
		(void) rkopsterror("uvec_sort : ");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
	if (uvec_number(uv) > 0) {
		switch (type) {
		case UVEC_ASCEND:
			cmp = uvec_sort_cmp_ascend;
			break;
		case UVEC_DESCEND:
			cmp = uvec_sort_cmp_descend;
			break;
#ifdef HAVE_STRCASECMP
		case UVEC_CASE_ASCEND:
			cmp = uvec_sort_cmp_case_ascend;
			break;
		case UVEC_CASE_DESCEND:
			cmp = uvec_sort_cmp_case_descend;
			break;
#endif /* HAVE_STRCASECMP */
		default:
#ifdef RKOERROR
			(void) rkocpyerror("uvec_sort : invalid ordering type!");
			rkoerrno = RKOUSEERR;
#endif
			return -2;
		}
	}
	qsort((void *) uvec_vector(uv), (size_t) uvec_number(uv),
		sizeof(char *), cmp);
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}

/* ---------------------------------------------------------------------- */
/* uvec_find - finds the first or last entry in a vector that matches
 * 	the string given the uvec sorting type.
 */

int uvec_find(uvec *uv, char const *str, enum uvec_order type) {
	int i = 0;
	char **vec;
	int (*cmp)(void const *, void const *);

#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif

	if (!uvec_exists(uv)) {
#ifdef RKOERROR
		(void) rkopsterror("uvec_find : ");
		rkoerrno = RKOUSEERR;
#endif
		return -2;
	}
	if (uvec_number(uv) > 0) {
		switch (type) {
		case UVEC_ASCEND:
			cmp = uvec_sort_cmp_descend;
			break;
		case UVEC_DESCEND:
			cmp = uvec_sort_cmp_ascend;
			break;
#ifdef HAVE_STRCASECMP
		case UVEC_CASE_ASCEND:
			cmp = uvec_sort_cmp_case_descend;
			break;
		case UVEC_CASE_DESCEND:
			cmp = uvec_sort_cmp_case_ascend;
			break;
#endif /* HAVE_STRCASECMP */
		default:	/* assume case sensitive */
			cmp = uvec_sort_cmp_ascend;
		}
	}
	while (*(vec = (uvec_vector(uv) + i))) {
		if (!(*cmp)(vec,  &str)) return i;
		i++;
	}
	return -1;	/* string not found */
}

/* ---------------------------------------------------------------------- */
/* uvec_uniq - remove all adjacent duplicate elements
 *	type = uvec sorting type ... the import information is whether
 *	to use a caseless comparison or not.
 */
int uvec_uniq(uvec *uv, enum uvec_order type) {
	int i = 1;
	char **vec;
	int (*cmp)(void const *, void const *);

	if (!uvec_exists(uv)) {
#ifdef RKOERROR
		(void) rkopsterror("uvec_uniq : ");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
	if (uvec_number(uv) > 1) {
		switch (type) {
		case UVEC_ASCEND:
			cmp = uvec_sort_cmp_ascend;
			break;
		case UVEC_DESCEND:
			cmp = uvec_sort_cmp_descend;
			break;
#ifdef HAVE_STRCASECMP
		case UVEC_CASE_ASCEND:
			cmp = uvec_sort_cmp_case_ascend;
			break;
		case UVEC_CASE_DESCEND:
			cmp = uvec_sort_cmp_case_descend;
			break;
#endif /* HAVE_STRCASECMP */
		default:
#ifdef RKOERROR
			(void) rkocpyerror("uvec_uniq : invalid ordering type!");
			rkoerrno = RKOUSEERR;
#endif
			return -2;
		}
		while (*(vec = (uvec_vector(uv) + i))) {
			if ((*cmp)(vec, (vec - 1))) {
				i++;			/* go to next */
			} else {
				uvec_delete(uv,i);	/* found match */
			}
		}
	}
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}
/* ---------------------------------------------------------------------- */
/* uvec_reverse - reverses the element order of the vector */
int uvec_reverse(uvec const *uv) {
	int num;
	char **ptrstart, **ptrend;
	char *swap;
	if (!uvec_exists(uv)) {
#ifdef RKOERROR
		(void) rkopsterror("uvec_reverse : ");
		rkoerrno = RKOUSEERR;
		return -1;
#endif
	}
	num = uvec_number(uv);
	if (num > 1) {		/* do reversal */
		ptrstart = uvec_vector(uv);
		ptrend = ptrstart + uvec_end(uv);
		while (ptrstart < ptrend) {
			swap = *ptrend;
			*ptrend-- = *ptrstart;
			*ptrstart++ = swap;
		}
	}
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}
/* ---------------------------------------------------------------------- */
/* uvec_randomize - randomizes the vector  - will be deterministic if
 * use the same seed, However, if seed<=0 then will "randomly"
 * choose one.
 */
int uvec_randomize(uvec const *uv, int seed) {
	char *swap;
	char **vec, **ptr1, **ptr2;
	int num, i;

	if (!uvec_exists(uv)) {
#  ifdef RKOERROR
		(void) rkopsterror("uvec_randomize : ");
		rkoerrno = RKOUSEERR;
		return -1;
#  endif
	}
	if (seed <=0) {
		seed = (int) time(NULL);
	}
	if (seed < 0) {
#  ifdef RKOERROR
		(void) rkocpyerror("uvec_randomize : seed making error!");
		rkoerrno = RKOUSEERR;
		return -2;
#  endif
	}
	setseed(seed);
	num = uvec_number(uv);
	vec = uvec_vector(uv);
	/* make 3*number of swaps */
	for (i = 0; i < 3*num; ++i) {
		ptr1 = vec + (int) (num*urand());
		ptr2 = vec + (int) (num*urand());
		swap = *ptr1;
		*ptr1 = *ptr2;
		*ptr2 = swap;
	}
#  ifdef RKOERROR
	rkoerrno = RKO_OK;
#  endif
	return 0;
}

/* ---------------------------------------------------------------------- */
/* A bunch of conversion routines
 */
/* str2uvec - converts a tokenized string to a uvec
 *	- outputs a uvec object ptr else NULL
 */
uvec *str2uvec(char const *token, char const *string) {
	int num = uvec_count_tok(token,string);
	uvec *tmp = (uvec *) NULL;

	if (num < 0) {
#ifdef RKOERROR
		if (rkoerrno == RKO_OK) rkoerrno = RKOGENERR;
		rkocpyerror("str2uvec : token count error!");
#endif
		return tmp;
	}

#if 0
	if (! string) {
#ifdef RKOERROR
		rkoerrno = RKOUSEERR;
		rkocpyerror("str2uvec : NULL string!");
#endif
		return tmp;
	}
#endif
	if (! token) {
#ifdef RKOERROR
		rkoerrno = RKOUSEERR;
		rkocpyerror("str2uvec : NULL token!");
#endif
		return tmp;
	}
	if ((uvec *) NULL != (tmp = uvec_alloc())) {
		if (uvec_copy_str(tmp, token, string)) {
#ifdef RKOERROR
			if (rkoerrno == RKO_OK) rkoerrno = RKOGENERR;
			rkopsterror("str2uvec : ");
#endif
			(void) uvec_dealloc(&tmp);
			return (uvec *) NULL;
		}
	}
	return tmp;
}

/* vec2uvec - converts a vector into a uvec
 *	- outputs a uvec object ptr else NULL
 */
uvec *vec2uvec(char const * const *vec, int num) {
	uvec *tmp = (uvec *) NULL;

	if ((uvec *) NULL != (tmp = uvec_alloc())) {
		if (uvec_copy_vec(tmp, vec, num)) {
#ifdef RKOERROR
			if (rkoerrno == RKO_OK) rkoerrno = RKOGENERR;
			rkopsterror("vec2uvec : ");
#endif
			(void) uvec_dealloc(&tmp);
			return (uvec *) NULL;
		}
	}
	return tmp;
}

/* uvec2uvec - copies a uvec into a uvec
 *	- outputs a uvec object ptr else NULL
 */
uvec *uvec2uvec(uvec const *uv) {
	uvec *tmp = (uvec *) NULL;

	if ((uvec *) NULL != (tmp = uvec_alloc())) {
		if (uvec_copy(tmp, uv)) {
#ifdef RKOERROR
			if (rkoerrno == RKO_OK) rkoerrno = RKOGENERR;
			rkopsterror("uvec2uvec : ");
#endif
			(void) uvec_dealloc(&tmp);
			return (uvec *) NULL;
		}
	}
	return tmp;
}

/* uvec2str - copies a uvec into a strmalloc object
 *	- outputs a (char *) ptr else NULL
 */
char *uvec2str(uvec const *uv, char const *token) {
	char **argv;
	char *tmp = (char *) NULL;
	int i,len = 0, toklen = strlen(token);

	if (! token) {
#ifdef RKOERROR
		rkoerrno = RKOUSEERR;
		rkocpyerror("str2uvec : NULL token!");
#endif
		return tmp;
	}
	
	if (uvec_exists(uv)) {
		argv = uvec_vector(uv);
		/* count the total length */
		for (i = 0; *argv != (char *) NULL; ++i, ++argv) {
			if (i) len += toklen;
			len += strlen(*argv);
		}
		/* alloc space */
		if ((char *) NULL==(tmp=(uv->str_fns.str_alloc)("", len + 1))) {
#ifdef RKOERROR
			if (rkoerrno == RKO_OK) rkoerrno = RKOGENERR;
			rkopsterror("uvec2str : ");
#endif
			return tmp;
		}
		/* copy stuff over */
		argv = uvec_vector(uv);
		for (i = 0; *argv != (char *) NULL; ++i, ++argv) {
			if (i) strcat(tmp, token);
			strcat(tmp, *argv);
		}
	}
	return tmp;
}

