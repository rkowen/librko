#ifndef _CLOCKER_H_
#  define _CLOCKER_H_
/* 
 * RCSID @(#)$Id: clocker.h,v 1.2 2002/06/27 20:32:02 rk Exp $
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

#include <time.h>

/* clock timer */
typedef clock_t clocker_t;

typedef   enum {_SET = 0, _RESET, _READ, _PER_SEC} clocker_action;

clocker_t   clocker_tick(clocker_t *clock_variable, clocker_action what_to_do);

double    clocker(clocker_t *clock_variable, clocker_action what_to_do);

#  ifdef __cplusplus
	}
#  endif
#endif /* _CLOCKER_H_ */
