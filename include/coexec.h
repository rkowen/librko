#ifndef _COEXEC_H_
#  define _COEXEC_H_
/* 
 * RCSID @(#)$Id: coexec.h,v 1.3 2002/10/21 17:17:04 rk Exp $
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

#  include <stdio.h>	/* FILE */

FILE *preprocess(FILE *instream, char **argv, int fd);
FILE *postprocess(FILE *outstream, char **argv, int fd);

FILE *prefilter(FILE *instream, char **argv);
FILE *postfilter(FILE *outstream, char **argv);
int   closefilter(FILE *filterstream);

FILE *invoke(char **argv);
int   spawn( FILE **childin, FILE **childout, char **argv);
int   spawn3(FILE **childin, FILE **childout, FILE **childerr, char **argv);

#  ifdef __cplusplus
	}
#  endif
#endif /* _COEXEC_H_ */
