static const char RCSID[]="@(#)$Id: preprocess.c,v 1.1 2002/06/30 04:07:13 rk Exp $";
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
/* preprocess.c - routine for fork/exec/dup/pipe a child process
 *	that filters the given filedesciptor fd for the child process
 *	(similar to popen(,"r"))
 *	returns FILE * of child fd output stream if successful,
 *	else NULL if not.
 */

FILE *preprocess(FILE *instream, char ** argv, int fd) {
	int pipepath[2];
	int parent_rfd, child_wfd, in_fd;
	int child_pid;
	FILE * returnfile = (FILE *) NULL;
	char buffer[256];

	/* fd must be >= 0 */
	if (fd < 0) {
		strcpy(buffer,"prefilter: invalid fd '");
		sprintf(buffer + strlen(buffer),"%d", fd);
		strcat(buffer,"' with '");
		strncat(buffer,argv[0],100);
		strcat(buffer,"'");
#ifdef RKOERROR
		(void ) rkocpyerror(buffer);
#else
			perror(buffer);
#endif
		return (FILE *) NULL;
	}
	/* open pipe for one-way communication */
	if(pipe(pipepath) < 0) {
		strcpy(buffer,"prefilter: failed to pipe with '");
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
		strcpy(buffer,"prefilter: failed to fork with '");
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
				"prefilter: failed to close readpipe with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto child_failure;
		}
		if (dup2(child_wfd,fd) < 0) {
			strcpy(buffer,
				"prefilter: failed to dup2 fd '");
			sprintf(buffer + strlen(buffer),"%d", fd);
			strcat(buffer,"' with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto child_failure;
		}
		if (execvp(argv[0],argv) < 0) {
			strcpy(buffer, "prefilter: failed to execvp with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto child_failure;
		}
		strcpy(buffer, "prefilter: should not 'get here' with '");
		strncat(buffer,argv[0],100);
		strcat(buffer,"'");
		child_failure: {
#ifdef RKOERROR
		(void ) rkocpyerror(buffer);
			rkoperror("");
#else
			perror(buffer);
#endif
			_exit(1);
		}

	default:				/* parent */
		if (close(child_wfd) < 0) {
			strcpy(buffer,
				"prefilter: failed to close writepipe with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto parent_failure;
		}
		if ((in_fd = fileno(instream)) < 0) {
			strcpy(buffer,
				"prefilter: failed to fileno stream with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto parent_failure;
		}
		if (dup2(parent_rfd,in_fd) < 0) {
			strcpy(buffer,
				"prefilter: failed to dup2 stream with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto parent_failure;
		}
		if((returnfile = fdopen(parent_rfd, "r")) == (FILE *) NULL) {
			strcpy(buffer,
				"prefilter: failed to fdopen readpipe with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto parent_failure;
		}
		if(setvbuf(returnfile, (char *) NULL, _IOLBF, 0)) {
			strcpy(buffer,
		"prefilter: parent failed to readpipe line buffer with '");
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

