static const char RCSID[]="@(#)$Id: uvec.c,v 1.10 1999/09/11 08:00:13 rk Exp $";
static const char AUTHOR[]="@(#)uvec 1.0 10/31/97 R.K.Owen,Ph.D.";
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
 *     This software is copyrighted by R.K.Owen,Ph.D. 1997
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
#include "librko.h"

static char TAG[5] = "UVEC";
#define 	UVECNEXTLENGTH(x)	(((x)*3)/2)
#define 	UVECPREVLENGTH(x)	(((x)*2)/3)

/* ---------------------------------------------------------------------- */
/* uvec_ctor - construct Unix vector to capacity cap
 * returns NULL if an error, else the memory location  if OK.
 * uvec_ctor will call uvec_init() to set things up.
 */
uvec *uvec_ctor(int cap) {
	uvec *uv = (uvec *) NULL;

	if (!(uv = (uvec *) malloc(sizeof(uvec)))) {
#ifdef RKOERROR
		(void) rkocpyerror("uvec_ctor : uvec malloc error!");
		rkoerrno = RKOMEMERR;
#endif
		return uv;
	}
	if (uvec_init(uv,cap)) {
#ifdef RKOERROR
		(void) rkopsterror("uvec_ctor : ");
#endif
		free(uv);
		uv = (uvec *) NULL;
	}
	return uv;
}
/* ---------------------------------------------------------------------- */
/* uvec_init - construct Unix vector to capacity cap
 * returns <0 if an error, else 0 if OK as well as all the other functions
 */
int uvec_init(uvec *uv, int cap) {
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
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}
/* ---------------------------------------------------------------------- */
/* uvec_dtor - destroy the uvec (calls uvec_close also) */
int uvec_dtor(uvec **uv) {
	int retval = 0;
	if ((retval = uvec_close(*uv))) return retval;
	free (*uv);
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
		strfree(&(uv->vector[i]));
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
	int i;

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

/* uvec_malloc - malloc & copy a string to element "place"
 * assume no element exists there yet and you have the capacity.
 */
static int uvec_malloc(uvec *uv, char const *str, int place) {
	if (place < 0 || place >= uv->capacity) {
#ifdef RKOERROR
		(void) rkocpyerror("uvec_malloc : invalid place!");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
	if (!(uv->vector[place] = strmalloc(str))) {
#ifdef RKOERROR
		(void) rkopsterror("uvec_malloc : ");
#endif
		return -2;
	}
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}

/* uvec_shift - internal function to shift part of the vector */
/* if start <= 0, then set start=0  (start of vector)
 * if end   <= 0, then set end=number (end of vector)
 * newstart = where to put the range of vector
 * everything between start & newstart is zeroed and assumed to be
 *
 * note 'end' = first element after the last element to move
 */
static int uvec_shift(uvec *uv, int start, int end, int newstart) {
	register int i;
	int n;
	int rstat;

	if (start <= 0) start = 0;
	if (end <= 0) end = uv->number;
	if (newstart < 0) {
#ifdef RKOERROR
		(void) rkocpyerror("uvec_shift : invalid newstart!");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
	if (start > uv->number) {
#ifdef RKOERROR
		(void) rkocpyerror("uvec_shift : invalid start!");
		rkoerrno = RKOUSEERR;
#endif
		return -2;
	}
	if (end > uv->number || end < start) {
#ifdef RKOERROR
		(void) rkocpyerror("uvec_shift : invalid end!");
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
			strfree(&(uv->vector[i]));	/* dealloc elements */
			uv->vector[i] = (char *) NULL;
		}
		for (i = start; i < end; ++i) {
			uv->vector[newstart++] = uv->vector[i];
			uv->vector[i] = (char *) NULL;
		}
	} else {				/* shift downwards */
		n = newstart + end - start;
		if (n >= uv->capacity) {
			/* increase capacity */
			if (rstat = uvec_increase(uv,
				(n>UVECNEXTLENGTH(uv->capacity)
				? n :  UVECNEXTLENGTH(uv->capacity)))) {
#ifdef RKOERROR
				(void) rkopsterror("uvec_shift : ");
#endif
				return rstat - 128;
			}
		}
		for (i = end; i < n; ++i) {
			strfree(&(uv->vector[i]));	/* dealloc elements */
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
/* uvec_insert - insert an element before element "place"
 */
int uvec_insert(uvec *uv, char const *str, int place) {
	int rstat;

	if (place < 0 || place > uv->number) {
#ifdef RKOERROR
		(void) rkocpyerror("uvec_insert : invalid place!");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
	if (rstat = uvec_shift(uv, place, 0, place + 1)) {
#ifdef RKOERROR
		(void) rkopsterror("uvec_insert : ");
#endif
		return rstat - 128;
	}
	if (rstat = uvec_malloc(uv, str, place)) {
#ifdef RKOERROR
		(void) rkopsterror("uvec_insert : ");
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
	if (rstat = uvec_shift(uv, place + 1, 0, place)) {
#ifdef RKOERROR
		(void) rkopsterror("uvec_delete : ");
#endif
		return rstat - 128;
	}
	--(uv->number);
	strfree(&(uv->vector[uv->number]));
	uv->vector[uv->number] = (char *) NULL;

	if (uv->number < uv->capacity/2) {
		if (rstat = uvec_decrease(uv, 0)) {
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
/* uvec_add - add 1 element to end of vector */
int uvec_add(uvec *uv, char const *str) {
	int rstat;
	if (rstat = uvec_insert(uv, str, uv->number)) {
#ifdef RKOERROR
		(void) rkopsterror("uvec_add : ");
#endif
		return rstat - 128;
	}

#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
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
	while (str = va_arg(ap, const char *)) {
		if (rstat = uvec_add(uv, str)) {
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
/* uvec_pop - pop off 1 element at end of vector */
int uvec_pop(uvec *uv) {
	int rstat;
	if (rstat = uvec_delete(uv, uv->number - 1)) {
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
/* some useful functions ...
 * note that all these use no direct access to the structure
 * and form an example set of how to use uvec */
/* ---------------------------------------------------------------------- */

/* uvec_copy_vec - copy an existing vector to an unitialized uvec */
int uvec_copy_vec(uvec *u, char **vec, int number) {
	int rstat;
	char  **ptr = vec;
	int num = 0;
	if (number <= 0) {
/* count number in vector (add 1) */
		while (*ptr++) ++num;
		number = ++num;
	}

	if (rstat = uvec_init(u, number)) {
#ifdef RKOERROR
		(void) rkopsterror("uvec_copy_vec : ");
		return rstat - 128;
#endif
	}
	for (ptr = vec; *ptr != (char *)NULL; ++ptr) {
		if (rstat = uvec_add(u, *ptr)) {
#ifdef RKOERROR
			(void) rkopsterror("uvec_copy_vec : ");
			return rstat - 128;
#endif
		}
	}
	return 0;
}

/* uvec_copy - copy one uvec to another unitialized one */
int uvec_copy(uvec *u, uvec const *v) {
	int rstat;

	if (rstat = uvec_copy_vec(u, uvec_vector(v), uvec_capacity(v))) {
#ifdef RKOERROR
		(void) rkopsterror("uvec_copy : ");
		return rstat - 128;
#endif
	}
	return 0;
}
/* ---------------------------------------------------------------------- */
/* uvec_sort - sort the vector */

static int uvec_sort_cmp_ascend(const void *a, const void *b) {
	return strcmp(*(char **) a, *(char **) b);
}

static int uvec_sort_cmp_descend(const void *a, const void *b) {
	return strcmp(*(char **) b, *(char **) a);
}

#ifndef NO_STRCASECMP
static int uvec_sort_cmp_case_ascend(const void *a, const void *b) {
	return strcasecmp(*(char **) a, *(char **) b);
}

static int uvec_sort_cmp_case_descend(const void *a, const void *b) {
	return strcasecmp(*(char **) b, *(char **) a);
}
#endif /* NO_STRCASECMP */

int uvec_sort(uvec *uv, enum uvec_order type) {
	int (*cmp)(const void *, const void *);
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
#ifndef NO_STRCASECMP
		case UVEC_CASE_ASCEND:
			cmp = uvec_sort_cmp_case_ascend;
			break;
		case UVEC_CASE_DESCEND:
			cmp = uvec_sort_cmp_case_descend;
			break;
#endif /* NO_STRCASECMP */
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
/* uvec_find - finds the first entry in a vector that matches the string
 *	type = uvec sorting type ... the important info is whether to
 *	use a caseless comparison or not.
 */

int uvec_find(uvec *uv, char const *str, enum uvec_order type) {
	int i = 0;
	char **vec;
	int (*cmp)(const void *, const void *);

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
		case UVEC_DESCEND:
			cmp = uvec_sort_cmp_ascend;
			break;
#ifndef NO_STRCASECMP
		case UVEC_CASE_ASCEND:
		case UVEC_CASE_DESCEND:
			cmp = uvec_sort_cmp_case_ascend;
			break;
#endif /* NO_STRCASECMP */
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
	int (*cmp)(const char *, const char *);

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
			cmp = strcmp;
			break;
		case UVEC_DESCEND:
			cmp = strcmp;
			break;
#ifndef NO_STRCASECMP
		case UVEC_CASE_ASCEND:
			cmp = strcasecmp;
			break;
		case UVEC_CASE_DESCEND:
			cmp = strcasecmp;
			break;
#endif /* NO_STRCASECMP */
		default:
#ifdef RKOERROR
			(void) rkocpyerror("uvec_uniq : invalid ordering type!");
			rkoerrno = RKOUSEERR;
#endif
			return -2;
		}
		while (*(vec = (uvec_vector(uv) + i))) {
			if ((*cmp)(*vec, *(vec - 1))) {
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
#ifndef NO_URAND
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
	setseed((INTEGER) seed);
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
#endif /* NO_URAND */
