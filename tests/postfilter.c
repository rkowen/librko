/* postfilter.c - routine for fork/exec/dup/pipe a child process
 *	that filters the stdout (similar to popen(,"w"))
 *	returns FILE * of child input stream if successful,
 *	else NULL if not.
 */
#define _POSIX_SOURCE

#include <stdio.h>
#include <stdlib.h>	/* _exit */
#include <sys/types.h>
#include <unistd.h>	/* fork,exec,dup */
#include <signal.h>	/* kill */

char buffer[256];

FILE * postfilter(FILE *outstream, char ** argv) {
	int pipepath[2];
	int parent_wfd, child_rfd, out_fd;
	int child_pid;
	FILE * returnfile = (FILE *) NULL;

	/* open pipe for one-way communication */
	if(pipe(pipepath) < 0) {
		(void) sprintf(buffer,"postfilter: failed to pipe with '%s'",
			argv[0]);
		perror(buffer);
		return (FILE *) NULL;
	}
	/* for clarity only */
	child_rfd = pipepath[0];
	parent_wfd = pipepath[1];

	/* fork process */
	switch(child_pid = fork()) {
	case -1:				/* fork failure */
		(void) sprintf(buffer,"postfilter: failed to fork with '%s'",
			argv[0]);
		perror(buffer);
		(void) close(parent_wfd);
		(void) close(child_rfd);
		return (FILE *) NULL;

	case 0:					/* child */
		if (close(parent_wfd) < 0) {
			(void) sprintf(buffer,
				"postfilter: failed to close writepipe with '%s'",
				argv[0]);
			goto child_failure;
		}
		if (dup2(child_rfd,STDIN_FILENO) < 0) {
			(void) sprintf(buffer,
				"postfilter: failed to dup2 stdin with '%s'",
				argv[0]);
			goto child_failure;
		}
		if (execvp(argv[0],argv) < 0) {
			(void) sprintf(buffer,
				"postfilter: failed to execvp with '%s'",
				argv[0]);
			goto child_failure;
		}
		(void) sprintf(buffer,
			"postfilter: should not 'get here' with '%s'",
			argv[0]);
		child_failure: {
			perror(buffer);
			_exit(1);
		}

	default:				/* parent */
		if (close(child_rfd) < 0) {
			(void) sprintf(buffer,
				"postfilter: failed to close readpipe with '%s'",
				argv[0]);
			goto parent_failure;
		}
		if ((out_fd = fileno(outstream)) < 0) {
			(void) sprintf(buffer,
				"postfilter: failed to fileno stream with '%s'",
				argv[0]);
			goto parent_failure;
		}
		if (dup2(parent_wfd,out_fd) < 0) {
			(void) sprintf(buffer,
				"postfilter: failed to dup2 steam with '%s'",
				argv[0]);
			goto parent_failure;
		}
		if((returnfile = fdopen(parent_wfd, "w")) == (FILE *) NULL) {
			(void) sprintf(buffer,
				"postfilter: failed to fdopen writepipe with '%s'",
				argv[0]);
			goto parent_failure;
		}
		if (0) parent_failure: {
			(void) close(child_rfd);
			(void) close(parent_wfd);
			perror(buffer);
			/* kill the boy... */
			(void) kill(child_pid,SIGKILL);
		}
		/* don't wait for child */
		return returnfile;
	}
}
