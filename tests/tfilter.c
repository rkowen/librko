/*
 * tests the prefilter, postfilter functions
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>	/* wait */
#include <ctype.h>
#include "../librko.h"

int main() {

	FILE *in, *out;
	char *inv[2] = {"./kitty", (char *) NULL};
	char *outv[2] = {"./doggy", (char *) NULL};
	char buffer[256];
	char *cptr;

#ifndef TEST
	if ((in = prefilter(stdin, inv)) == (FILE *) NULL) {
		printf("FAILED - prefilter\n");
		return EXIT_FAILURE;
	}
	if ((out = postfilter(stdout, outv)) == (FILE *) NULL) {
		printf("FAILED - postfilter\n");
		return EXIT_FAILURE;
	}
#else
	in = stdin;
	out = stdout;
#endif

	while (fgets(buffer,256, in), !feof(in)) {
		cptr = buffer;
		while (*cptr) {		/* tfilter all text to UPPERCASE */
			*cptr = (char) toupper((int) *cptr);
			cptr++;
		}
		fputs(buffer,out);
		fflush(out);
	}
	wait(NULL);
	fclose(in);
	fclose(out);

	return EXIT_SUCCESS;
}
