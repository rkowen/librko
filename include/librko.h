#ifndef _LIBRKO_H_
#  define _LIBRKO_H_
/* 
 * RCSID @(#)$Id: librko.h,v 1.22 2002/02/27 22:29:53 rk Exp $
 */
/*
 *********************************************************************
 *
 *     This software is copyrighted by R.K.Owen,Ph.D. 1997
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
/* define NO_STRCASECMP if function doesn't exist
 */
#  if 0
#    define NO_STRCASECMP
#  endif

#  include <stdio.h>
#  include <stdarg.h>
#  include <time.h>		/* clock_t */
#  include <netdb.h>		/* gethostbyname, gethostbyaddr */

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

#  include "memdebug.h"
#  include "rkoerror.h"
#  include "clocker.h"
#  include "gcd.h"
#  include "iprime.h"
#  include "isqrt.h"
#  include "istext.h"
#  include "strchop.h"
#  include "strmalloc.h"
#  include "list.h"
#  include "uvec.h"
#  include "avec.h"
#  include "urand.h"

FILE * invoke(char ** argv);
int    spawn(FILE **childin, FILE **childout, char ** argv);
FILE * prefilter(FILE *instream, char ** argv);
FILE * postfilter(FILE *outstream, char ** argv);
int closefilter(FILE *filterstream);
int timedfgets(char *buf, int size, FILE *stream, int seconds);
struct hostent *gethostbyX(char *hostX);
int tcp_connect(char *hostname, int port_num);
int tcp_close(int sd);
int dirtree(int sort, int dirlvl, int lnklvl, const char *dir,
	int (dirfn)(const char *), int (filefn)(const char *),
	int (direnter)(const char *), int (dirleave)(const char *));

int strmemalloc(char **ChArSpAcE, char ***charspace,
		size_t numstr, size_t maxstrlen);
void strmemfree(char **ChArSpAcE, char ***charspace);
int wcstrcmp(const char *str, const char *wcstr);
char *macstrncpy(char *string, const char *mstring, size_t n);
extern char *MacroStr[];
extern int MacroTrf[];
#define NAMETOL(a,b)	nametol(a,#b,&b)
#define NAMETOD(a,b)	nametod(a,#b,&b)
int nametol(const char *line, const char *name, long *value);
int nametod(const char *line, const char *name, double *value);
PRECISION foptim(int iopt, PRECISION ax, PRECISION bx,
	PRECISION (*f)(PRECISION), PRECISION tol);
PRECISION fzeroin(PRECISION ax, PRECISION bx,
	PRECISION y0, PRECISION (*f)(PRECISION), PRECISION tol);

/* metropolis declarations */
/* user defined precision - those who don't want the default double
 * may need to define a exp( ) replacement.
 *
 * The following will use the <math.h> double exp( ) and create a cast
 * to (double) for the argument, if the "mpls_exp" macro is not defined.
 */
#  ifndef mpls_exp
#    define mpls_exp(a)	exp((double) a)
#  endif

#  define Mpls_RAN_INDEX	1
#  define Mpls_RAN_SWAP		2
#  define Mpls_RAN_PERMUTE	4
#  define Mpls_RAN_REVERSE	8
#  define Mpls_FIX_INITIAL	16
#  define Mpls_FIX_FINAL	32

typedef enum  {_NONE, _SWAP, _PERM, _REVR} swaptype;

int metropolis(int flag, int maxstep, int indexdim, int *index,
		PRECISION (*cost)(int,int*,swaptype,int,int),
		PRECISION (*temp)(int,int),
		PRECISION (*ranfn)(void));
int ranindex(int low, int high, int indexdim, int *index,
		PRECISION (*ranfn)(void));
void ranswap(int low, int high, int *swap1, int *swap2,
		PRECISION (*ranfn)(void));
void ranperm(int low, int high, int *cut, PRECISION (*ranfn)(void));
void idxswap(int *index, int swap1, int swap2);
void idxperm(int low, int high, int *index, int *work, int cut);
void idxrevr(int *index, int cut1, int cut2);

#  ifdef __cplusplus
	}
#  endif
#endif /* _LIBRKO_H_ */
