/* :*:cc color.c -I$HOME/include -L$HOME/lib -lrko
 * :*:END:
 */
/* color - outputs an ANSI color sequence to stdout
 *
 * by R.K.Owen,Ph.D. 12/29/95
 * requires the librko/ansi_seq routines
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ansi_seq.h"

void usage(char *ProgName) {
	int i;

	(void) printf(
"\n%s%s%s - outputs an ANSI character attribute & color sequence\n"
"	which are of the form ^[[xx;xx;xxm, where ^[ is an <ESC> and\n"
"	xx are the proper character codes for attribute, background,\n"
"	and foreground character color. Each one is output as processed and\n"
"	errors are ignored.\n"
"\n"
"usage: %s%s%s [-t|%s[attribute]%s %s[-bgcolor]%s %s[+fgcolor]%s]"
"\n"
"where:\n"
"	-t	print out color test only\n\n"
"or:	use the number or name (or beginning part thereof)\n"
"	to set that attribute\n",
		ansi_seq(BOLD, BLACK, RED),
		ProgName,
		ansi_seq(NONE, BLACK, WHITE),
		ansi_seq(BOLD, BLACK, RED),
		ProgName,
		ansi_seq(NONE, BLACK, WHITE),
		ansi_seq(NONE, WHITE, MAGENTA),
		ansi_seq(NONE, BLACK, WHITE),
		ansi_seq(BOLD, WHITE, CYAN),
		ansi_seq(NONE, BLACK, WHITE),
		ansi_seq(BOLD, CYAN, WHITE),
		ansi_seq(NONE, BLACK, WHITE));

	(void) printf("%s%-20s%s  %s%-20s%s\n",
		ansi_seq(NONE, WHITE, MAGENTA),
		"attribute",
		ansi_seq(NONE, BLACK, WHITE),
		ansi_seq(BOLD, WHITE, CYAN),
		"bg/fg color",
		ansi_seq(NONE, BLACK, WHITE));
	for (i = 0; i < ATTRIB_NUM; ++i) {
		(void) printf("%2d %-17s %2d %-17s\n",
			i+1,  ANSI_Attributes[i],
			i+1,  ANSI_Colors[i]);
	}
	for (i = ATTRIB_NUM; i < COLOR_NUM; ++i) {
		(void) printf("%-20s %2d %-17s\n",
			" ",
			i+1,  ANSI_Colors[i]);
	}
	(void) printf("\n");
}

int NumberOrName(const char **names, int uplimit, char *arg) {
	long t = 0l;
	int i;
	size_t l;
	int flag = 0;

	t = atol(arg);
	if ((t > uplimit) || (t <= 0)) {/* failed finding number */
		l = strlen(arg);
		for (i = 0; i < uplimit; ++i) { /* search names */
			if (!strncmp(arg, names[i], l)) { /* match */
				return i;
			}
		}
	} else {			/* found valid number */
		return (int) (t - 1);	/* offset by 1 */
	}
	return -1;			/* ruinous failure */
}

int main(int argc, char *argv[]) {
	char *PrgNm = *argv++;
	int i;

	const char *(*ansifn)();
	const char **ansiattcol;
	int uplimit;

	if (argc <= 1) {
		usage(PrgNm);
		return 1;
	}

	(void) set_ansi_tty(1);		/* force ANSI strings */

	while (*argv != (char *) NULL) {
		if (**argv == '+') {
			ansifn = ansi_fgcolor;
			(*argv)++;
			ansiattcol = ANSI_Colors;
			uplimit = COLOR_NUM;
		} else if (**argv == '-') {
			if (*(*argv + 1) == 't') {
				return tansi_seq(0, NULL);
			}
			ansifn = ansi_bgcolor;
			(*argv)++;
			ansiattcol = ANSI_Colors;
			uplimit = COLOR_NUM;
		} else {
			ansifn = ansi_attribute;
			ansiattcol = ANSI_Attributes;
			uplimit = ATTRIB_NUM;
		}
		if ((i = NumberOrName(ansiattcol, uplimit, *argv)) < 0) {
			/* failed so go on with your life */
		} else {
			/* sweet success */
			(void) printf("%s", (*ansifn)(i));
		}
		argv++;			/* try next one */
	}

	return 0;
}
