static char USMID[]="%W%";
static const char RCSID[]="@(#)$Id: wcstrcmp.c,v 1.2 1998/10/14 15:16:10 rk Exp $";
static char AUTHOR[]="@(#)wcstrcmp 1.1 08/09/95 R.K.Owen,PhD";

/* wcstrcmp - compares two strings, where one can contain wildcard characters.
 *   the allowed wild cards are '*' to match any number of characters and
 *   '?' to match a single character.  These wild card characters can be
 *   escaped by preceding them with a '\'.  A '\\' before a wild card has
 *   no effect on the wild card and the '\\' is treated as a single '\'
 *   (escaping the escape character).  Remember an extra '\' may be required
 *   for each of the '\' for the 'C' compilation parsing.
 *   If there is a match then it returns 0, else it is non-zero.
 *
 * author	R.K.Owen,Ph.D.	08/09/95
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
 *                   rkowen@ckns.net
 *
 *********************************************************************
 */

/* define TEST to create a stand-alone program to test the routine */
#if 0
#  define TEST
#endif

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef RKOERROR
#  include "librko.h"
#endif

/* set ptr to a valid character in the wild card stream */
static const char* wc_char(const char *ptr) {
	if(*ptr == '\\') {
		return ++ptr;
	} else {
		return ptr;
	}
}

int wcstrcmp(const char *str, const char *wcstr) {
	const char *ptr, *pstr = str, *pwcstr = wcstr;
	int retval = -1;

/* check for any wildcard chars in wcstr */
	if (!(strpbrk(wcstr, "?*"))) {
		/* no wildcards to deal with ... simple strcmp */
		return strcmp(str, wcstr);
	} else {
		while (*pstr != '\0' && *pwcstr != '\0') {
			if (*pwcstr == '*') {
				/* special case for trailing '*' */
				if ( *(pwcstr + 1) == '\0') {
					return 0;
				}
				/* look for a char that matches */
				do {
					pwcstr = wc_char(++pwcstr);
					if (*pwcstr == '\0' ||
						(ptr = strchr(pstr,*pwcstr))
						== NULL) break;
					pstr = ptr;
					retval = wcstrcmp(pstr,pwcstr);
				} while (retval != 0);
				return retval;
			} else if (*pwcstr == '?') {
				/* match single char wildcard */
				pwcstr++;
				pstr++;
				continue;
			} else if (pwcstr = wc_char(pwcstr),*pstr == *pwcstr) {
				/* match single char */
				pwcstr++;
				pstr++;
				continue;
			} else {
				return (int) (*pstr - *pwcstr);
			}
		}
		/* special case for trailing '*' */
		if (*pwcstr == '*') return 0;
		else return (int) (*pstr - *pwcstr);
	}
#ifdef RKOERROR
	(void) rkocpyerror("Should not reach this line in wcstrcmp\n");
	rkoperror("wcstrcmp");
	rkoerrno = RKOGENERR;
#endif
	return retval;
}

#ifdef TEST

int main() {
	char test[][3][20] = {
	/*	str,		wc str,			match */
		{"abcdefgh",	"abcdefgh",		1	},
		{"abcdefgh",	"abcdefgi",		0	},
		{"abcdefgh",	"abcdef",		0	},
		{"abcdefgh",	"ab*gh",		1	},
		{"abcdefgh",	"ab*gi",		0	},
		{"abcdefgh",	"ab*ghi",		0	},
		{"abcdefgh",	"ab*g",			0	},
		{"abcdefgh",	"a??d??gh",		1	},
		{"abcdefgh",	"a??d??gi",		0	},
		{"abcdefgh",	"a??d??ghi",		0	},
		{"abcdefgh",	"a??d??g",		0	},
		{"abcdefgh",	"a??d*gh",		1	},
		{"abcdefgh",	"a??d*gi",		0	},
		{"abcdefgh",	"a??d*ghi",		0	},
		{"abcdefgh",	"a??d*g",		0	},
		{"abcdefgh",	"a??d*h",		1	},
		{"abcdefgh",	"a??d*i",		0	},
		{"abcdefgh",	"ab*",			1	},
		{"abcdefgh",	"*gh",			1	},
		{"abcdefgh",	"*",			1	},
		{"abcdefgh",	"????????",		1	},
		{"abcdefgh",	"???????",		0	},
		{"abcdefgh",	"?????????",		0	},
		{"ab*defgh",	"ab\\**gh",		1	},
		{"abcdefgh",	"ab\\**gh",		0	},
		{"ab*defgh",	"ab\\*d*h",		1	},
		{"ab*d?fgh",	"ab\\*d\\?*h",		1	},
		{"ab*defgh",	"ab\\*d\\?*h",		0	},
		{"abcabcgh",	"a*cgh",		1	},
		{"abcdefgh",	"*def*",		1	},
		{"abcdefgh",	"*defgh*",		1	},
		{"abcdefgh",	"abc*defgh",		1	},
	};
	int i, result, totresult = 0, num = sizeof(test)/sizeof(test[0]);
	for (i = 0; i < num; ++i) {
		result = wcstrcmp(test[i][0], test[i][1]);
		if (result != 0 ) {
			if (*test[i][2] == 0) printf("OK   :");
			else {
				totresult++;
				printf("FAIL :");
			}
		} else {
			if (*test[i][2] == 1) printf("OK   :");
			else {
				totresult++;
				printf("FAIL :");
			}
		}
		printf("%15.15s %15.15s : %d\n", test[i][0], test[i][1],
			result);
	}
	fprintf(stderr,"\nThere were ");
	if (totresult == 0) fprintf(stderr,"NO");
	else fprintf(stderr,"%d", totresult);
	fprintf(stderr," wildcard test failures.\n\n");
	return totresult;
}

#endif /* TEST */
