/*
 * tests the prefilter, postfilter functions with ./doggy & ./kitty
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>	/* sleep */
#include <sys/wait.h>	/* wait */
#include "coexec.h"

#define MAXTEST 8
/* the following is for brain-dead K&R C compilers found on Sun's */
#define HACK(a) str[i] = a; i++;

#define MAXBUF		1024

int main() {

	FILE *in, *out, *infile, *outfile;
	char *inv[2] = {"./kitty", (char *) NULL};
	char *outv[2] = {"./doggy", (char *) NULL};
	char *out2v[3] = {"./birdy","--", (char *) NULL};
	char buffer[MAXBUF];
	char tmpfile[L_tmpnam];
	char *cptr;

	char *str[MAXTEST];
	int i=0, err = 0;

	HACK("BOL: doggy:KITTY:BOF :EOL")
	HACK("BOL: doggy:KITTY:THIS IS A TEST :EOL")
	HACK("BOL: doggy:KITTY:THIS FILE CONTAINS TEXT AND NOTHING MORE :EOL")
	HACK("BOL: doggy:KITTY: :EOL")
	HACK("BOL: doggy:KITTY:THIS LIBRARY IS PROVIDED BY R.K.OWEN,PH.D. :EOL")
	HACK("BOL: doggy:KITTY:SEE COPYRIGHT NOTICE FOR DETAILS :EOL")
	HACK("BOL: doggy:KITTY:EOF :EOL")
	HACK(NULL)


#ifndef TEST
	if ((outfile = freopen(tmpnam(tmpfile),"w",stdout)) == (FILE *) NULL){
		printf("FAILED - fopen/stdout\n");
		return 1;
	}
	/* kitty */
	if ((infile = freopen("testtext","r",stdin)) == (FILE *) NULL) {
		printf("FAILED - fopen/stdin\n");
		return 2;
	}
	if ((in = prefilter(stdin, inv)) == (FILE *) NULL) {
		printf("FAILED - prefilter\n");
		return 3;
	}
	/* birdy */
	if ((out = postfilter(stdout, out2v)) == (FILE *) NULL) {
		printf("FAILED - postfilter\n");
		return 5;
	}
	/* doggy */
	if ((out = postfilter(stdout, outv)) == (FILE *) NULL) {
		printf("FAILED - postfilter\n");
		return 4;
	}
#else
	in = stdin;
	out = stdout;
#endif

#if 0
	fprintf(stdout,"stdout:start-of-in\n");
	fflush(stdout);
#endif
	i = 0;
	while (fgets(buffer,MAXBUF,in),!feof(in)) {
		cptr = buffer;
		while (*cptr) {		/* tfilter all text to UPPERCASE */
			*cptr = (char) toupper((int) *cptr);
			cptr++;
		}
		fputs(buffer,out);
		/* fflush(out); */
	}
	fprintf(stderr,"end-of-in\n");
	sleep(1);
	wait(NULL);
	fclose(in);
	fclose(out);
	if ((in = fopen(tmpfile,"r")) == (FILE *) NULL) {
		printf("FAILED - fopen/tmpfile\n");
		return 6;
	}
	while (fgets(buffer,MAXBUF,in),!feof(in)) {
		if (buffer[strlen(buffer)-1] == '\n')	/* strip trailing \n */
			buffer[strlen(buffer)-1] = '\0';
		if (strcmp(buffer, str[i])) {
			err++;
			fprintf(stderr,"FAIL: %s\n    > %s\n", buffer, str[i]);
		} else {
			fprintf(stderr,"OK  : %s\n", buffer);
		}
		i++;
	}
	fclose(in);

	/* remove(tmpfile); */

	if (err) {
		fprintf(stderr,"    %d test failures out of %d\n", err, i);
	} else {
		fprintf(stderr,"    no test failures out of %d\n", i);
	}
	return err;
}
