/* kitty - just prepends "kitty:" to each line of stdin and
 *	sends it to stdout.
 * by R.K.Owen,Ph.D. 11/17/1998
 */

#include <stdio.h>

#define MAXBUF	1024

int main() {
	char buffer[MAXBUF];

	(void) setvbuf(stdout, (char *) NULL, _IOLBF, 0);
	while (fgets(buffer, MAXBUF, stdin) != (char *) NULL) {
		if (buffer[strlen(buffer)-1] == '\n')
			buffer[strlen(buffer)-1] = '\0';
		(void) fprintf(stdout,"kitty:%s\n", buffer);
	}
	return 0;
}
