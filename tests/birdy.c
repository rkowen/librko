#include <stdio.h>

#define MAXBUF	1024

int main(int argc, char *argv[]) {
	char buffer[MAXBUF];
	char *prgname = *argv++;
	FILE *in;

	while (*argv != (char *) NULL) {
		if ((in = fopen(*argv, "r")) == (FILE *) NULL) {
			(void) fprintf(stderr,"%s: can't open '%s'\n",
				prgname, *argv);
		}
		(void) setvbuf(stdout, (char *) NULL, _IOLBF, 0);
		while (fgets(buffer, MAXBUF, in) != (char *) NULL) {
			if (buffer[strlen(buffer)-1] == '\n')
				buffer[strlen(buffer)-1] = '\0';
			(void) fprintf(stdout,"BOL: %s :EOL\n", buffer);
		}
		(void) fclose(in);
		argv++;
	}
	return 0;
}
