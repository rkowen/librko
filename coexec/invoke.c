static const char RCSID[]="@(#)$Id: invoke.c,v 1.1 2002/06/30 04:07:13 rk Exp $";
static const char AUTHOR[]="@(#)coexec 1.0 2002/02/28 R.K.Owen,Ph.D.";
/* coexec.c - contains all the co-executable routines
 */
/** ** Copyright *********************************************************** **
 ** 									     **
 ** Copyright 2001 by R.K.Owen,Ph.D.		                      	     **
 ** last known email: librko@kooz.sj.ca.us				     **
 **                   rk@owen.sj.ca.us					     **
 ** see LICENSE.LGPL, which must be provided, for details		     **
 ** 									     **
 ** ************************************************************************ **/
#define _POSIX_SOURCE

#include <stdio.h>	/* perror */
#include <stdlib.h>	/* _exit */
#include <string.h>	/* strcpy, strcat, etc */
#include <unistd.h>	/* fork,exec,dup */
#include <signal.h>	/* kill */
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#include "coexec.h"
#ifdef RKOERROR
#  include "rkoerror.h"
#endif

/*----------------------------------------------------------------------*/
/* invoke.c - routine for fork/exec/dup/pipe a child process and send its
 *	output back to be read by the caller
 *	returns FILE * of child output stream if successful,
 *	else NULL if not.
 */

FILE *invoke(char ** argv) {
	int pipepath[2];
	int parent_rfd, child_wfd;
	int child_pid;
	FILE * returnfile = (FILE *) NULL;
	char buffer[256];

	/* open pipe for one-way communication */
	if(pipe(pipepath) < 0) {
		strcpy(buffer, "invoke: failed to pipe with '");
		strncat(buffer,argv[0],100);
		strcat(buffer,"'");
#ifdef RKOERROR
		(void ) rkocpyerror(buffer);
#else
			perror(buffer);
#endif
		return (FILE *) NULL;
	}
	/* for clarity only */
	parent_rfd = pipepath[0];
	child_wfd = pipepath[1];

	/* fork process */
	switch(child_pid = fork()) {
	case -1:				/* fork failure */
		strcpy(buffer, "invoke: failed to fork with '");
		strncat(buffer,argv[0],100);
		strcat(buffer,"'");
#ifdef RKOERROR
		(void ) rkocpyerror(buffer);
#else
			perror(buffer);
#endif
		(void) close(parent_rfd);
		(void) close(child_wfd);
		return (FILE *) NULL;

	case 0:					/* child */
		if (close(parent_rfd) < 0) {
			strcpy(buffer,
				"invoke: failed to close readpipe with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto child_failure;
		}
		if (close(STDIN_FILENO) < 0) {
			strcpy(buffer,
				"invoke: can't close child's stdin with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto child_failure;
		}
		if (dup2(child_wfd,STDOUT_FILENO) < 0) {
			strcpy(buffer, "invoke: failed to dup2 stdout with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto child_failure;
		}
		if (execvp(argv[0],argv) < 0) {
			strcpy(buffer, "invoke: failed to execvp with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto child_failure;
		}
		strcpy(buffer, "invoke: should not 'get here' with '");
		strncat(buffer,argv[0],100);
		strcat(buffer,"'");
		child_failure: {
#ifdef RKOERROR
		(void ) rkocpyerror(buffer);
#else
			perror(buffer);
#endif
			_exit(1);
		}

	default:				/* parent */
		if (close(child_wfd) < 0) {
			strcpy(buffer,
				"invoke: failed to close writepipe with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto parent_failure;
		}
		if((returnfile = fdopen(parent_rfd, "r")) == (FILE *) NULL) {
			strcpy(buffer,
				"invoke: failed to fdopen readpipe with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto parent_failure;
		}
		if(setvbuf(returnfile, (char *) NULL, _IOLBF, 0)) {
			strcpy(buffer,
			"invoke: parent failed to readpipe line buffer with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto parent_failure;
		}
		if (0) parent_failure: {
			(void) close(child_wfd);
			(void) close(parent_rfd);
#ifdef RKOERROR
		(void ) rkocpyerror(buffer);
#else
			perror(buffer);
#endif
			/* kill the boy... */
			(void) kill(child_pid,SIGKILL);
		}
		/* don't wait for child */
		return returnfile;
	}
}

