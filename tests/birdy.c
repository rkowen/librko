#include <stdio.h>

#define MAXBUF	1024

int main() {
	char buffer[MAXBUF];

	(void) setvbuf(stdout, (char *) NULL, _IOLBF, 0);
	while (fgets(buffer, MAXBUF, stdin) != (char *) NULL) {
		if (buffer[strlen(buffer)-1] == '\n')
			buffer[strlen(buffer)-1] = '\0';
		(void) fprintf(stdout,"BOL: %s :EOL\n", buffer);
	}
	return 0;
}
