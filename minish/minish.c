/* minish.c - minish (mini-shell) which will efficiently process
 *	a string similar to the system() function, but will produce
 *	reasonable return values and handle more types of file redirection.
 */

#include <stdio.h>
#include <string.h>

#ifndef min
#  define MIN(a,b)	((a)<(b) ? (a) : (b))
#endif
int minish_yyparse(void);

static const char *commandline_ptr;	/* current position in commandline */
static int commandline_current;		/* current position in commandline */
static int commandline_lim;		/* commandline EOD */

extern int minish_signal;		/* signal sent to a child process */
extern int minish_retval;		/* last valid return value */

int minish_string_yyinput(char *buf, int max_size) {
	int n = MIN(max_size, commandline_lim - commandline_current); 

	if (n > 0) {
		memcpy(buf, commandline_ptr, n);
		commandline_ptr += n;
		commandline_current += n;
	}
	if (*commandline_ptr == '\0' && 0 < n && n < max_size
	&& *(commandline_ptr-1) != '\n') {
	/* paste on a \n to terminate line */
		buf[n++] = '\n';
	}
	return n;
}

int minish(const char *commandline, int *signal) {
	int retval = 0;

	commandline_ptr = commandline;
	commandline_current = 0;
	commandline_lim = strlen(commandline);
	*signal = 0;

	if (commandline_lim > 0) {
		retval = minish_yyparse();
		if (minish_retval == -1) *signal = minish_signal;
	}
	return minish_retval;
}

#ifdef SHTEST

#  define MAXBUF 256

int main() {
	FILE *shtestfile;
	FILE *shtestdat;
	char buffer[MAXBUF];
	char commandline[MAXBUF];
	int retdat = 0, sigdat = 0;
	int retval = 0, sigval = 0;

	if (!(shtestfile = freopen("shtest.out", "a", stdout))) {
		fprintf(stderr,"Can't open shtest.out file for output\n");
		return 1;
	}

	if (!(shtestfile = freopen("shtest.dat", "r", stdin))) {
		fprintf(stderr,"Can't open shtest.dat file for input\n");
		return 2;
	}

	while (!(fgets(buffer,MAXBUF,stdin), feof(stdin))) {
		sscanf(buffer," %d  %d %[^\n]\n",
			&retdat, &sigdat, commandline);
		retval = minish(commandline, &sigval);
		if (retdat == 0 && retval != 0) rkoperror("main");
		if (retdat != 0) rkoperror("main");
		printf("ret=%3d sig=%3d\n", retval, sigval); fflush(stdout);
	}
	return 0;
}
#endif /* SHTEST */
