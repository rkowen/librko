/* tspawn - test spawn.c, uses testtext & ./birdy */

#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include "coexec.h"
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
	FILE *in, *childin, *childout;
	char *prgname = *argv++;
	char buffer1[MAXBUFF];
	char buffer2[MAXBUFF];
	char *str[MAXTEST];
	int i=0, ret = 0, err = 0;

	HACK("BOL: BOF :EOL")
	HACK("BOL: This is a test :EOL")
	HACK("BOL: This file contains text and nothing more :EOL")
	HACK("BOL:  :EOL")
	HACK("BOL: This Library is provided by R.K.Owen,Ph.D. :EOL")
	HACK("BOL: see copyright notice for details :EOL")
	HACK("BOL: EOF :EOL")
	HACK(NULL)

	subargv = uvec_ctor(3);
	(void) uvec_addl(subargv,"./birdy", "--", NULL);

	if ((in = fopen("testtext", "r")) == (FILE *) NULL) {
		printf("%s failed to open 'testtext'\n", prgname);
		return 1;
	}
	if ((ret = spawn(&childin, &childout, uvec_vector(subargv))) <=0 ) {
		printf("%s failed to spawn '%s' ret = %d\n", prgname,
			*uvec_vector(subargv), ret);
		return 2;
	}

	i = 0;
	while (!(fgets(buffer1, MAXBUFF, in), feof(in))) {
		(void) fprintf(childin,"%s", buffer1);
		if (!fgets(buffer2, MAXBUFF, childout)) {
			break;
		}

		if (buffer2[strlen(buffer2)-1] == '\n')	/* strip trailing \n */
			buffer2[strlen(buffer2)-1] = '\0';
		if (strcmp(buffer2, str[i])) {
			err++;
			printf("FAIL: %s\n    > %s\n", buffer2, str[i]);
		} else {
			printf("OK  : %s\n", buffer2);
		}
		i++;
	}
	(void) fclose(childin);
	(void) fclose(childout);

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
