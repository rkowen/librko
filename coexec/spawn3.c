static const char RCSID[]="@(#)$Id: spawn3.c,v 1.1 2002/10/21 17:17:04 rk Exp $";
static const char AUTHOR[]="@(#)coexec 1.0 2002/02/28 R.K.Owen,Ph.D.";
/* coexec.c - contains all the co-executable routines
 */
/** ** Copyright *********************************************************** **
 ** 									     **
 ** Copyright 2002 by R.K.Owen,Ph.D.		                      	     **
 ** last known email: librko@kooz.sj.ca.us				     **
 **                   rk@owen.sj.ca.us					     **
 ** see LICENSE.LGPL, which must be provided, for details		     **
 ** 									     **
 ** ************************************************************************ **/
#define _POSIX_SOURCE

#include <stdio.h>	/* perror */
#include <stdlib.h>	/* _exit */
#include <string.h>	/* strcpy, strcat, etc */
#include <sys/types.h>
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
/* spawn3.c - routine for fork/exec/dup/pipe a child process and tie its
 *	input/output/stderr back to be written/read/read by the caller.
 *	returns the child pid (>0) if successful, else < 0 if error.
 */

int spawn3(FILE **childin, FILE **childout, FILE **childerr, char **argv) {
	int pipepath1[2];
	int pipepath2[2];
	int pipepath3[2];
	int parent_rfd, parent_wfd, parent_efd;
	int child_rfd,  child_wfd, child_efd;
	int child_pid = 0;
	char buffer[256];


	/* open pipes for two one-way communication */
	if(pipe(pipepath1) < 0 || pipe(pipepath2) < 0 || pipe(pipepath3) < 0) {
		strcpy(buffer, "spawn3: failed to pipe with '");
		strncat(buffer,argv[0],100);
		strcat(buffer,"'");
#ifdef RKOERROR
		(void ) rkocpyerror(buffer);
#else
			perror(buffer);
#endif
		return -1;
	}
	/* for clarity only */
	parent_rfd = pipepath1[0];
	child_wfd = pipepath1[1];
	child_rfd = pipepath2[0];
	parent_wfd = pipepath2[1];
	parent_efd = pipepath3[0];
	child_efd = pipepath3[1];

	/* fork process */
	switch(child_pid = fork()) {
	case -1:				/* fork failure */
		strcpy(buffer, "spawn3: failed to fork with '");
		strncat(buffer,argv[0],100);
		strcat(buffer,"'");
#ifdef RKOERROR
		(void ) rkocpyerror(buffer);
#else
			perror(buffer);
#endif
		(void) close(parent_rfd);
		(void) close(child_wfd);
		(void) close(child_rfd);
		(void) close(parent_wfd);
		(void) close(parent_efd);
		(void) close(child_efd);
		return -2;

	case 0:					/* child */
		if (close(parent_rfd) < 0 || close(parent_wfd) < 0
		|| (parent_efd) < 0) {
			strcpy(buffer,
				"spawn3: failed to close pipes with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto child_failure;
		}
		if (dup2(child_rfd, STDIN_FILENO) < 0) {
			strcpy(buffer,
				"spawn3: child failed to dup2 stdin with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto child_failure;
		}
		if (dup2(child_wfd, STDOUT_FILENO) < 0) {
			strcpy(buffer,
				"spawn3: child failed to dup2 stdout with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto child_failure;
		}
		if (dup2(child_efd, STDERR_FILENO) < 0) {
			strcpy(buffer,
				"spawn3: child failed to dup2 stderr with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto child_failure;
		}
		if (child_rfd != STDIN_FILENO) {
			if(close(child_rfd) < 0) {
				strcpy(buffer,
				"spawn3: child failed to close in pipe to '");
				strncat(buffer,argv[0],100);
				strcat(buffer,"'");
				goto child_failure;
			}
		}
		if (child_wfd != STDOUT_FILENO) {
			if(close(child_wfd) < 0) {
				strcpy(buffer,
				"spawn3: child failed to close out pipe to '");
				strncat(buffer,argv[0],100);
				strcat(buffer,"'");
				goto child_failure;
			}
		}
		if (child_wfd != STDERR_FILENO) {
			if(close(child_efd) < 0) {
				strcpy(buffer,
				"spawn3: child failed to close err pipe to '");
				strncat(buffer,argv[0],100);
				strcat(buffer,"'");
				goto child_failure;
			}
		}
		if (execvp(argv[0],argv) < 0) {
			strcpy(buffer, "spawn3: child failed to execvp with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto child_failure;
		}
		strcpy(buffer, "spawn3: should not 'get here' with '");
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
		if (close(child_wfd) < 0 || close(child_rfd) < 0) {
			strcpy(buffer,
			"spawn3: parent failed to close pipes with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto parent_failure;
		}
		if((*childout = fdopen(parent_rfd, "r")) == (FILE *) NULL) {
			strcpy(buffer,
			"spawn3: parent failed to fdopen readpipe with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto parent_failure;
		}
		if((*childerr = fdopen(parent_efd, "r")) == (FILE *) NULL) {
			strcpy(buffer,
			"spawn3: parent failed to fdopen errorpipe with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto parent_failure;
		}
		if((*childin = fdopen(parent_wfd, "w")) == (FILE *) NULL) {
			strcpy(buffer,
			"spawn3: parent failed to fdopen writepipe with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto parent_failure;
		}
		if(setvbuf(*childout, (char *) NULL, _IOLBF, 0)) {
			strcpy(buffer,
			"spawn3: parent failed to readpipe line buffer with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto parent_failure;
		}
		if(setvbuf(*childerr, (char *) NULL, _IOLBF, 0)) {
			strcpy(buffer,
			"spawn3: parent failed to errpipe line buffer with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto parent_failure;
		}
		if(setvbuf(*childin, (char *) NULL, _IOLBF, 0)) {
			strcpy(buffer,
			"spawn3: parent failed to writepipe line buffer with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto parent_failure;
		}
		if (0) parent_failure: {
			(void) close(child_wfd);
			(void) close(parent_rfd);
			(void) close(child_rfd);
			(void) close(parent_wfd);
			(void) close(parent_efd);
			(void) close(child_efd);
#ifdef RKOERROR
		(void ) rkocpyerror(buffer);
#else
			perror(buffer);
#endif
			/* kill the boy... */
			(void) kill(child_pid,SIGKILL);
		}
		/* don't wait for child */
		return child_pid;
	}
}
