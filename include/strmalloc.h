#ifndef _STRMALLOC_H_
#  define _STRMALLOC_H_
/* 
 * RCSID @(#)$Id: strmalloc.h,v 1.1 2002/02/08 16:26:01 rk Exp $
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

#include <string.h>

#  ifdef __cplusplus
extern "C" {
#  endif

char *strnmalloc(char const *in, size_t n);
char *strmalloc(char const *in);
int strfree(char **str);

#  ifdef __cplusplus
	}
#  endif
#endif /* _STRMALLOC_H_ */
