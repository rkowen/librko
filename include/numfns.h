#ifndef _NUMFNS_H_
#  define _NUMFNS_H_
/* 
 * RCSID @(#)$Id: numfns.h,v 1.1 2002/06/30 04:07:13 rk Exp $
 */
/** ** Copyright *********************************************************** **
 ** 									     **
 ** Copyright 2001 by R.K.Owen,Ph.D.		                      	     **
 ** last known email: librko@kooz.sj.ca.us				     **
 **                   rk@owen.sj.ca.us					     **
 ** see LICENSE.LGPL, which must be provided, for details		     **
 ** 									     **
 ** ************************************************************************ **/

#  include "config.h"

#  ifdef __cplusplus
extern "C" {
#  endif

#  ifndef _PRECISION_
#    define _PRECISION_
     typedef double PRECISION;
#  endif

#  ifndef _INTEGER_
#    define _INTEGER_
     typedef int INTEGER;
#  endif


PRECISION foptim(int iopt, PRECISION ax, PRECISION bx,
	PRECISION (*f)(PRECISION), PRECISION tol);
PRECISION fzeroin(PRECISION ax, PRECISION bx,
	PRECISION y0, PRECISION (*f)(PRECISION), PRECISION tol);
int divdiffcoef(int opt, int N, PRECISION *t, PRECISION *g, PRECISION *c);


#  ifdef __cplusplus
	}
#  endif
#endif /* _NUMFNS_H_ */
