/* urand.h - header file */

#ifndef _URAND_H_
#  define _URAND_H_

#ifdef __cplusplus
extern "C" {
#endif

/*      URAND IS A UNIFORM RANDOM NUMBER GENERATOR BASED  ON  THEORY  AND
 *  SUGGESTIONS  GIVEN  IN  D.E. KNUTH (1969),  VOL  2.   THE INTEGER  IY
 *  SHOULD BE INITIALIZED TO AN ARBITRARY INTEGER PRIOR TO THE FIRST CALL
 *  TO URAND.  THE CALLING PROGRAM SHOULD  NOT  ALTER  THE  VALUE  OF  IY
 *  BETWEEN  SUBSEQUENT CALLS TO URAND.  VALUES OF URAND WILL BE RETURNED
 *  IN THE INTERVAL (0,1).
 *
 *  C version ran through f2c and cleaned up by R.K.Owen,Ph.D. 02/12/93
 *  with many features added.
 */
/*
 *  irand()	- returns INTEGER   in range [0,IRAND_MAX]
 *  urand()	- returns PRECISION in range [0,1.0)
 *  setseed(INTEGER X)	- sets the seed to X
 *  getseed()	- gets the current seed
 *  getirand()	- returns last value returned from irand() (if no urand calls)
 *  geturand()	- returns last value returned from urand() (if no irand calls)
 *
 *  extern INTEGER IRAND_MAX	- the maximum value returned by irand
 *				  it's only defined after a call to
 *				  urand(), irand(), or setseed().
 */
/** ** Copyright *********************************************************** **
 ** 									     **
 ** Copyright 1996 by R.K.Owen,Ph.D.		                      	     **
 ** last known email: librko@kooz.sj.ca.us				     **
 **                   rk@owen.sj.ca.us					     **
 ** see LICENSE.LGPL, which must be provided, for details		     **
 ** 									     **
 ** ************************************************************************ **/

#  ifndef _INTEGER_
#    define _INTEGER_
typedef int INTEGER;
#  endif

#  ifndef _PRECISION_
#    define _PRECISION_
typedef double PRECISION;
#  endif

/* declarations */
INTEGER irand(void);
PRECISION urand(void);
void setseed(INTEGER ity);
INTEGER getseed(void);
INTEGER getirand(void);
PRECISION geturand(void);

extern INTEGER IRAND_MAX;

#ifdef __cplusplus
}
#endif

#endif /* _URAND_H_ */
