/* wf - does a word frequency count on the input text
 *
 * by R.K.Owen,Ph.D. 2002/02/14
 * requires the librko avec & uvec routines
 */

#include <stdio.h>
#include <string.h>	/* strlen */
#include <ctype.h>	/* isalpha, tolower */
#include <unistd.h>	/* getopt */
#include "avec.h"
#include "uvec.h"

void usage(char *ProgName) {
	int i;

	(void) printf(
"\n"
"%s - does a word frequency count on the input text"
"\n"
"usage: %s [-h][-c][-n][-o output][--][input ...]\n"
"	-h		this helpful information\n"
"	-c		do not convert to lowercase (case is important)\n"
"	-n		last line shows aggregate and unique word count\n"
"	-o output	output file name (else defaults to stdout)\n"
"	--		stops further option processing\n"
"	input ...	one or more input file names (else defaults to stdin)\n"
"			the input of '-' will use stdin at that place\n"
"\n"
"output: is a sorted list with the count first, a tab, then the word\n\n",
		ProgName, ProgName);
}

/* global values */
#define WBUFSIZE 1024
char wbuf[WBUFSIZE];
int keep_case = 0;
int out_num = 0;
avec *words;

/* getopt misc. */
static const char GETOPTstr[] = "hcno:";
extern char *optarg;
extern int optind, opterr, optopt;

int initial_word (void) {
	if (avec_set_fns(AVEC_COUNT,NULL)) return -2;
	if (!(words = avec_ctor(500))) return -1;
	if (avec_resize_percentage(words,40)) return -3;
	return 0;
}

int final_word (FILE *output) {
	avec_element **waeptr = (avec_element **) NULL;
	char const * const * vec;
	uvec *uv;
	char **ptr;
	unsigned int   icount;
	unsigned long  lcount;
	unsigned short scount;
	unsigned long  wcount = 0, acount = 0;

	/* get keys */
	vec = avec_keys(words);
	/* put into uvec */
	uv = uvec_alloc();
	uvec_copy_vec(uv, vec, 0);
	/* sort uvec */
	uvec_sort(uv, UVEC_ASCEND);

	/* dump vector */
	ptr = uvec_vector(uv);
	while (*ptr) {
		wcount++;
		if (sizeof(void *) == sizeof(unsigned int)) {
			icount = (unsigned int) avec_lookup(words,*ptr);
			lcount = icount;
		} else if (sizeof(void *) == sizeof(unsigned short)) {
			scount = (unsigned short) avec_lookup(words,*ptr);
			lcount = scount;
		} else if (sizeof(void *) == sizeof(unsigned long)) {
			lcount = (unsigned long) avec_lookup(words,*ptr);
		} else if (sizeof(void *) == sizeof(unsigned long)) {
		}
		acount += lcount;
		fprintf(output,"%7ld\t%s\n", lcount, *ptr);
		ptr++;
	}
	if (out_num) {
		fprintf(output,"%7ld\t%ld\n", acount, wcount);
	}

	if (avec_dtor(&words)) return -1;
	return 0;
}

int count_words(FILE *input) {
	char *wptr = wbuf, *ptr;
	int wbufsize = sizeof(wbuf)/sizeof(*wbuf);
	size_t wlen = 0;
	size_t numread;

	/* make sure wbuf ends with '\0' */
	wbuf[--wbufsize] = '\0';

	/* read into buffer */
	while ((numread = fread(wptr,1,wbufsize - wlen,input) + wlen)) {
		/* set anything non-alph to '\0' */
		ptr = wptr;			/* do only what was read */
		while (ptr - wptr < numread) {
			*ptr = (isalpha((int) *ptr) ? *ptr : '\0');
			ptr++;
		}
		/* translate to lowercase */
		if (!keep_case) {
			ptr = wptr;		/* do only what was read */
			while (ptr - wptr < numread) {
				*ptr = (char) tolower((int) *ptr);
				ptr++;
			}
		}
		/* search for strings */
		ptr = wbuf;			/* look at full wbuf */
		while (ptr - wptr < numread) {
			if (!*ptr) {ptr++; continue;}	/* skip '\0's */

			wlen = strlen(ptr);		/* length of string */

			if (ptr - wptr + wlen >= wbufsize) {
			/* at end of block and possibly a split word */
				strcpy(wbuf, ptr);
				wptr = wbuf + wlen;
				ptr = wptr + numread;	/* end of block */
			} else {
				avec_insert(words,ptr);
				ptr += wlen;
				wlen = 0;
			}
		}
	}
}

int main(int argc, char *argv[]) {
	int retval = 0;
	char *PrgNm = argv[0];	/* must send entire argv to getopt */
	char *ptr;
	int i;
	FILE *output;
	FILE *input;

	output = stdout;
	input = stdin;

	while ((i = getopt(argc,argv,GETOPTstr)) != -1) {
		switch (i) {
		case 'h':
			usage(PrgNm);
			return 0;
			break;
		case 'c':
			keep_case = 1;
			break;
		case 'n':
			out_num = 1;
			break;
		case 'o':
			if (!(output = fopen(optarg, "w"))) {
				fprintf(stderr,"Can't open %s for writing!\n\n",
					optarg);
				usage(PrgNm);
				return 2;
			}
			break;
		case '?':
		default:
			usage(PrgNm);
			return 1;
			break;
		}
	}

	/* cycle through arguments */
	if ((retval = initial_word())) {
		fprintf(stderr,"%s : initial_word failure = '%d'\n",
			PrgNm, retval);
		return retval;
	}
	argv = &(argv[optind]);
	if (*argv) {
		while (*argv) {				/* step through args */
			if (!strcmp(*argv,"-")) {	/* want stdin */
				input = stdin;
			} else {
				if(!(input = fopen(*argv,"r"))) {
					fprintf(stderr,
					"\nCan't open %s for reading!\n\n",
						*argv);
				}
			}
			argv++;
			retval = count_words(input);
			fclose(input);
		}
	} else {					/* no arg hence stdin */
		retval = count_words(input);
	}

	if ((retval = final_word(output))) {
		fprintf(stderr,"%s : final_word failure = '%d'\n",
			PrgNm, retval);
		return retval;
	}
	fclose(output);
	return 0;
}
