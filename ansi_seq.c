static char USMID[]="%W%";
static const char RCSID[]="@(#)$Id: ansi_seq.c,v 1.1 1998/10/07 16:19:56 rk Exp $";
static char AUTHOR[]="@(#)ansi_seq 1.1 12/21/95 R.K.Owen,PhD";

/* ansi_seq - creates a char * string that is the ANSI color and
 *	attribute sequence.
 * An ANSI sequence consists of one or more of the following numeric codes:
 * Attribute codes: 
 * "00"=none "01"=bold "04"=underscore "05"=blink "07"=reverse "08"=concealed
 * Text color codes:
 * 30=black 31=red 32=green 33=yellow 34=blue 35=magenta 36=cyan 37=white
 * Background color codes:
 * 40=black 41=red 42=green 43=yellow 44=blue 45=magenta 46=cyan 47=white
 *
 * an ANSI sequence is initiated with "^[[xx;xx;xxm"  where xx are the
 * values specified above and ^[ refers to an <esc>
 *
 * by R.K. Owen, Ph.D.  12/21/95
 */
/*
 *********************************************************************
 *
 *     This software is copyrighted by R.K.Owen,Ph.D. 1997
 *
 * The author, R.K.Owen, of this software is not liable for any
 * problems WHATSOEVER which may result from use  or  abuse  of
 * this software. The author, R.K.Owen, grants unlimited rights
 * to anyone who uses and modifies this  software  for  private
 * non-commercial  use  as  long  as  this copyright and rights
 * notice remains in this software and is made available to all
 * recipients of this software.
 *
 * last known email: rk@owen.sj.ca.us
 *                   rkowen@kudonet.com
 *
 *********************************************************************
 */

#include <stdio.h>
#include <unistd.h>
#include "librko.h"

const char *ANSI_Attributes[ATTRIB_NUM] = {
  "none", "bold", "underscore", "blink", "reverse", "concealed"};
const char *ANSI_Colors[COLOR_NUM] = {
  "black", "red", "green", "yellow", "blue", "magenta", "cyan", "white"};

static char *attribnum[ATTRIB_NUM] = {
  "00",   "01",   "04",         "05",    "07",      "08"};
static char *fgcolornum[COLOR_NUM] = {
  "30",    "31",   "32",   "33",     "34",   "35",      "36",   "37"};
static char *bgcolornum[COLOR_NUM] = {
  "40",    "41",   "42",   "43",     "44",   "45",      "46",   "47"};

static int IS_TTY_OUT;

static void is_ansi_tty(void) {
#ifndef NO_ISATTY
	if (isatty(STDOUT_FILENO)) {
		IS_TTY_OUT = 1;
	} else {
		IS_TTY_OUT = -1;
	}
#else
	IS_TTY_OUT = 1;		/* always set for ANSI sequences */
#endif
}

int set_ansi_tty(int ans) {
	int t;
	if (!IS_TTY_OUT) is_ansi_tty();
	t = (IS_TTY_OUT + 1)/2;	/* saves it as 0 or 1 */
	if (ans)	IS_TTY_OUT = 1;
	else		IS_TTY_OUT = -1;
	return t;			/* returns the old setting */
}

const char *ansi_seq(ansi_attributes a, ansi_colors b, ansi_colors f) {
	static char buffer[ANSISEQ_NUM][ANSISEQ_LEN];
	static int current;	/* initialized to 0 */

	if (!IS_TTY_OUT) is_ansi_tty();
	if (IS_TTY_OUT == 1) {
		current++;
		current %= ANSISEQ_NUM;
		(void) sprintf(buffer[current], "\x1B[%s;%s;%sm",
			attribnum[a], bgcolornum[b], fgcolornum[f]);
		return (const char *) buffer[current];
	} else {
		*buffer[current] = '\0';
		return (const char *) buffer[current];
	}
}

const char *ansi_color(ansi_colors b, ansi_colors f) {
	static char buffer[ANSISEQ_NUM][ANSISEQ_LEN];
	static int current;	/* initialized to 0 */

	if (!IS_TTY_OUT) is_ansi_tty();
	if (IS_TTY_OUT == 1) {
		current++;
		current %= ANSISEQ_NUM;
		(void) sprintf(buffer[current], "\x1B[%s;%sm",
			bgcolornum[b], fgcolornum[f]);
		return (const char *) buffer[current];
	} else {
		*buffer[current] = '\0';
		return (const char *) buffer[current];
	}
}

const char *ansi_fgcolor(ansi_colors f) {
	static const char *fgcolorseq[COLOR_NUM] = {
	"\x1B[30m","\x1B[31m","\x1B[32m","\x1B[33m",
	"\x1B[34m","\x1B[35m","\x1B[36m","\x1B[37m"};
	static char buffer = '\0';

	if (!IS_TTY_OUT) is_ansi_tty();
	if (IS_TTY_OUT == 1) {
		return fgcolorseq[f];
	} else {
		return (const char *) &buffer;
	}
}

const char *ansi_bgcolor(ansi_colors b) {
	static const char *bgcolorseq[COLOR_NUM] = {
	"\x1B[40m","\x1B[41m","\x1B[42m","\x1B[43m",
	"\x1B[44m","\x1B[45m","\x1B[46m","\x1B[47m"};
	static char buffer = '\0';

	if (!IS_TTY_OUT) is_ansi_tty();
	if (IS_TTY_OUT == 1) {
		return bgcolorseq[b];
	} else {
		return (const char *) &buffer;
	}
}

const char *ansi_attribute(ansi_attributes a) {
	static const char *attribseq[ATTRIB_NUM] = {
	"\x1B[00m","\x1B[01m","\x1B[04m","\x1B[05m","\x1B[07m","\x1B[08m"};
	static char buffer = '\0';

	if (!IS_TTY_OUT) is_ansi_tty();
	if (IS_TTY_OUT == 1) {
		return attribseq[a];
	} else {
		return (const char *) &buffer;
	}
}
