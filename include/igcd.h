#ifndef _GCD_H_
#  define _GCD_H_
/* 
 * RCSID @(#)$Id: igcd.h,v 1.2 2002/06/27 20:32:02 rk Exp $
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
