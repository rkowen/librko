#ifndef _RKOERROR
#  define _RKOERROR
/* 
 * RCSID @(#)$Id: rkoerror.h,v 1.1 2002/02/08 16:26:01 rk Exp $
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
/* define NO_STRCASECMP if function doesn't exist
 */

#  ifdef __cplusplus
extern "C" {
#  endif

/* librko - error message package */

extern int rkoerrno;

#define	RKO_OK		0
#define	RKOGENERR	1
#define	RKOSIGERR	2
#define	RKOSIGNAL	3
#define	RKOMEMERR	4
#define	RKOIOERR	5
#define	RKOUSEERR	6
#define	RKO_MSG		99

void rkoperror(const char *s);
char *rkostrerror(void);
char *rkocpyerror(const char *s);
char *rkocaterror(const char *s);
char *rkopsterror(const char *s);

#  ifdef __cplusplus
	}
#  endif
#endif /* _RKOERROR_H_ */
