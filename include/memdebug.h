#ifndef _MEMDEBUG_H_
#  define _MEMDEBUG_H_
/* 
 * RCSID @(#)$Id: memdebug.h,v 1.1 2002/02/08 23:10:22 rk Exp $
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
/* define HAVE_STRCASECMP if function exists
 */
#  ifdef __cplusplus
extern "C" {
#  endif

/* memory - provides a front-end for the memory allocation routines to
 *	help find memory leaks.  Supports only the ANSI-C routines:
 *		  calloc,   free,   malloc,   realloc
 *	call with the following names:
 *		m_calloc, m_free, m_malloc, m_realloc
 *	the output goes to stderr.
 */

#  ifdef MEMDEBUG

#    include <stdlib.h>	/* calloc, free, malloc, realloc */

void *m_calloc(size_t nelem, size_t size, char *file, int line);
void m_free(void *ptr, char *file, int line);
void *m_malloc(size_t size, char *file, int line);
void *m_realloc(void *rptr, size_t size, char *file, int line);

#    define   calloc(a,b)	m_calloc((a),(b),	__FILE__, __LINE__)
#    define   free(a)		m_free((a),		__FILE__, __LINE__)
#    define   malloc(a)		m_malloc((a),		__FILE__, __LINE__)
#    define   realloc(a,b)	m_realloc((a),(b),	__FILE__, __LINE__)

#  ifdef __cplusplus
	}
#  endif
#endif /* _MEMDEBUG_H_ */
