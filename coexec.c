static const char RCSID[]="@(#)$Id: coexec.c,v 1.5 2002/03/01 05:23:28 rk Exp $";
static const char AUTHOR[]="@(#)coexec 1.0 2002/02/28 R.K.Owen,Ph.D.";
/* coexec.c - contains all the co-executable routines
 *	invoke		- invoke an executable and retrieve the output
 *	spawn		- spawn  an executable with 2-way communication
 *	prefilter	- popen  an executable that prefilters stdin
 *	postfilter	- popen  an executable that postfilters stdout
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
#define _POSIX_SOURCE

#include <stdio.h>
#include <stdlib.h>	/* _exit */
#include <string.h>	/* strcpy, strcat, etc */
#include <sys/types.h>
#include <unistd.h>	/* fork,exec,dup */
#include <signal.h>	/* kill */
#ifdef RKOERROR
#  include "rkoerror.h"
#endif

char buffer[512];

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

/*----------------------------------------------------------------------*/
/* postfilter.c - *	filters the stdin (similar to popen(,"a"))
 *	returns FILE * of child input stream if successful,
 *	else NULL if not.
 *	Calls preprocess().
 */

FILE *prefilter(FILE *outstream, char ** argv) {
	return preprocess(outstream, argv, STDOUT_FILENO);
}

/*----------------------------------------------------------------------*/
/* postprocess.c - routine for fork/exec/dup/pipe a child process
 *	that filters the given filedesciptor fd for the child process
 *	(similar to popen(,"w"))
 *	returns FILE * of child fd input stream if successful,
 *	else NULL if not.
 */

FILE *postprocess(FILE *outstream, char ** argv, int fd) {
	int pipepath[2];
	int parent_wfd, child_rfd, out_fd;
	int child_pid;
	FILE * returnfile = (FILE *) NULL;

	/* fd must be >= 0 */
	if (fd < 0) {
		strcpy(buffer,"postfilter: invalid fd '");
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
		strcpy(buffer,"postfilter: failed to pipe with '");
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
	child_rfd = pipepath[0];
	parent_wfd = pipepath[1];

	/* fork process */
	switch(child_pid = fork()) {
	case -1:				/* fork failure */
		strcpy(buffer,"postfilter: failed to fork with '");
		strncat(buffer,argv[0],100);
		strcat(buffer,"'");
#ifdef RKOERROR
		(void ) rkocpyerror(buffer);
#else
			perror(buffer);
#endif
		(void) close(parent_wfd);
		(void) close(child_rfd);
		return (FILE *) NULL;

	case 0:					/* child */
		if (close(parent_wfd) < 0) {
			strcpy(buffer,
				"postfilter: failed to close readpipe with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto child_failure;
		}
		if (dup2(child_rfd,fd) < 0) {
			strcpy(buffer,
				"postfilter: failed to dup2 fd '");
			sprintf(buffer + strlen(buffer),"%d", fd);
			strcat(buffer,"' with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto child_failure;
		}
		if (execvp(argv[0],argv) < 0) {
			strcpy(buffer, "postfilter: failed to execvp with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto child_failure;
		}
		strcpy(buffer, "postfilter: should not 'get here' with '");
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
		if (close(child_rfd) < 0) {
			strcpy(buffer,
				"postfilter: failed to close readpipe with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto parent_failure;
		}
		if ((out_fd = fileno(outstream)) < 0) {
			strcpy(buffer,
				"postfilter: failed to fileno stream with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto parent_failure;
		}
		if (dup2(parent_wfd,out_fd) < 0) {
			strcpy(buffer,
				"postfilter: failed to dup2 stream with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto parent_failure;
		}
		if((returnfile = fdopen(parent_wfd, "w")) == (FILE *) NULL) {
			strcpy(buffer,
			  "postfilter: failed to fdopen writepipe with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto parent_failure;
		}
		if(setvbuf(returnfile, (char *) NULL, _IOLBF, 0)) {
			strcpy(buffer,
		"postfilter: parent failed to writepipe line buffer with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto parent_failure;
		}
		if (0) parent_failure: {
			(void) close(child_rfd);
			(void) close(parent_wfd);
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

/*----------------------------------------------------------------------*/
/* postfilter.c - *	filters the stdout (similar to popen(,"w"))
 *	returns FILE * of child input stream if successful,
 *	else NULL if not.
 *	Calls postfilter().
 */

FILE *postfilter(FILE *outstream, char ** argv) {
	return postprocess(outstream, argv, STDIN_FILENO);
}

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

/*----------------------------------------------------------------------*/
/* spawn.c - routine for fork/exec/dup/pipe a child process and tie its
 *	input/output back to be written/read by the caller.
 *	returns the child pid (>0) if successful, else < 0 if error.
 */

int spawn(FILE **childin, FILE **childout, char **argv) {
	int pipepath1[2];
	int pipepath2[2];
	int parent_rfd, parent_wfd;
	int child_rfd,  child_wfd;
	int child_pid = 0;

	/* open pipes for two one-way communication */
	if(pipe(pipepath1) < 0 || pipe(pipepath2) < 0) {
		strcpy(buffer, "spawn: failed to pipe with '");
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

	/* fork process */
	switch(child_pid = fork()) {
	case -1:				/* fork failure */
		strcpy(buffer, "spawn: failed to fork with '");
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
		return -2;

	case 0:					/* child */
		if (close(parent_rfd) < 0 || close(parent_wfd) < 0) {
			strcpy(buffer,
				"spawn: failed to close pipes with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto child_failure;
		}
		if (dup2(child_rfd, STDIN_FILENO) < 0) {
			strcpy(buffer,
				"spawn: child failed to dup2 stdin with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto child_failure;
		}
		if (dup2(child_wfd, STDOUT_FILENO) < 0) {
			strcpy(buffer,
				"spawn: child failed to dup2 stdout with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto child_failure;
		}
		if (child_rfd != STDIN_FILENO) {
			if(close(child_rfd) < 0) {
				strcpy(buffer,
				"spawn: child failed to close in pipe to '");
				strncat(buffer,argv[0],100);
				strcat(buffer,"'");
				goto child_failure;
			}
		}
		if (child_wfd != STDOUT_FILENO) {
			if(close(child_wfd) < 0) {
				strcpy(buffer,
				"spawn: child failed to close out pipe to '");
				strncat(buffer,argv[0],100);
				strcat(buffer,"'");
				goto child_failure;
			}
		}
		if (execvp(argv[0],argv) < 0) {
			strcpy(buffer, "spawn: child failed to execvp with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto child_failure;
		}
		strcpy(buffer, "spawn: should not 'get here' with '");
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
			"spawn: parent failed to close pipes with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto parent_failure;
		}
		if((*childout = fdopen(parent_rfd, "r")) == (FILE *) NULL) {
			strcpy(buffer,
			"spawn: parent failed to fdopen readpipe with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto parent_failure;
		}
		if((*childin = fdopen(parent_wfd, "w")) == (FILE *) NULL) {
			strcpy(buffer,
			"spawn: parent failed to fdopen writepipe with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto parent_failure;
		}
		if(setvbuf(*childout, (char *) NULL, _IOLBF, 0)) {
			strcpy(buffer,
			"spawn: parent failed to readpipe line buffer with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto parent_failure;
		}
		if(setvbuf(*childin, (char *) NULL, _IOLBF, 0)) {
			strcpy(buffer,
			"spawn: parent failed to writepipe line buffer with '");
			strncat(buffer,argv[0],100);
			strcat(buffer,"'");
			goto parent_failure;
		}
		if (0) parent_failure: {
			(void) close(child_wfd);
			(void) close(parent_rfd);
			(void) close(child_rfd);
			(void) close(parent_wfd);
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
