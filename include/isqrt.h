#ifndef _ISQRT_H_
#  define _ISQRT_H_
/* 
 * RCSID @(#)$Id: isqrt.h,v 1.1 2002/02/08 23:10:22 rk Exp $
 */
/*
 *********************************************************************
 *
 *     This software is copyrighted by R.K.Owen,Ph.D. 2002
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
