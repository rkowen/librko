static const char RCSID[]="@(#)$Id: postfilter.c,v 1.2 2003/08/25 18:12:50 rk Exp $";
static const char AUTHOR[]="@(#)coexec 1.0 2002/02/28 R.K.Owen,Ph.D.";
/** ** Copyright *********************************************************** **
 ** 									     **
 ** Copyright 2001 by R.K.Owen,Ph.D.		                      	     **
 ** last known email: librko@kooz.sj.ca.us				     **
 **                   rk@owen.sj.ca.us					     **
 ** see LICENSE.LGPL, which must be provided, for details		     **
 ** 									     **
 ** ************************************************************************ **/
#define _POSIX_SOURCE

#include <unistd.h>	/* STDIN_FILENO */
#include <stdio.h>	/* FILE */
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#include "coexec.h"
#ifdef RKOERROR
#  include "rkoerror.h"
#endif

/*----------------------------------------------------------------------*/
/* postfilter.c -
 *	filters the stdout (similar to popen(,"w"))
 *	returns FILE * of child input stream if successful,
 *	else NULL if not.
 *	Calls postfilter().
 */

FILE *postfilter(FILE *outstream, char ** argv) {
	return postprocess(outstream, argv, STDIN_FILENO);
}

