/* mshexec - forks and executes a command
 * by R.K. Owen,Ph.D. 10/15/1998
 */
/*
 *********************************************************************
 *
 *     This software is copyrighted by R.K.Owen,Ph.D. 1998
 *
 * The author, R.K.Owen, of this software is not liable for any
 * problems WHATSOEVER which may result from use  or  abuse  of
 * this software. The author, R.K.Owen, grants unlimited rights
 * to anyone who uses and modifies this  software  for  private
 * non-commercial  use  as  long  as  this copyright and rights
 * notice remains in this software and is made available to all
 * recipients of this software.
 *
 * last known email: rkowen@ckns.net
 *                   rk@owen.sj.ca.us
 *
 *********************************************************************
 */

#include <sys/types.h>	/* fork, waitpid */
#include <unistd.h>	/* fork, execvp */
#include <wait.h>	/* waitpid */
#include "librko.h"	/* uvec */
#include "minish.h"	/* fdlist */

/*======================================================================*/
/* Public Interface                                                     */
/*======================================================================*/

/* minish_exec - forks & exec's the given program
 * returns the return status of the executed code.
 * and if there is an error or return status < 0, must chec to
 * rkoerrno to see the reason.
 * returns the signal in (int *signal) that may have stopped the child
 * process and the function returns precisely -1;
 */
int minish_exec(uvec *mshargv, minish_fdlist *fdlist, int *signal) {
	pid_t child;
	int child_status;
	char **argv;

	if (!uvec_exists(mshargv)) {
#ifdef RKOERROR
		rkocpyerror("exec : argv doesn't exist!");
		rkoerrno = RKOUSEERR;
#endif
		return -2;
	}
	if (!minish_fdlist_exists(fdlist)) {
#ifdef RKOERROR
		rkocpyerror("exec : fdlist doesn't exist!");
		rkoerrno = RKOUSEERR;
#endif
		return -3;
	}
/* OK so far */
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif

/* go ahead fork & execute */
	if ((child = fork()) < 0) {
#ifdef RKOERROR
		rkocpyerror("exec : fork failed!");
		rkoerrno = RKOGENERR;
#endif
		return -4;
	}
	if (child) {		/* parent process */
		if (waitpid(child, &child_status, 0) != child) {
#ifdef RKOERROR
			rkocpyerror("exec : parent : wait failed!");
			rkoerrno = RKOGENERR;
#endif
			return -5;
		}
/* check for normal return status */
		if (WIFEXITED(child_status)) {
			*signal = 0;
			return WEXITSTATUS(child_status);
		}
		if (WIFSIGNALED(child_status)) {
			*signal = WTERMSIG(child_status);
			return -1;
		}
	} else {		/* child process */
#ifdef SLEEPDEBUG
		(void) sleep(90);	/* gives time to attach to process */
#endif
		if (minish_fdlist_process(fdlist)) {
#ifdef RKOERROR
			rkopsterror("exec : child : ");
#endif
			return -6;
		}
		if ((argv = uvec_vector(mshargv)) == (char **) NULL) {
#ifdef RKOERROR
			rkopsterror("exec : child : ");
#endif
			return -7;
		}
		if (execvp(argv[0], argv) < 0) {
#ifdef RKOERROR
			rkocpyerror("exec : child : failure!");
			rkoerrno = RKOGENERR;
#endif
			return -8;
		}
		/* child does not return after successful execvp */
	}
}

#ifdef TEST

int main () {
	minish_fdlist fdlist;
	uvec msharg;
	int command=0, retval, sigval;

/* redirect stdout to exectest.out */
	remove("exectest.out");
	freopen("exectest.out", "a", stdout);

/* ---------------------------------------------------------------------- */
/* command mtest/right this is the "right" line of text > exectest.out */
	if (minish_fdlist_ctor(&fdlist)) rkoperror("main");
	if (uvec_ctor(&msharg,10)) rkoperror("main");

	if (minish_fdlist_add(&fdlist, MINISH_FD_WRITE, 1, "exectest.out"))
		rkoperror("main");
	if (uvec_add(&msharg, "mtest/right")) rkoperror("main");
	if (uvec_add(&msharg, "this is the")) rkoperror("main");
	if (uvec_add(&msharg, "\"right\"")) rkoperror("main");
	if (uvec_add(&msharg, "line of text")) rkoperror("main");

	retval = minish_exec(&msharg, &fdlist, &sigval);
	printf("%d: retval = %d, sigval = %d\n", ++command, retval, sigval);
	fflush(stdout);

	if (minish_fdlist_dtor(&fdlist)) rkoperror("main");
	if (uvec_dtor(&msharg)) rkoperror("main");
/* ---------------------------------------------------------------------- */
/* command mtest/wrong this is the "wrong" line of text >> exectest.out */
	if (minish_fdlist_ctor(&fdlist)) rkoperror("main");
	if (uvec_ctor(&msharg,10)) rkoperror("main");

	if (minish_fdlist_add(&fdlist, MINISH_FD_APPEND, 1, "exectest.out"))
		rkoperror("main");
	if (uvec_add(&msharg, "mtest/wrong")) rkoperror("main");
	if (uvec_add(&msharg, "this is the")) rkoperror("main");
	if (uvec_add(&msharg, "\"wrong\"")) rkoperror("main");
	if (uvec_add(&msharg, "line of text")) rkoperror("main");

	retval = minish_exec(&msharg, &fdlist, &sigval);
	printf("%d: retval = %d, sigval = %d\n", ++command, retval, sigval);
	fflush(stdout);

	if (minish_fdlist_dtor(&fdlist)) rkoperror("main");
	if (uvec_dtor(&msharg)) rkoperror("main");
/* ---------------------------------------------------------------------- */
/* command mtest/rsignal SIGCONT a "right signal" line of text >> exectest.out */
	if (minish_fdlist_ctor(&fdlist)) rkoperror("main");
	if (uvec_ctor(&msharg,10)) rkoperror("main");

	if (minish_fdlist_add(&fdlist, MINISH_FD_APPEND, 1, "exectest.out"))
		rkoperror("main");
	if (uvec_add(&msharg, "mtest/rsignal")) rkoperror("main");
	if (uvec_add(&msharg, "SIGCONT")) rkoperror("main");
	if (uvec_add(&msharg, "a")) rkoperror("main");
	if (uvec_add(&msharg, "\"right signal\"")) rkoperror("main");
	if (uvec_add(&msharg, "line of text")) rkoperror("main");

	retval = minish_exec(&msharg, &fdlist, &sigval);
	printf("%d: retval = %d, sigval = %d\n", ++command, retval, sigval);
	fflush(stdout);

	if (minish_fdlist_dtor(&fdlist)) rkoperror("main");
	if (uvec_dtor(&msharg)) rkoperror("main");
/* ---------------------------------------------------------------------- */
/* command mtest/rsignal SIGHUP a "right signal" line of text >> exectest.out */
	if (minish_fdlist_ctor(&fdlist)) rkoperror("main");
	if (uvec_ctor(&msharg,10)) rkoperror("main");

	if (minish_fdlist_add(&fdlist, MINISH_FD_APPEND, 1, "exectest.out"))
		rkoperror("main");
	if (uvec_add(&msharg, "mtest/rsignal")) rkoperror("main");
	if (uvec_add(&msharg, "SIGHUP")) rkoperror("main");
	if (uvec_add(&msharg, "a")) rkoperror("main");
	if (uvec_add(&msharg, "\"right signal\"")) rkoperror("main");
	if (uvec_add(&msharg, "line of text")) rkoperror("main");

	retval = minish_exec(&msharg, &fdlist, &sigval);
	printf("%d: retval = %d, sigval = %d\n", ++command, retval, sigval);
	fflush(stdout);

	if (minish_fdlist_dtor(&fdlist)) rkoperror("main");
	if (uvec_dtor(&msharg)) rkoperror("main");
/* ---------------------------------------------------------------------- */
/* command mtest/rsignal SIGINT a "right signal" line of text >> exectest.out */
	if (minish_fdlist_ctor(&fdlist)) rkoperror("main");
	if (uvec_ctor(&msharg,10)) rkoperror("main");

	if (minish_fdlist_add(&fdlist, MINISH_FD_APPEND, 1, "exectest.out"))
		rkoperror("main");
	if (uvec_add(&msharg, "mtest/rsignal")) rkoperror("main");
	if (uvec_add(&msharg, "SIGINT")) rkoperror("main");
	if (uvec_add(&msharg, "a")) rkoperror("main");
	if (uvec_add(&msharg, "\"right signal\"")) rkoperror("main");
	if (uvec_add(&msharg, "line of text")) rkoperror("main");

	retval = minish_exec(&msharg, &fdlist, &sigval);
	printf("%d: retval = %d, sigval = %d\n", ++command, retval, sigval);
	fflush(stdout);

	if (minish_fdlist_dtor(&fdlist)) rkoperror("main");
	if (uvec_dtor(&msharg)) rkoperror("main");
/* ---------------------------------------------------------------------- */

	return 0;
}
#endif /* TEST */
