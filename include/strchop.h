#ifndef _STRCHOP_H_
#  define _STRCHOP_H_
/* 
 * RCSID @(#)$Id: strchop.h,v 1.1 2002/02/08 23:10:22 rk Exp $
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
#  ifdef __cplusplus
extern "C" {
#  endif

#include <stdlib.h>	/* size_t */

char *strchop(const char *string, int chop_len,
        size_t *str_len, char **next);

#  ifdef __cplusplus
	}
#  endif
#endif /* _UVEC_H_ */
