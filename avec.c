static const char RCSID[]="@(#)$Id: avec.c,v 1.1 2002/02/08 16:27:21 rk Exp $";
static const char AUTHOR[]="@(#)avec 1.0 09/09/99 R.K.Owen,Ph.D.";
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
 *     This software is copyrighted by R.K.Owen,Ph.D. 1999
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

#define HASH(ss,v) {for(s=ss,i=0; *s; s++) i=131 * i + *s;v=i % LibHashSize;}

static char TAG[5] = "AVEC";

/* ---------------------------------------------------------------------- */
/* avec_ctor - construct hash array to at least capacity cap
 * returns NULL if an error, else the memory location  if OK.
 * avec_ctor will call avec_init() to set things up.
 */
avec *avec_ctor(int cap) {
	avec *ha = (avec *) NULL;

	if (!(ha = (avec *) malloc(sizeof(avec)))) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_ctor : avec malloc error!");
		rkoerrno = RKOMEMERR;
#endif
		return ha;
	}
	if (avec_init(ha,cap)) {
#ifdef RKOERROR
		(void) rkopsterror("avec_ctor : ");
#endif
		free(ha);
		ha = (avec *) NULL;
	}
	return ha;
}
/* ---------------------------------------------------------------------- */
/* avec_hash_cap - returns the next largest prime to the given value
 */
static int avec_hash_cap(int cap) {
/* next larger prime */
	while (iprime(cap) != 1) ++cap;
	return cap;
}


/* avec_init - construct Unix vector to at least capacity cap
 * returns <0 if an error, else 0 if OK as well as all the other functions
 */
int avec_init(avec *ha, int cap) {
	if (ha == (avec *) NULL) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_init : null pointer!");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
/* can't guarantee that struct will be initialized to 0 hence use "tag" */
	if (!strncmp(ha->tag,TAG, 5)) {
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
	if (!(ha->vector = (char **) calloc(cap, sizeof(char *)))) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_init : malloc error!");
		rkoerrno = RKOMEMERR;
#endif
		return -4;
	}
	(void) strcpy(ha->tag, TAG);
	ha->capacity = cap;
	ha->number = 0;
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}
/* ---------------------------------------------------------------------- */
/* avec_dtor - destroy the avec (calls avec_close also) */
int avec_dtor(avec **ha) {
	int retval = 0;
	if ((retval = avec_close(*ha))) return retval;
	free (*ha);
	return retval;
}
/* ---------------------------------------------------------------------- */
/* avec_close - destroy the avec contents */
int avec_close(avec *ha) {
	int i;

	if (ha == (avec *) NULL) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_dtor : null pointer!");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
	*(ha->tag) = '\0';
	for (i = 0; i < ha->number; ++i) {
		strfree(&(ha->vector[i]));
		ha->vector[i] = (char *) NULL;
	}
	free(ha->vector);
	ha->vector = (char **) NULL;
	ha->capacity = 0;
	ha->number = 0;
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}
/* ---------------------------------------------------------------------- */
/* avec_increase - internal function to increase the size of vector
 * if newcap <= 0 then increase by default size else compute  new capacity
 */
static int avec_increase(avec *ha, int newcap) {
	int i;

	newcap = (newcap <= 0 ? AVECNEXTLENGTH(ha->capacity) : newcap);
	newcap = avec_hash_cap(newcap);
	if (!(ha->vector = (char **) realloc(ha->vector,
		newcap*sizeof(char *)))) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_increase : realloc error!");
		rkoerrno = RKOMEMERR;
#endif
		return -1;
	}
	ha->capacity = newcap;
	/* zero out extra capacity */
	for (i = ha->number; i < ha->capacity; ++i)
		ha->vector[i] = (char *) NULL;
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}
/* ---------------------------------------------------------------------- */
/* avec_decrease - internal function to decrease the size of vector
 * if newcap <= 0 then decrease by default size else compute  new capacity
 */
static int avec_decrease(avec *ha, int newcap) {
	int i;

	newcap = (newcap <= 0 ? AVECPREVLENGTH(ha->capacity) : newcap);
	if (!(ha->vector = (char **) realloc(ha->vector,
		newcap*sizeof(char *)))) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_decrease : realloc error!");
		rkoerrno = RKOMEMERR;
#endif
		return -1;
	}
	ha->capacity = newcap;
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}

/* avec_malloc - malloc & copy a string to element "place"
 * assume no element exists there yet and you have the capacity.
 */
static int avec_malloc(avec *ha, char const *str, int place) {
	if (place < 0 || place >= ha->capacity) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_malloc : invalid place!");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
	if (!(ha->vector[place] = strmalloc(str))) {
#ifdef RKOERROR
		(void) rkopsterror("avec_malloc : ");
#endif
		return -2;
	}
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}

/* avec_shift - internal function to shift part of the vector */
/* if start <= 0, then set start=0  (start of vector)
 * if end   <= 0, then set end=number (end of vector)
 * newstart = where to put the range of vector
 * everything between start & newstart is zeroed and assumed to be
 *
 * note 'end' = first element after the last element to move
 */
static int avec_shift(avec *ha, int start, int end, int newstart) {
	register int i;
	int n;
	int rstat;

	if (start <= 0) start = 0;
	if (end <= 0) end = ha->number;
	if (newstart < 0) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_shift : invalid newstart!");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
	if (start > ha->number) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_shift : invalid start!");
		rkoerrno = RKOUSEERR;
#endif
		return -2;
	}
	if (end > ha->number || end < start) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_shift : invalid end!");
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
			strfree(&(ha->vector[i]));	/* dealloc elements */
			ha->vector[i] = (char *) NULL;
		}
		for (i = start; i < end; ++i) {
			ha->vector[newstart++] = ha->vector[i];
			ha->vector[i] = (char *) NULL;
		}
	} else {				/* shift downwards */
		n = newstart + end - start;
		if (n >= ha->capacity) {
			/* increase capacity */
			if (rstat = avec_increase(ha,
				(n>AVECNEXTLENGTH(ha->capacity)
				? n :  AVECNEXTLENGTH(ha->capacity)))) {
#ifdef RKOERROR
				(void) rkopsterror("avec_shift : ");
#endif
				return rstat - 128;
			}
		}
		for (i = end; i < n; ++i) {
			strfree(&(ha->vector[i]));	/* dealloc elements */
			ha->vector[i] = (char *) NULL;
		}
		for (i = end - 1; i >= start; --i) {
			ha->vector[--n] = ha->vector[i];
			ha->vector[i] = (char *) NULL;
		}
	}
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}
/* ---------------------------------------------------------------------- */
/* accessor functions */
int avec_exists(avec const *ha) {
	int retval = 0;
	if (ha == (avec *) NULL) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_exists : null pointer!");
		rkoerrno = RKOUSEERR;
#endif
		retval = 0;
	} else {
		if (strncmp(ha->tag,TAG, 5)) {
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

int avec_capacity(avec const *ha) {
	if (avec_exists(ha)) {
#ifdef RKOERROR
		rkoerrno = RKO_OK;
#endif
		return ha->capacity;
	} else {
#ifdef RKOERROR
		(void) rkopsterror("avec_capacity : ");
#endif
		return -1;
	}
}

int avec_number(avec const *ha) {
	if (avec_exists(ha)) {
#ifdef RKOERROR
		rkoerrno = RKO_OK;
#endif
		return ha->number;
	} else {
#ifdef RKOERROR
		(void) rkopsterror("avec_number : ");
#endif
		return -1;
	}
}

int avec_end(avec const *ha) {
	if (avec_exists(ha)) {
#ifdef RKOERROR
		rkoerrno = RKO_OK;
#endif
		return ha->number - 1;
	} else {
#ifdef RKOERROR
		(void) rkopsterror("avec_end : ");
#endif
		return -1;
	}
}

char ** avec_vector(avec const *ha) {
	if (avec_exists(ha)) {
#ifdef RKOERROR
		rkoerrno = RKO_OK;
#endif
		return ha->vector;
	} else {
#ifdef RKOERROR
		(void) rkopsterror("avec_vector : ");
#endif
		return (char **) NULL;
	}
}

/* ---------------------------------------------------------------------- */
/* avec_insert - insert an element before element "place"
 */
int avec_insert(avec *ha, char const *str, int place) {
	int rstat;

	if (place < 0 || place > ha->number) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_insert : invalid place!");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
	if (rstat = avec_shift(ha, place, 0, place + 1)) {
#ifdef RKOERROR
		(void) rkopsterror("avec_insert : ");
#endif
		return rstat - 128;
	}
	if (rstat = avec_malloc(ha, str, place)) {
#ifdef RKOERROR
		(void) rkopsterror("avec_insert : ");
#endif
		return rstat - 128;
	}
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	++(ha->number);
	return 0;
}
/* ---------------------------------------------------------------------- */
/* avec_delete - delete an element at element "place"
 */
int avec_delete(avec *ha, int place) {
	int rstat;

	if (ha->number <= 0) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_delete : empty vector!");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
	if (place < 0 || place > ha->number - 1) {
#ifdef RKOERROR
		(void) rkocpyerror("avec_delete : invalid place!");
		rkoerrno = RKOUSEERR;
#endif
		return -2;
	}
	if (rstat = avec_shift(ha, place + 1, 0, place)) {
#ifdef RKOERROR
		(void) rkopsterror("avec_delete : ");
#endif
		return rstat - 128;
	}
	--(ha->number);
	strfree(&(ha->vector[ha->number]));
	ha->vector[ha->number] = (char *) NULL;

	if (ha->number < ha->capacity/2) {
		if (rstat = avec_decrease(ha, 0)) {
#ifdef RKOERROR
			(void) rkopsterror("avec_delete : ");
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
/* avec_add - add 1 element to end of vector */
int avec_add(avec *ha, char const *str) {
	int rstat;
	if (rstat = avec_insert(ha, str, ha->number)) {
#ifdef RKOERROR
		(void) rkopsterror("avec_add : ");
#endif
		return rstat - 128;
	}

#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}
/* ---------------------------------------------------------------------- */
/* avec_addl - add a variable number of elements (terminated by a NULL
 *	argument) to end of vector
 */
int avec_addl(avec *ha, ...) {
	int rstat;
	const char *str;
	va_list ap;

	va_start(ap, ha);
	while (str = va_arg(ap, const char *)) {
		if (rstat = avec_add(ha, str)) {
#ifdef RKOERROR
			(void) rkopsterror("avec_addl : ");
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
/* avec_pop - pop off 1 element at end of vector */
int avec_pop(avec *ha) {
	int rstat;
	if (rstat = avec_delete(ha, ha->number - 1)) {
#ifdef RKOERROR
		(void) rkopsterror("avec_pop : ");
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
 * and form an example set of how to use avec */
/* ---------------------------------------------------------------------- */

/* avec_copy_vec - copy an existing vector to an unitialized avec */
int avec_copy_vec(avec *u, char **vec, int number) {
	int rstat;
	char  **ptr = vec;
	int num = 0;
	if (number <= 0) {
/* count number in vector (add 1) */
		while (*ptr++) ++num;
		number = ++num;
	}

	if (rstat = avec_init(u, number)) {
#ifdef RKOERROR
		(void) rkopsterror("avec_copy_vec : ");
		return rstat - 128;
#endif
	}
	for (ptr = vec; *ptr != (char *)NULL; ++ptr) {
		if (rstat = avec_add(u, *ptr)) {
#ifdef RKOERROR
			(void) rkopsterror("avec_copy_vec : ");
			return rstat - 128;
#endif
		}
	}
	return 0;
}

/* avec_copy - copy one avec to another unitialized one */
int avec_copy(avec *u, avec const *v) {
	int rstat;

	if (rstat = avec_copy_vec(u, avec_vector(v), avec_capacity(v))) {
#ifdef RKOERROR
		(void) rkopsterror("avec_copy : ");
		return rstat - 128;
#endif
	}
	return 0;
}
/* ---------------------------------------------------------------------- */
/* avec_sort - sort the vector */

static int avec_sort_cmp_ascend(const void *a, const void *b) {
	return strcmp(*(char **) a, *(char **) b);
}

static int avec_sort_cmp_descend(const void *a, const void *b) {
	return strcmp(*(char **) b, *(char **) a);
}

#ifndef NO_STRCASECMP
static int avec_sort_cmp_case_ascend(const void *a, const void *b) {
	return strcasecmp(*(char **) a, *(char **) b);
}

static int avec_sort_cmp_case_descend(const void *a, const void *b) {
	return strcasecmp(*(char **) b, *(char **) a);
}
#endif /* NO_STRCASECMP */

int avec_sort(avec *ha, enum avec_order type) {
	int (*cmp)(const void *, const void *);
	if (!avec_exists(ha)) {
#ifdef RKOERROR
		(void) rkopsterror("avec_sort : ");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
	if (avec_number(ha) > 0) {
		switch (type) {
		case AVEC_ASCEND:
			cmp = avec_sort_cmp_ascend;
			break;
		case AVEC_DESCEND:
			cmp = avec_sort_cmp_descend;
			break;
#ifndef NO_STRCASECMP
		case AVEC_CASE_ASCEND:
			cmp = avec_sort_cmp_case_ascend;
			break;
		case AVEC_CASE_DESCEND:
			cmp = avec_sort_cmp_case_descend;
			break;
#endif /* NO_STRCASECMP */
		default:
#ifdef RKOERROR
			(void) rkocpyerror("avec_sort : invalid ordering type!");
			rkoerrno = RKOUSEERR;
#endif
			return -2;
		}
	}
	qsort((void *) avec_vector(ha), (size_t) avec_number(ha),
		sizeof(char *), cmp);
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}
/* ---------------------------------------------------------------------- */
/* avec_uniq - remove all adjacent duplicate elements
 *	type = avec sorting type ... the import information is whether
 *	to use a caseless comparison or not.
 */
int avec_uniq(avec *ha, enum avec_order type) {
	int i = 1;
	char **vec;
	int (*cmp)(const char *, const char *);

	if (!avec_exists(ha)) {
#ifdef RKOERROR
		(void) rkopsterror("avec_uniq : ");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
	if (avec_number(ha) > 1) {
		switch (type) {
		case AVEC_ASCEND:
			cmp = strcmp;
			break;
		case AVEC_DESCEND:
			cmp = strcmp;
			break;
#ifndef NO_STRCASECMP
		case AVEC_CASE_ASCEND:
			cmp = strcasecmp;
			break;
		case AVEC_CASE_DESCEND:
			cmp = strcasecmp;
			break;
#endif /* NO_STRCASECMP */
		default:
#ifdef RKOERROR
			(void) rkocpyerror("avec_uniq : invalid ordering type!");
			rkoerrno = RKOUSEERR;
#endif
			return -2;
		}
		while (*(vec = (avec_vector(ha) + i))) {
			if ((*cmp)(*vec, *(vec - 1))) {
				i++;			/* go to next */
			} else {
				avec_delete(ha,i);	/* found match */
			}
		}
	}
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}
/* ---------------------------------------------------------------------- */
/* avec_reverse - reverses the element order of the vector */
int avec_reverse(avec const *ha) {
	int num;
	char **ptrstart, **ptrend;
	char *swap;
	if (!avec_exists(ha)) {
#ifdef RKOERROR
		(void) rkopsterror("avec_reverse : ");
		rkoerrno = RKOUSEERR;
		return -1;
#endif
	}
	num = avec_number(ha);
	if (num > 1) {		/* do reversal */
		ptrstart = avec_vector(ha);
		ptrend = ptrstart + avec_end(ha);
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
/* avec_randomize - randomizes the vector  - will be deterministic if
 * use the same seed, However, if seed<=0 then will "randomly"
 * choose one.
 */
int avec_randomize(avec const *ha, int seed) {
	char *swap;
	char **vec, **ptr1, **ptr2;
	int num, i;

	if (!avec_exists(ha)) {
#  ifdef RKOERROR
		(void) rkopsterror("avec_randomize : ");
		rkoerrno = RKOUSEERR;
		return -1;
#  endif
	}
	if (seed <=0) {
		seed = (int) time(NULL);
	}
	if (seed < 0) {
#  ifdef RKOERROR
		(void) rkocpyerror("avec_randomize : seed making error!");
		rkoerrno = RKOUSEERR;
		return -2;
#  endif
	}
	setseed((INTEGER) seed);
	num = avec_number(ha);
	vec = avec_vector(ha);
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
