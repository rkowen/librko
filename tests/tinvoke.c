/* tinvoke - test invoke.c, uses testtext */

#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include "coexec.h"
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#include "uvec.h"
#ifdef RKOERROR
extern int rkoerrno;
#endif

#define MAXTEST 8
/* the following is for brain-dead K&R C compilers found on Sun's */
#define HACK(a) str[i] = a; i++;

#define MAXBUFF		1024

int main(int argc, char *argv[]) {

	uvec *subargv;
	FILE *in;
	char *prgname = *argv++;
	char buffer[MAXBUFF];
	char *str[MAXTEST];
	int i=0, err = 0;

	HACK("BOL: BOF :EOL")
	HACK("BOL: This is a test :EOL")
	HACK("BOL: This file contains text and nothing more :EOL")
	HACK("BOL:  :EOL")
	HACK("BOL: This Library is provided by R.K.Owen,Ph.D. :EOL")
	HACK("BOL: see copyright notice for details :EOL")
	HACK("BOL: EOF :EOL")

	subargv = uvec_ctor(3);
	(void) uvec_addl(subargv,"./birdy", "testtext", NULL);

	if ((in = invoke(uvec_vector(subargv))) == (FILE *) NULL) {
		printf("%s failed to invoke '%s'\n", prgname,
			*uvec_vector(subargv));
		return 1;
	}
	i = 0;
	while (!(fgets(buffer, MAXBUFF, in), feof(in))) {
		if (buffer[strlen(buffer)-1] == '\n')	/* strip trailing \n */
			buffer[strlen(buffer)-1] = '\0';
		if (strcmp(buffer, str[i])) {
			err++;
			printf("FAIL: %s\n    > %s\n", buffer, str[i]);
		} else {
			printf("OK  : %s\n", buffer);
		}
		i++;
	}
	if (err) {
		printf("    %d test failures out of %d\n", err, i);
	} else {
		printf("    no test failures out of %d\n", i);
	}

	fclose(in);
	wait(NULL);
	(void) uvec_dtor(&subargv);

	return err;
}
