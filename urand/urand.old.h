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
 * last known email: rk@owen.sj.ca.us
 *                   rkowen@kudonet.com
 *                   smbd89a@prodigy.com
 *
 *********************************************************************
 */

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
