#ifndef _GCD_H_
#  define _GCD_H_
/* 
 * RCSID @(#)$Id: igcd.h,v 1.1 2002/02/08 23:10:22 rk Exp $
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

/* GCD - greatest common divisor */
#define __GCD(NM, TYPE) TYPE NM(TYPE a, TYPE b);
__GCD(chgcd,char)
__GCD(scgcd,signed char)
__GCD(ucgcd,unsigned char)
__GCD(hgcd,short)
__GCD(uhgcd,unsigned short)
__GCD(igcd,int)
__GCD(ugcd,unsigned int)
__GCD(lgcd,long)
__GCD(ulgcd,unsigned long)
#undef __GCD

#  ifdef __cplusplus
	}
#  endif
#endif /* _GCD_H_ */
