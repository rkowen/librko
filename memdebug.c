static const char RCSID[]="@(#)$Id: memdebug.c,v 1.6 2002/02/21 18:05:34 rk Exp $";
static const char AUTHOR[]="@(#)memdebug 1.0 02/10/95 R.K.Owen,PhD";

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
 * last known email: librko@kooz.sj.ca.us
 *                   rk@owen.sj.ca.us
 *
 *********************************************************************
 */

#include <stdlib.h>
#include <stdio.h>

FILE *m_output;

FILE *memdebug_output(FILE *output) {

	if (!m_output) m_output = stderr;

	if (output)
		m_output = output;

	return m_output;
}

void *m_calloc(size_t nelem, size_t size, char *file, int line) {
	void *ptr;
	char *null = "(null)";

	if (!m_output) m_output = stderr;

	ptr = calloc(nelem, size);
	if (ptr != NULL)
		(void) fprintf(m_output,"RKOMEM:         calloc : %s %d :"
			" %p %d %d\n",
			file, line, ptr, nelem, size);
	else
		(void) fprintf(m_output,"RKOMEM:         calloc : %s %d :"
			" %s %d %d\n",
			file, line, null, nelem, size);
	return ptr;
}

void m_free(void *ptr, char *file, int line) {
	char *null = "(null)";

	if (!m_output) m_output = stderr;

	if (ptr != NULL)
		(void) fprintf(m_output,"RKOMEM:         free   : %s %d : %p\n",
			file, line, ptr);
	else
		(void) fprintf(m_output,"RKOMEM:         free   : %s %d : %s\n",
			file, line, null);
	free(ptr);
}

void *m_malloc(size_t size, char *file, int line) {
	void *ptr;
	char *null = "(null)";

	if (!m_output) m_output = stderr;

	ptr = malloc(size);
	if (ptr != NULL)
		(void) fprintf(m_output,"RKOMEM:         malloc : %s %d :"
			" %p %d\n",
			file, line, ptr, size);
	else
		(void)fprintf(m_output,"RKOMEM:         malloc : %s %d :"
		" %s %d\n",
			file, line, null, size);
	return ptr;
}

void *m_realloc(void *rptr, size_t size, char *file, int line) {
	void *ptr;
	char *null = "(null)";

	if (!m_output) m_output = stderr;

	ptr = realloc(rptr, size);
	if (rptr != NULL)
		(void) fprintf(m_output,"RKOMEM: realloc_free   : %s %d : %p\n",
		file, line, rptr);

	if (size) {
		(void) fprintf(m_output,"RKOMEM: realloc_malloc : %s %d :",
			file, line);
		if (ptr != NULL)
			(void) fprintf(m_output," %p %d\n", ptr, size);
		else
			(void) fprintf(m_output," %s %d\n", null, size);
	}
	return ptr;
}
