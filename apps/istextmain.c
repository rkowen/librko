/* istext - returns a status of 0 if the file(s) given on the command line
 *	are purely text files, else returns 1.
 *	Read the usage for other options.
 *
 * by R.K.Owen,Ph.D. (rk@owen.sj.ca.us)
 *
 */

#include <stdio.h>
int istext(int);

void usage(char *progname) {
	(void) fprintf(stderr,
"\n"
"%s\n"
"	- returns a status of either 0 or 1, depending whether the file(s)\n"
"	are entirely text or not.\n"
"	returns -1 if it cannot open the file, and -2 if none on commandline\n\n"
"usage: %s [-v|-t|-b][-h][--]file [...]\n"
"	-v = verbose (report filename and \"is text\"/\"is binary\"/\"cannot open\")\n"
"	-t = report only the filenames of text   files given on the commandline\n"
"	-b = report only the filenames of binary files given on the commandline\n"
"	-h = this helpful message\n"
"	-- = stop option parsing (useful for filenames that start with `-')\n"
"\n"
	, progname, progname);
}

int main (int argc, char *argv[]) {
	char *progname = *argv++;
	int verbose = 0, textonly = 0, binaryonly = 0;
	char *ptr;
	FILE *stream;
	int c;

	if (*argv == (char *) NULL) {
		return -2;
	}
/* parse for options */
	while (ptr = *argv, *ptr++ == '-') {
		switch (*ptr) {
		case 'h':
		case 'H':
			usage(progname);
			return -1;
		case 'v':
		case 'V':
			textonly = 0;
			binaryonly = 0;
			verbose = ~0;
			break;
		case 't':
		case 'T':
			textonly = ~0;
			binaryonly = 0;
			verbose = 0;
			break;
		case 'b':
		case 'B':
			textonly = 0;
			binaryonly = ~0;
			verbose = 0;
			break;
		case '-':
			argv++;
			goto endopts;
		default:
			/* do nothing on non-valid options */
		}
		argv++;
	}
endopts:
/* parse for files and test them */
	while (*argv != (char *) NULL) {
		if (!(stream = fopen(*argv, "r"))) {
			if (verbose) {
				printf("%s cannot open\n", *argv);
				goto endfiletest;
			} else if (textonly) {
				goto endfiletest;
			} else if (binaryonly) {
				goto endfiletest;
			} else {
				return -1;
			}
		} else {	/* read through file testing each character */
			while (!(c = fgetc(stream), feof(stream))) {
				if (!istext(c)) {
					if (verbose) {
						printf("%s is binary\n", *argv);
						goto endfiletest;
					} else if (textonly) {
						goto endfiletest;
					} else if (binaryonly) {
						printf("%s\n", *argv);
						goto endfiletest;
					} else {
						return 1;
					}
				}
			}
			if (verbose) {
				printf("%s is text\n", *argv);
			} else if (textonly) {
				printf("%s\n", *argv);
				goto endfiletest;
			}
endfiletest:		fclose(stream);
		}
		++argv;
	}
	return 0;	/* all the files were text */
}
