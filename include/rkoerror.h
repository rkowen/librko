#ifndef _RKOERROR
#  define _RKOERROR
/* 
 * RCSID @(#)$Id: rkoerror.h,v 1.3 2002/06/27 20:32:02 rk Exp $
 */
/** ** Copyright *********************************************************** **
 ** 									     **
 ** Copyright 2001 by R.K.Owen,Ph.D.		                      	     **
 ** last known email: librko@kooz.sj.ca.us				     **
 **                   rk@owen.sj.ca.us					     **
 ** see LICENSE.LGPL, which must be provided, for details		     **
 ** 									     **
 ** ************************************************************************ **/

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
