/* quick program to echo the command line and raise & return with the signal
 * number, where the signal is:
 *   (StdC): SIGABRT, SIGFPE, SIGILL, SIGINT, SIGSEGV, SIGTERM
 *  (Posix): SIGALRM, SIGCHLD, SIGCONT, SIGHUP, SIGKILL, SIGQUIT, SIGSTOP,
 *           SIGUSR1,SIGUSR2, SIGTSTP, SIGTTIN, SIGTTOU
 * one of these names MUST be the first argument, else just echoing will occur
 * by R.K.Owen,Ph.D.,  10/15/98
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#ifndef EXIT_SUCCESS
#  define EXIT_SUCCESS 0
#endif

#ifdef __STDC__
int sig_raise(char *sigstr)
#else
int sig_raise(sigstr)
char *sigstr;
#endif
{
	int tsignal = 0;
	char *signame[] = {
		"SIGABRT", "SIGFPE", "SIGILL", "SIGINT", "SIGSEGV", "SIGTERM", 
		"SIGALRM", "SIGCHLD", "SIGCONT", "SIGHUP", "SIGKILL", "SIGQUIT",
		"SIGSTOP", "SIGUSR1", "SIGUSR2",
		"SIGTSTP", "SIGTTIN", "SIGTTOU",
		(char *)NULL};
	int sigvalue[] = {
		SIGABRT, SIGFPE, SIGILL, SIGINT, SIGSEGV, SIGTERM,
		SIGALRM, SIGCHLD, SIGCONT, SIGHUP, SIGKILL, SIGQUIT, SIGSTOP,
		SIGUSR1,SIGUSR2, SIGTSTP, SIGTTIN, SIGTTOU, 0};
	char **ptr = signame;

	if (sigstr == (char *) NULL) return -1;

	while (*ptr != (char *) NULL) {
		if(! strcmp(sigstr, *ptr)) {
			printf("%s=%d\n", *ptr, sigvalue[tsignal]);
			(void) fflush(stdout);
			(void) raise(sigvalue[tsignal]);
			return sigvalue[tsignal];
		}
		tsignal++;
		ptr++;
	}
	return -2;
}

#ifdef __STDC__
main(int argc, char *argv[])
#else
main(argc,argv)
int argc;
char *argv[];
#endif
{
	int i;
	for (i=2; i < argc; i++) printf("%s ", argv[i]);
	printf("\n");
	if(argv[1] != (char *) NULL) {
		i = sig_raise(argv[1]);
		printf("exit normally with %s\n", argv[1]);
	}
	return EXIT_SUCCESS;
}
