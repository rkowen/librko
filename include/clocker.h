#ifndef _CLOCKER_H_
#  define _CLOCKER_H_
/* 
 * RCSID @(#)$Id: clocker.h,v 1.1 2002/02/10 08:02:28 rk Exp $
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
