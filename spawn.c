static const char RCSID[]="@(#)$Id: spawn.c,v 1.3 1999/09/09 21:30:44 rk Exp $";
static const char AUTHOR[]="@(#)spawn 1.0 11/06/98 R.K.Owen,Ph.D.";
/* spawn.c - routine for fork/exec/dup/pipe a child process and tie its
 *	input/output back to be written/read by the caller.
 *	returns the child pid (>0) if successful, else < 0 if error.
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
#define _POSIX_SOURCE

#include <stdio.h>
#include <stdlib.h>	/* _exit */
#include <sys/types.h>
#include <unistd.h>	/* fork,exec,dup */
#include <signal.h>	/* kill */

char buffer[256];

int spawn(FILE **childin, FILE **childout, char **argv) {
	int pipepath1[2];
	int pipepath2[2];
	int parent_rfd, parent_wfd;
	int child_rfd,  child_wfd;
	int child_pid = 0;
	FILE * returnfile = (FILE *) NULL;

	/* open pipes for two one-way communication */
	if(pipe(pipepath1) < 0 || pipe(pipepath2) < 0) {
		(void) sprintf(buffer,"spawn: failed to pipe with '%s'",
			argv[0]);
		perror(buffer);
		return -1;
	}
	/* for clarity only */
	parent_rfd = pipepath1[0];
	child_wfd = pipepath1[1];
	child_rfd = pipepath2[0];
	parent_wfd = pipepath2[1];

	/* fork process */
	switch(child_pid = fork()) {
	case -1:				/* fork failure */
		(void) sprintf(buffer,"spawn: failed to fork with '%s'",
			argv[0]);
		perror(buffer);
		(void) close(parent_rfd);
		(void) close(child_wfd);
		(void) close(child_rfd);
		(void) close(parent_wfd);
		return -2;

	case 0:					/* child */
		if (close(parent_rfd) < 0 || close(parent_wfd) < 0) {
			(void) sprintf(buffer,
				"spawn: child failed to close pipes with '%s'",
				argv[0]);
			goto child_failure;
		}
		if (dup2(child_rfd, STDIN_FILENO) < 0) {
			(void) sprintf(buffer,
				"spawn: child failed to dup2 stdin with '%s'",
				argv[0]);
			goto child_failure;
		}
		if (dup2(child_wfd, STDOUT_FILENO) < 0) {
			(void) sprintf(buffer,
				"spawn: child failed to dup2 stdout with '%s'",
				argv[0]);
			goto child_failure;
		}
		if (child_rfd != STDIN_FILENO) {
			if(close(child_rfd) < 0) {
				(void) sprintf(buffer,
				"spawn: child failed to close in pipe to '%s'",
					argv[0]);
				goto child_failure;
			}
		}
		if (child_wfd != STDOUT_FILENO) {
			if(close(child_wfd) < 0) {
				(void) sprintf(buffer,
				"spawn: child failed to close out pipe to '%s'",
					argv[0]);
				goto child_failure;
			}
		}
		if (execvp(argv[0],argv) < 0) {
			(void) sprintf(buffer,
				"spawn: child failed to execvp with '%s'",
				argv[0]);
			goto child_failure;
		}
		(void) sprintf(buffer,
			"spawn: should not 'get here' with '%s'",
			argv[0]);
		child_failure: {
			perror(buffer);
			_exit(1);
		}

	default:				/* parent */
		if (close(child_wfd) < 0 || close(child_rfd) < 0) {
			(void) sprintf(buffer,
				"spawn: parent failed to close pipes with '%s'",
				argv[0]);
			goto parent_failure;
		}
		if((*childout = fdopen(parent_rfd, "r")) == (FILE *) NULL) {
			(void) sprintf(buffer,
				"spawn: parent failed to fdopen readpipe with '%s'",
				argv[0]);
			goto parent_failure;
		}
		if((*childin = fdopen(parent_wfd, "w")) == (FILE *) NULL) {
			(void) sprintf(buffer,
				"spawn: parent failed to fdopen writepipe with '%s'",
				argv[0]);
			goto parent_failure;
		}
		if(setvbuf(*childout, (char *) NULL, _IOLBF, 0)) {
			(void) sprintf(buffer,
				"spawn: parent failed readpipe line buffer with '%s'",
				argv[0]);
			goto parent_failure;
		}
		if(setvbuf(*childin, (char *) NULL, _IOLBF, 0)) {
			(void) sprintf(buffer,
				"spawn: parent failed writepipe line buffer with '%s'",
				argv[0]);
			goto parent_failure;
		}
		if (0) parent_failure: {
			(void) close(child_wfd);
			(void) close(parent_rfd);
			(void) close(child_rfd);
			(void) close(parent_wfd);
			perror(buffer);
			/* kill the boy... */
			(void) kill(child_pid,SIGKILL);
		}
		/* don't wait for child */
		return child_pid;
	}
}
