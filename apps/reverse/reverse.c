static const char AUTHOR[]="@(#)reverse 11/22/94 1.0 R.K.Owen,PhD";
/*
 * reverse.c - reverse lines from files or stdin
 */
/*
 *********************************************************************
 *
 *     This software is copyrighted by R.K.Owen,Ph.D. 1994
 *
 * The author, R.K.Owen, of this software is not liable for any
 * problems WHATSOEVER which may result from use  or  abuse  of
 * this software. The author, R.K.Owen, grants unlimited rights
 * to anyone who uses and modifies this  software  for  private
 * non-commercial  use  as  long  as  this copyright and rights
 * notice remains in this software and is made available to all
 * recipients of this software.
 *
 * last known email: rkowen@ames.arc.nasa.gov
 *                   rkowen@nas.nasa.gov
 *
 *********************************************************************
 */
/*
 * define TEST to print out lines to stderr at crucial points
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BUFLEN 2048

struct llist {
	char *line;
	struct llist *prev;
};

static void usage(void) {
	(void) fprintf(stderr,
"\nReverse lines for a number of files or stdin and results go to stdout\n"
"usage: reverse [-h] [--] [file [file]...]\n"
"  -h      = display help\n"
"  --      = end of options, useful if the file name starts with '-'\n"
"  files   = files to randomize else use as pipe if no files specified\n\n");
}

main(int argc, char **argv) {
	char line[BUFLEN], *cptr;
	struct llist start, *ptr=&start, *ptrnext, *end = &start;
	int i, nlin=0, nchr=0, len;
	int ist;
	FILE *in;

/* parse for options */
	for(ist=1; ist<argc; ++ist) {
		cptr = argv[ist];
		if (*cptr == '-') {
			cptr++;
			if (*cptr == 'h') {		/* usage help */
				usage();
				return 0;
			} else if (*cptr == '-') {	/* end options */
#ifdef TEST
				(void) fprintf(stderr,"end options\n");
				(void) fflush(stderr);
#endif
				++ist;
				break;
			}
		} else {
#ifdef TEST
				(void) fprintf(stderr,
				"start, argc = %d,%d\n", ist, argc);
				(void) fflush(stderr);
#endif
			break;
		}
	}
#ifdef TEST
	(void) fprintf(stderr,"start of files, argc = %d,%d\n", ist, argc);
	(void) fflush(stderr);
#endif
/* read in files, if ist==argc then use as pipe */
	start.prev = NULL;
	i = ist;
	do {
		if (i == argc) {
			 in = stdin;
#ifdef TEST
			(void) fprintf(stderr,"stdin:READING Pass\n");
			(void) fflush(stderr);
#endif
		} else if((in=fopen(argv[i], "r+")) == (FILE *)NULL)
			(void) perror(argv[i]);
#ifdef TEST
		(void) fprintf(stderr,"%s:READING Pass\n", argv[i]);
		(void) fflush(stderr);
#endif
		while(fgets(line, BUFLEN, in) != (char *)NULL) {
			/* alloc space for line in struct */
			len = strlen(line);
			ptr->line = (char *)
				malloc(sizeof(char)*(len+1));
			if (ptr->line == (char*) NULL) {
				(void) fprintf(stderr,"%s:Not enough memory "
				"... failing in %s\n", argv[0],argv[i]);
				return 1;
			}
			(void) strncpy(ptr->line,line,len+1);
			/* alloc space for next struct */
			ptrnext = (struct llist *) malloc(
				sizeof(struct llist));
			if (ptrnext == (struct llist*) NULL) {
				(void) fprintf(stderr,"%s:Not enough Memory "
				"... failing in %s\n", argv[0],argv[i]);
				return 2;
			}
			/* diddle ptrs to the next & prev struct */
			ptrnext->prev = ptr;
			end = ptr = ptrnext;
			ptrnext = NULL;
			nchr += len;
			nlin++;
		}
		(void) fclose(in);
		i++;
	} while (i < argc);
#ifdef TEST
	(void) fprintf(stderr,"Outputting lines\n");
	(void) fflush(stderr);
#endif
	ptr = end;
	while (ptr != NULL) {
		(void) fputs(ptr->line,stdout);
		ptr = ptr->prev;
	}
	return 0;
}
