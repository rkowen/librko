#ifndef _IPRIME_H_
#  define _IPRIME_H_
/* 
 * RCSID @(#)$Id: iprime.h,v 1.1 2002/02/08 23:10:22 rk Exp $
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

/* integer prime */
#define __IPRIME(NM, TYPE) TYPE NM(TYPE a);
__IPRIME(chprime,char)
__IPRIME(scprime,signed char)
__IPRIME(ucprime,unsigned char)
__IPRIME(hprime,short)
__IPRIME(uhprime,unsigned short)
__IPRIME(iprime,int)
__IPRIME(uiprime,unsigned int)
__IPRIME(lprime,long)
__IPRIME(ulprime,unsigned long)
#undef __IPRIME

#  ifdef __cplusplus
	}
#  endif
#endif /* _IPRIME_H_ */
