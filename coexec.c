static const char USMID[]="%W%";
static const char RCSID[]="@(#)$Id: coexec.c,v 1.1 1998/10/07 16:19:56 rk Exp $";
static const char AUTHOR[]="@(#)prefilter 1.0 03/26/96 R.K.Owen,Ph.D.";
/* prefilter.c - routine for fork/exec/dup/pipe a child process
 *	that filters the stdin (similar to popen(,"a"))
 *	returns FILE * of child output stream if successful,
 *	else NULL if not.
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
 * last known email: rkowen@kudonet.com
 *                   rk@owen.sj.ca.us
 *
 *********************************************************************
 */
#define _POSIX_SOURCE

#include <stdio.h>
#include <stdlib.h>	/* _exit */
#include <sys/types.h>
#include <unistd.h>	/* fork,exec,dup */
#include <signal.h>	/* kill */

char buffer[256];

FILE * prefilter(FILE *instream, char ** argv) {
	int pipepath[2];
	int parent_rfd, child_wfd, in_fd;
	int child_pid;
	FILE * returnfile = (FILE *) NULL;

	/* open pipe for one-way communication */
	if(pipe(pipepath) < 0) {
		(void) sprintf(buffer,"prefilter: failed to pipe with '%s'",
			argv[0]);
		perror(buffer);
		return (FILE *) NULL;
	}
	/* for clarity only */
	parent_rfd = pipepath[0];
	child_wfd = pipepath[1];

	/* fork process */
	switch(child_pid = fork()) {
	case -1:				/* fork failure */
		(void) sprintf(buffer,"prefilter: failed to fork with '%s'",
			argv[0]);
		perror(buffer);
		(void) close(parent_rfd);
		(void) close(child_wfd);
		return (FILE *) NULL;

	case 0:					/* child */
		if (close(parent_rfd) < 0) {
			(void) sprintf(buffer,
				"prefilter: failed to close readpipe with '%s'",
				argv[0]);
			goto child_failure;
		}
		if (dup2(child_wfd,STDOUT_FILENO) < 0) {
			(void) sprintf(buffer,
				"prefilter: failed to dup2 stdout with '%s'",
				argv[0]);
			goto child_failure;
		}
		if (execvp(argv[0],argv) < 0) {
			(void) sprintf(buffer,
				"prefilter: failed to execvp with '%s'",
				argv[0]);
			goto child_failure;
		}
		(void) sprintf(buffer,
			"prefilter: should not 'get here' with '%s'",
			argv[0]);
		child_failure: {
			perror(buffer);
			_exit(1);
		}

	default:				/* parent */
		if (close(child_wfd) < 0) {
			(void) sprintf(buffer,
				"prefilter: failed to close writepipe with '%s'",
				argv[0]);
			goto parent_failure;
		}
		if ((in_fd = fileno(instream)) < 0) {
			(void) sprintf(buffer,
				"prefilter: failed to fileno stream with '%s'",
				argv[0]);
			goto parent_failure;
		}
		if (dup2(parent_rfd,in_fd) < 0) {
			(void) sprintf(buffer,
				"prefilter: failed to dup2 steam with '%s'",
				argv[0]);
			goto parent_failure;
		}
		if((returnfile = fdopen(parent_rfd, "r")) == (FILE *) NULL) {
			(void) sprintf(buffer,
				"prefilter: failed to fdopen readpipe with '%s'",
				argv[0]);
			goto parent_failure;
		}
		if (0) parent_failure: {
			(void) close(child_wfd);
			(void) close(parent_rfd);
			perror(buffer);
			/* kill the boy... */
			(void) kill(child_pid,SIGKILL);
		}
		/* don't wait for child */
		return returnfile;
	}
}
