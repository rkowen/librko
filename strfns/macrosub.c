static const char RCSID[]="@(#)$Id: macrosub.c,v 1.6 1999/11/02 18:23:55 rk Exp $";
static char AUTHOR[]="@(#)macrosub.c 1.0 08/30/94 R.K.Owen,PhD";

/* macstrncpy(string1,string2,n) - Macro string copy
 *   Copies string2 to string1 where string2 may contain single letter
 *   macros (e.g. $a,$<, etc.).  It can recursively expand macros upto
 *   MACRODEPTH levels.  It will process no more than n characters
 *   (usually the size of string1).
 *   Macros are defined with $m=string'\0', where the lack of spaces
 *   between $m and = and string is very significant and 'm' is any
 *   single macro character.  Also anything before the macro is
 *   processed normally.
 *   Macros maybe pre-set by adding
 *     extern char **MacroStr;
 *     extern int MacroTrf[128];
 *     ...
 *     MacroStr[MacroTrf['m']] = "string"
 *   where 'm' is any single macro character.  However, if 'm' is not
 *   a letter or '@', '<', '*' then it will require some modification
 *   of this source.
 *   Note, unlike strncpy, macstrncpy only append a single '\0' at
 *   the end of the string1; and it returns (char *)string1 unless
 *   there is an error then it returns (char *) NULL.
 *
 * author	R.K.Owen,Ph.D.	08/31/94
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
 * last known email: librko@kooz.sj.ca.us
 *                   rk@owen.sj.ca.us
 *
 *********************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "librko.h"

#define MACRODEPTH	10
#define MACRONUM	54
#define MACROCHAR	'$'

#if !(defined(__STDC__) || defined(_AIX))
#  define void
#  define const
#endif

char *MacroStr[MACRONUM] = {			/* macro substitutions */
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0};

int MacroTrf[128] = {		/* translate ASCII character to index */
	-99,	-1,	-1,	-1,	-1,	-1,	-1,	-1,
	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,
	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,
	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,
	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,
	-1,	-1,/***/54,	-1,	-1,	-1,	-1,	-1,
	-1,	-1,	-1,	-1,	-1,	-1,	-1,	-1,
	-1,	-1,	-1,	-1,/*<*/53,	-1,	-1,	-1,
   /*@*/52,/*A*/ 0,	 1,	 2,	 3,	 4,	 5,	 6,
   /*H*/ 7,	 8,	 9,	10,	11,	12,	13,	14,
   /*P*/15,	16,	17,	18,	19,	20,	21,	22,
   /*X*/23,	24,	25,	-1,	-1,	-1,	-1,	-1,
	-1,/*a*/26,	27,	28,	29,	30,	31,	32,
   /*h*/33,	34,	35,	36,	37,	38,	39,	40,
   /*p*/41,	42,	43,	44,	45,	46,	47,	48,
   /*x*/49,	50,	51,	-1,	-1,	-1,	-1,	-1};

#if defined(__STDC__) || defined(_AIX)
char *macstrncpy(char *string, const char *mstring, size_t n)
#else
char *macstrncpy(string, mstring, n)
char *string;
char *mstring;
int n;
#endif
{
	char *ptr = string;
	const char *macroptr[MACRODEPTH];	/* ptr to level */
	int macrolevel = 0;		/* current macro level */
	int nchars = 0;			/* number of characters processed */
	int len;
	char c, m, msgbuf[256];
	int i;
/* initialize */
	if (n == 0) return string;
	macroptr[0] = mstring;
	for (i = 1; i < MACRODEPTH; ++i) macroptr[i] = NULL;

/* descend into the macro levels */
	while (macrolevel >= 0) {
		while ((c = *(macroptr[macrolevel])) != '\0') {
			if (c == MACROCHAR) {			/* macro indicator */
				(macroptr[macrolevel])++; /* advance ptr */
				c = *(macroptr[macrolevel]);
				if (c == MACROCHAR) {		/* $$ = '$' */
					nchars++;
					if (nchars >= n) { /* no more chars */
						return string;
					}
					*ptr++ = c;
					(macroptr[macrolevel])++;
					continue;
				}
				m = (int) c;		/* save macro letter */
				if (MacroTrf[m] < 0) {	/* invalid macro */
#ifdef RKOERROR
					rkoerrno = RKOUSEERR;
					(void) sprintf(msgbuf,
				"macrostr : Invalid macro character: `%c'",
						c);
					(void) rkocpyerror(msgbuf);
#endif
					*ptr = '\0';
					return (char *) NULL;
				}
				(macroptr[macrolevel])++;	/* next char */
				c = *(macroptr[macrolevel]);

				/* set macro (only at top level) */
				if (c == '=' && macrolevel == 0) {
					(macroptr[0])++;
					len = strlen(macroptr[0]);
					free(MacroStr[MacroTrf[m]]);
					MacroStr[MacroTrf[m]] = 
						(char *) malloc(len+1);
					strcpy(MacroStr[MacroTrf[m]],
						macroptr[0]);
					break;
				}
				/* start processing at macro */
				if (++macrolevel >= MACRODEPTH) {
#ifdef RKOERROR
					rkoerrno = RKOUSEERR;
					(void) sprintf(msgbuf,
				"macrostr : Macro levels nested too deep: `%d'",
						macrolevel);
					(void) rkocpyerror(msgbuf);
#endif
					return (char *) NULL;
				}
				macroptr[macrolevel] = MacroStr[MacroTrf[m]];
				continue;
			}
			nchars++;
			*ptr++ = c;
			if (nchars >= n) { /* no more chars can be processed */
				return string;
			}
			(macroptr[macrolevel])++;	/* next char */
		}
		macrolevel--;			/* decrease macro level */
	}
	*ptr = '\0';
	return string;
}
