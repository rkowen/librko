static char USMID[]="%W%";
static const char RCSID[]="@(#)$Id: memdebug.c,v 1.2 1998/10/14 15:16:10 rk Exp $";
static char AUTHOR[]="@(#)memory 1.0 02/10/95 R.K.Owen,PhD";

/* memory - provides a front-end for the memory allocation routines to
 *	help find memory leaks.  Supports only the ANSI-C routines:
 *		  calloc,   free,   malloc,   realloc
 *	call with the following names:
 *		m_calloc, m_free, m_malloc, m_realloc
 *	the output goes to stderr.
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
 * last known email: rkowen@ckns.net
 *                   rk@owen.sj.ca.us
 *
 *********************************************************************
 */

#include <stdlib.h>
#include <stdio.h>

#if 0
#  define TEST		/* stand-alone test program */
#endif

void *m_calloc(size_t nelem, size_t size, char *file, int line) {
	void *ptr;

	ptr = calloc(nelem, size);
	(void) fprintf(stderr,"  calloc : %s %d : %p %d %d\n",
		file, line, ptr, nelem, size);
	return ptr;
}

void m_free(void *ptr, char *file, int line) {
	(void) fprintf(stderr,"    free : %s %d : %p \n",
		file, line, ptr);
	free(ptr);
}

void *m_malloc(size_t size, char *file, int line) {
	void *ptr;

	ptr = malloc(size);
	(void) fprintf(stderr,"  malloc : %s %d : %p %d\n",
		file, line, ptr, size);
	return ptr;
}

void *m_realloc(void *rptr, size_t size, char *file, int line) {
	void *ptr;

	ptr = realloc(rptr, size);
	(void) fprintf(stderr," realloc : %s %d : %p %p %d\n",
		file, line, ptr, rptr, size);
	return ptr;
}

#ifdef TEST

#  ifndef QUIET
#    define  _calloc(a,b)	m_calloc(a,b,	__FILE__, __LINE__)
#    define    _free(a)		m_free(a,	__FILE__, __LINE__)
#    define  _malloc(a)		m_malloc(a,	__FILE__, __LINE__)
#    define _realloc(a,b)	m_realloc(a,b,	__FILE__, __LINE__)
#  else
#    define  _calloc(a,b)	calloc(a,b)
#    define    _free(a)		free(a)
#    define  _malloc(a)		malloc(a)
#    define _realloc(a,b)	realloc(a,b)
#  endif

typedef struct Trial {
	int a;
	double b;
	char c[5];
	struct Trial *next;
} trial;

int main() {

	size_t num = 20;
	int *ia;
	double *da;
	char *ca;
	trial *ta;

	if ((ia = (int *) _malloc(num * sizeof(int))) == NULL)
		(void) fprintf(stderr,"malloc error\n");
	_free(ia);

	if ((ia = (int *) _malloc(2 * num * sizeof(int))) == NULL)
		(void) fprintf(stderr,"malloc error\n");
	if ((ia = (int *) _realloc(ia, 4 * num * sizeof(int))) == NULL)
		(void) fprintf(stderr,"realloc error\n");

	if ((da = (double *) _malloc(num * sizeof(double))) == NULL)
		(void) fprintf(stderr,"malloc error\n");

	if ((ta = (trial *) _calloc(num, sizeof(trial))) == NULL)
		(void) fprintf(stderr,"malloc error\n");
	_free(ta);
	if ((ta = (trial *) _calloc(2 * num, sizeof(trial))) == NULL)
		(void) fprintf(stderr,"malloc error\n");

	if ((da = (double *) _realloc(da, 2 * num * sizeof(double))) == NULL)
		(void) fprintf(stderr,"realloc error\n");
	_free(ia);

	if ((ca = (char *) _malloc(num * sizeof(char))) == NULL)
		(void) fprintf(stderr,"malloc error\n");

	_free(da);
	_free(ta);
	_free(ca);
	_free(ia);

	return 0;
}
#endif /* TEST */
