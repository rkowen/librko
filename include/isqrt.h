#ifndef _ISQRT_H_
#  define _ISQRT_H_
/* 
 * RCSID @(#)$Id: isqrt.h,v 1.2 2002/06/27 20:32:02 rk Exp $
 */
/** ** Copyright *********************************************************** **
 ** 									     **
 ** Copyright 2002 by R.K.Owen,Ph.D.		                      	     **
 ** last known email: librko@kooz.sj.ca.us				     **
 **                   rk@owen.sj.ca.us					     **
 ** see LICENSE.LGPL, which must be provided, for details		     **
 ** 									     **
 ** ************************************************************************ **/
#  ifdef __cplusplus
extern "C" {
#  endif

/* integer sqrt */
#define __ISQRT(NM, TYPE) TYPE NM(TYPE a);
__ISQRT(chsqrt,char)
__ISQRT(scsqrt,signed char)
__ISQRT(ucsqrt,unsigned char)
__ISQRT(hsqrt,short)
__ISQRT(uhsqrt,unsigned short)
__ISQRT(isqrt,int)
__ISQRT(uisqrt,unsigned int)
__ISQRT(lsqrt,long)
__ISQRT(ulsqrt,unsigned long)
#undef __ISQRT

#  ifdef __cplusplus
	}
#  endif
#endif /* _ISQRT_H_ */
