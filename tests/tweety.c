/* tweety - reads files given on commandline line-by-line and for each line
 *	it prepends "BOL: " and appends " :EOL" and sends it to stdout.
 *	It also passes a line count on stderr "CNT: '%d'".
 *	If a commandline argument is "--" then use stdin at that point.
 *
 *	by R.K.Owen, Ph.D.	11/17/1998
 */

#include <stdio.h>
#include <string.h>

#define MAXBUF	1024

int main(int argc, char *argv[]) {
	char buffer[MAXBUF];
	char *prgname = *argv++;
	int cnt = 0;
	FILE *in;

	while (*argv != (char *) NULL) {
		if (strcmp(*argv,"--")) {	/* want stdin */
			if ((in = fopen(*argv, "r")) == (FILE *) NULL) {
				(void) fprintf(stderr,"%s: can't open '%s'\n",
					prgname, *argv);
			}
		} else {
			in = stdin;
		}
		(void) setvbuf(stdout, (char *) NULL, _IOLBF, 0);
		while (fgets(buffer, MAXBUF, in) != (char *) NULL) {
			cnt++;
			if (buffer[strlen(buffer)-1] == '\n')
				buffer[strlen(buffer)-1] = '\0';
			(void) fprintf(stdout,"BOL: %s :EOL\n", buffer);
			(void) fprintf(stderr,"CNT: '%d'\n", cnt);
		}
		(void) fclose(in);
		argv++;
	}
	return 0;
}
