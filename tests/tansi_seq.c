/* quick & dirty program to create an ANSI color sequence
 * by R.K. Owen, Ph.D.  12/21/95
 */

#include <stdio.h>
#if 0
#  include "ansi_seq.h"
#else
#  include "librko.h"
#endif

char *attribchar[ATTRIB_NUM] = {
  "N",    "B",    "U",          "K",     "R",       "C"};
char *attribnum[ATTRIB_NUM] = {
  "00",   "01",   "04",         "05",    "07",      "08"};
char *fgcolornum[COLOR_NUM] = {
  "30",    "31",   "32",   "33",     "34",   "35",      "36",   "37"};
char *bgcolornum[COLOR_NUM] = {
  "40",    "41",   "42",   "43",     "44",   "45",      "46",   "47"};

int main(int argc, char *argv[]) {
	int i,ii,j,k;
	char word[] = " AaBbCc ";

	if (argc > 1) {
		argv++;
		if (**argv == '-') {
			k = set_ansi_tty(0);
			(void) printf(
			"Former state = %d, turning ANSI sequences off!\n", k);
		} else if (**argv == '+') {
			k = set_ansi_tty(1);
			(void) printf(
			"Former state = %d, turning ANSI sequences on!\n", k);
		}
	}

	(void) printf("\n%s%-20s %s%-20s %s%-20s%s\n",
		ansi_seq(NONE, WHITE, BLUE), 	"ATTRIBUTE",
		ansi_fgcolor(RED),		"NUMBER",
		ansi_fgcolor(BLACK), 		"MATRIX-SYMBOL",
		ansi_seq(NONE, BLACK, WHITE));
	for (ii = 0; ii <= ATTRIB_NUM; ++ii) {
		i = ii % ATTRIB_NUM;
		(void) printf("%s%s%s%-20s%s %-20s %-20s\n",
			ansi_attribute(NONE),
			ansi_bgcolor(WHITE),
			ansi_fgcolor(MAGENTA),
			ANSI_Attributes[i],
			ansi_seq(BOLD, BLACK, WHITE),
			attribnum[i], attribchar[i]);
	}

	(void) printf("\n%s%-20s%s %-20s %s%-20s%s\n",
		ansi_seq(NONE, WHITE, BLUE),	"COLORS",
		ansi_fgcolor(RED),		"FG-NUMBER",
		ansi_fgcolor(BLACK),		"BG-NUMBER",
		ansi_seq(NONE, BLACK, WHITE));

	for (i = 0; i < COLOR_NUM; ++i) {
		(void) printf("%s%-20s%s %-20s %-20s\n",
			ansi_seq(NONE, WHITE, MAGENTA),
			ANSI_Colors[i],
			ansi_seq(BOLD, BLACK, WHITE),
			fgcolornum[i], bgcolornum[i]);
	}

	(void) printf("\n%sColor Matrix\n%s%-8s%s \\ %s%s \n",
		ansi_seq(NONE, WHITE, BLUE),
		ansi_seq(NONE, WHITE, RED),
		"FG",
		ansi_seq(NONE, WHITE, BLACK),
		ansi_seq(NONE, WHITE, MAGENTA),
		"BG");
	(void) printf("%s%-8s ",
		ansi_seq(NONE, WHITE, MAGENTA),
		" ");
	for (j = 0; j < COLOR_NUM - 1; ++j) {
		(void) printf("%-9s", ANSI_Colors[j]);
	}
	(void) printf("%s \n", ANSI_Colors[COLOR_NUM - 1]);
	for (k = 0; k < COLOR_NUM; ++k) {
		(void) printf("%s%-8s%s ",
			ansi_seq(NONE, WHITE, RED),
			ANSI_Colors[k],
			ansi_seq(NONE, BLACK, WHITE));
		for (j = 0; j < COLOR_NUM; ++j) {
			for (ii = 0; ii <= ATTRIB_NUM; ++ii) {
				i = ii % ATTRIB_NUM;
				(void)printf("%s%s",
					ansi_seq(i,j,k), attribchar[i]);
			}
			if (j == COLOR_NUM - 1) {
				(void) printf("%s\n",
					ansi_seq(NONE, BLACK, WHITE));
			} else {
				(void) printf("%s  ",
					ansi_seq(NONE, BLACK, WHITE));
			}
		}
	}

	(void) printf("\n");

	for (i = 0; i < ATTRIB_NUM; ++i) {
		(void) printf("\n%s%-8s \n",
			ansi_seq(NONE, WHITE, MAGENTA),
			ANSI_Attributes[i]);
		(void) printf("%s%-8s ",
			ansi_seq(NONE, WHITE, MAGENTA),
			" ");
		for (j = 0; j < COLOR_NUM - 1; ++j) {
			(void) printf("%-8s", ANSI_Colors[j]);
		}
		(void) printf("%s \n", ANSI_Colors[COLOR_NUM - 1]);

		for (k = 0; k < COLOR_NUM; ++k) {
			(void) printf("%s%-8s %s",
				ansi_seq(NONE, WHITE, RED),
				ANSI_Colors[k],
				ansi_attribute(i));
			for (j = 0; j < COLOR_NUM; ++j) {
				(void)printf("%s%-8s",
					ansi_color(j, k), word);
			}
			(void) printf("%s\n", ansi_seq(NONE, BLACK, WHITE));
		}
	}

	return 0;
}
