#ifndef _DIVDIFF_H_
#  define _DIVDIFF_H_
/*
 * Header file for the Divided Difference Routines
 *
 * by R.K.Owen,Ph.D.  05/12/94
 */
/*
 *********************************************************************
 *
 *     This software is copyrighted by R.K.Owen,Ph.D. 1996
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

/*
 * if _PRECISION_ is not defined then default to double
 */
#  ifndef _PRECISION_
typedef double PRECISION;
#  endif

#  ifdef __cplusplus
extern "C" {
#  endif

int DivDiffCoef(int opt, int N, PRECISION *t, PRECISION *g, PRECISION *c);
PRECISION DivDiffEval(int N, PRECISION *t, PRECISION *c, PRECISION x);

#  ifdef __cplusplus
}
#  endif

#endif /* _DIVDIFF_H_ */
