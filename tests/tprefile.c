/* tinvoke - test invoke.c, uses testtext */

#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include "coexec.h"
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#ifdef RKOERROR
extern int rkoerrno;
#endif

#define MAXTEST 8
/* the following is for brain-dead K&R C compilers found on Sun's */
#define HACK(a) str[i] = a; i++;

#define MAXBUFF		1024

int try_prefile(char *filename, char **str) {
	FILE *in;
	char *inname;
	char buffer[MAXBUFF];
	int i=0, err = 0;

	if ((inname = prefile(filename)) == (char *) NULL) {
		printf("%s failed to prefile '%s'\n", filename);
		unlink_prefile();
		return 100;
	}
	if ((in = fopen(inname,"r")) == (FILE *) NULL) {
		printf("%s failed to open prefile '%s' for '%s'\n",
			inname, filename);
		unlink_prefile();
		return 200;
	}
	unlink_prefile();  /* this is safe under UNIX */
	i = 0;
	printf("\n    prefile '%s' test\n", filename);
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
		printf("    %d test failures out of %d for %s\n", err, i,
			filename);
	} else {
		printf("    no test failures out of %d for %s\n", i,
			filename);
	}

	fclose(in);
	wait(NULL);
	return err;
}

int main(int argc, char *argv[]) {

	char *str[MAXTEST];
	char *prgname = *argv++;
	int i=0, err = 0;

	HACK("BOF")
	HACK("This is a test")
	HACK("This file contains text and nothing more")
	HACK("")
	HACK("This Library is provided by R.K.Owen,Ph.D.")
	HACK("see copyright notice for details")
	HACK("EOF")

	i=0;
	i++; err += (try_prefile("testtext"    , str)? 1 : 0);
#if defined(GZIP)
	i++; err += (try_prefile("testtext2"   , str)? 1 : 0);
#endif
#if defined(GZIP) || defined(UNCOMPRESS)
	i++; err += (try_prefile("testtext3"   , str)? 1 : 0);
#endif
#if defined(GZIP)
	i++; err += (try_prefile("testtext4"   , str)? 1 : 0);
#endif
#if defined(GZIP)
	i++; err += (try_prefile("testtext2.gz", str)? 1 : 0);
#endif
#if defined(GZIP) || defined(UNCOMPRESS)
	i++; err += (try_prefile("testtext3.Z" , str)? 1 : 0);
#endif
#if defined(GZIP)
	i++; err += (try_prefile("testtext4.z" , str)? 1 : 0);
#endif


	if (err) {
		printf("\n    %d collective prefile test failures out of %d\n",
			err, i);
	} else {
		printf("\n    no collective prefile test failures out of %d\n",
			i);
	}

	return err;
}
