/* tspawn3 - test spawn3.c, uses testtext & ./tweety */

#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#include "coexec.h"
#include "uvec.h"
#ifdef RKOERROR
extern int rkoerrno;
#endif

#define MAXTEST 8
/* the following is for brain-dead K&R C compilers found on Sun's */
#define HACK(a) str[i] = a; i++;
#define HACK2(a) estr[i] = a; i++;

#define MAXBUFF		1024

int main(int argc, char *argv[]) {

	uvec *subargv;
	FILE *in, *childin, *childout, *childerr;
	char *prgname = *argv++;
	char buffer1[MAXBUFF];
	char buffer2[MAXBUFF];
	char buffer3[MAXBUFF];
	char *str[MAXTEST];
	char *estr[MAXTEST];
	int i=0, ret = 0, err = 0;

	HACK("BOL: BOF :EOL")
	HACK("BOL: This is a test :EOL")
	HACK("BOL: This file contains text and nothing more :EOL")
	HACK("BOL:  :EOL")
	HACK("BOL: This Library is provided by R.K.Owen,Ph.D. :EOL")
	HACK("BOL: see copyright notice for details :EOL")
	HACK("BOL: EOF :EOL")
	HACK(NULL)

	i=0;
	HACK2("CNT: '1'")
	HACK2("CNT: '2'")
	HACK2("CNT: '3'")
	HACK2("CNT: '4'")
	HACK2("CNT: '5'")
	HACK2("CNT: '6'")
	HACK2("CNT: '7'")
	HACK2(NULL)

	subargv = uvec_ctor(3);
	(void) uvec_addl(subargv,"./tweety", "--", NULL);

	if ((in = fopen("testtext", "r")) == (FILE *) NULL) {
		printf("%s failed to open 'testtext'\n", prgname);
		return 1;
	}

	if ((ret=spawn3(&childin,&childout,&childerr,uvec_vector(subargv)))<=0){
		printf("%s failed to spawn3 '%s' ret = %d\n", prgname,
			*uvec_vector(subargv), ret);
		return 2;
	}

	i = 0;

	/* all the i/o is short enough to fit in the i/o buffers ...
	 * i.e. no selects need to be done.
	 */

	/* read file to child stdin and read child stdout */
	/* read the child's stderr too */
	while (!(fgets(buffer1, MAXBUFF, in), feof(in))) {
		(void) fprintf(childin,"%s", buffer1);
		if (!fgets(buffer2, MAXBUFF, childout)) break;
		if (!fgets(buffer3, MAXBUFF, childerr)) break;

		if (buffer2[strlen(buffer2)-1] == '\n')	/* strip trailing \n */
			buffer2[strlen(buffer2)-1] = '\0';
		if (buffer3[strlen(buffer3)-1] == '\n')	/* strip trailing \n */
			buffer3[strlen(buffer3)-1] = '\0';

		if (strcmp(buffer2, str[i])) {
			err++;
			printf("FAIL: %s\n    > %s\n", buffer2, str[i]);
		} else {
			printf("OK  : %s\n", buffer2);
		}
		if (strcmp(buffer3, estr[i])) {
			err++;
			printf("FAIL: stderr: %s\n    > %s\n",buffer3,estr[i]);
		} else {
			printf("OK  : stderr: %s\n", buffer3);
		}
		i++;
	}

	(void) fclose(childin);
	(void) fclose(childout);
	(void) fclose(childerr);

	if (err) {
		printf("    %d test failures out of %d\n", err, 2*i);
	} else {
		printf("    no test failures out of %d\n", 2*i);
	}

	fclose(in);
	wait(NULL);
	(void) uvec_dtor(&subargv);

	return err;
}
