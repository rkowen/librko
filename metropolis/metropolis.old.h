/* AUTHOR "metropolis.h 02/11/93 1.0 R.K.Owen,PhD" */
/*
 * metropolis.h - the include file for the metropolis set of functions
 *
 * author	R.K.Owen,Ph.D.	02/11/93
 */
/* 
 *     This software is copyrighted by R.K.Owen,Ph.D. 1996
 * 
 * The author, R.K.Owen, of this software is not liable for any
 * problems WHATSOEVER which may result from use  or  abuse  of
 * this  software.  The author, R.K.Owen, grants limited rights
 * to anyone who uses and modifies this  software  for  private
 * non-commercial  use  as  long  as  this copyright and rights
 * notice remains in this software and is made available to all
 * recipients of this software.
 * 
 * Anyone  who  includes this software in a commercial package,
 * or intends  to,  must  contact  the  author,  R.K.Owen,  and
 * license this software.
 *
 * last known email: rkowen@ckns.net
 *                   rk@owen.sj.ca.us
 */

#ifndef _METROPOLIS_H_
#  define _METROPOLIS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* user defined precision for those who don't want the default double */
/* However, you may need to define a exp( ) replacement.
 *
 * The following will use the <math.h> double exp( ) and create a cast
 * to (double) for the argument, if the "exp" macro is not defined.
 */
#ifndef _PRECISION_
#  define _PRECISION_
typedef double PRECISION;
#else
#  ifndef exp
#    define exp(a)	exp((double) a)
#  endif
#endif

#define Mpls_RAN_INDEX		1
#define Mpls_RAN_SWAP		2
#define Mpls_RAN_PERMUTE	4
#define Mpls_RAN_REVERSE	8
#define Mpls_FIX_INITIAL	16
#define Mpls_FIX_FINAL		32

typedef enum  {_NONE, _SWAP, _PERM, _REVR} swaptype;

int metropolis(int flag, int maxstep, int indexdim, int *index,
		PRECISION (*cost)(int,int*,swaptype,int,int),
		PRECISION (*temp)(int,int),
		PRECISION (*ranfn)(void));

int ranindex(int low, int high, int indexdim, int *index,
		PRECISION (*ranfn)(void));

void ranswap(int low, int high, int *swap1, int *swap2,
		PRECISION (*ranfn)(void));

void ranperm(int low, int high, int *cut,
		PRECISION (*ranfn)(void));

void idxswap(int *index, int swap1, int swap2);

void idxperm(int low, int high, int *index, int *work, int cut);

void idxrevr(int *index, int cut1, int cut2);

#ifdef __cplusplus
}
#endif

#endif /* _METROPOLIS_H_ */
