#ifndef _COEXEC_H_
#  define _COEXEC_H_
/* 
 * RCSID @(#)$Id: coexec.h,v 1.1 2002/03/01 05:23:28 rk Exp $
 */
/*
 *********************************************************************
 *
 *     This software is copyrighted by R.K.Owen,Ph.D. 2002
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

#  include <stdio.h>	/* FILE */

FILE *preprocess(FILE *instream, char **argv, int fd);
FILE *postprocess(FILE *outstream, char **argv, int fd);

FILE *prefilter(FILE *instream, char **argv);
FILE *postfilter(FILE *outstream, char **argv);
int   closefilter(FILE *filterstream);

FILE *invoke(char **argv);
int   spawn(FILE **childin, FILE **childout, char **argv);

#  ifdef __cplusplus
	}
#  endif
#endif /* _COEXEC_H_ */
