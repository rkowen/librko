static const char RCSID[]="@(#)$Id: strchop.c,v 1.7 2002/08/23 15:18:11 rk Exp $";
static const char AUTHOR[]="@(#)strchop 1.0 1995/10/05 R.K.Owen,Ph.D.";

/* strchop - chops a string at the spaces (as determined by isspace())
 *	and behaves in the same manner as strtok in some aspects.
 *
 * prototype:
char *strchop(const char *string, int chop_len,
	size_t *str_len, char **next);
 *
 *	string		= string to chop up else NULL if continuing
 *			  with the same one from a previous call.
 *			  the "string" contents can not be changed between
 *			  successive calls with NULL.
 *	chop_len	= Maximum length of returned strings, if <= 0 then
 *			  free up internal memory
 *	str_len		= returned string length if not NULL
 *
 *	next		= pointer to the start of next text area of "string"
 *			  if not NULL.  If no more is left then returns NULL.
 *
 *	strchop		= returns a pointer to an internal area that contains
 *			  the "chopped" string with length <= chop_len
 *			  else returns NULL if no more to do.
 *
 * author	R.K.Owen,Ph.D.	1995/10/05
 *
 */
/** ** Copyright *********************************************************** **
 ** 									     **
 ** Copyright 1995 by R.K.Owen,Ph.D.		                      	     **
 ** last known email: librko@kooz.sj.ca.us				     **
 **                   rk@owen.sj.ca.us					     **
 ** see LICENSE.LGPL, which must be provided, for details		     **
 ** 									     **
 ** ************************************************************************ **/
/*
 * Define:	RKOERROR and load with librko.a for error handling
 *		TEST to run a stand-alone test program
 */

#if 0
#  define TEST
#endif

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#ifdef MEMDEBUG
#  include "memdebug.h"
#endif
#ifdef RKOERROR
#  include "rkoerror.h"
#endif

char *strchop(const char *string, int chop_len,
	size_t *str_len, char **next) {

	static char *chop_buffer = (char *) NULL;
	static const char *start;
	const char *end, *start_next;
	size_t len;

	if (chop_len <= 0) {
		free(chop_buffer);
		start = end = (char *) NULL;
		chop_buffer = (char *) NULL;
#ifdef RKOERROR
		rkoerrno = RKO_OK;
#endif
		return (char *) NULL;
	}
/* if first time with this string, dealloc any memory & reallocate space */
	if (string != (char *) NULL) {
		free(chop_buffer);
		start = end = (char *) NULL;
		if ((chop_buffer = malloc(chop_len + 1)) == (char *) NULL) {
			/* memory error */
#ifdef RKOERROR
			(void) rkocpyerror("strchop : malloc error");
			rkoerrno = RKOMEMERR;
#endif
			return (char *) NULL;
		}
		*chop_buffer = '\0';
		start = string;
	} else if (chop_buffer == (char *) NULL) {
			/* user error */
#ifdef RKOERROR
			(void) rkocpyerror(
			"strchop : user error - buffer was deallocated");
			rkoerrno = RKOUSEERR;
#endif
			return (char *) NULL;
	}
/* strip initial spaces */
	while (isspace((int) *start)) start++;

/* find a good place to put end */
	len = strlen(start);
	if (len <= chop_len) {
/* it's short enough already - return what you have */
		(void) strcpy(chop_buffer, start);
		if (str_len != (size_t *) NULL) *str_len = len;
		if (next != (char **) NULL) *next = (char *) NULL;
#ifdef RKOERROR
		rkoerrno = RKO_OK;
#endif
		return chop_buffer;
	} /* else */
	start_next = end = start + chop_len;
/* work back from there */
	while (!isspace((int) *end)) { start_next = end; end--;
		if (end <= start) {	/* can't chop it */
#ifdef RKOERROR
			(void) rkocpyerror(
				"strchop : user error - string can not be "
				"chopped at given limits");
			rkoerrno = RKOUSEERR;
#endif
			return (char *) NULL;
		}
	}
/* found a space */
	if (next != (char **) NULL) *next = (char *) start_next;
/* find next non-space */
	while (isspace((int) *end)) {
		end--;
		if (end <= start) {	/* all space !? */
			*chop_buffer = '\0';
			if (str_len != (size_t *) NULL) *str_len = 0;
#ifdef RKOERROR
			rkoerrno = RKO_OK;
#endif
			return chop_buffer;
		}
	}
/* found non-space */
	len = end - start + 1;
	(void) strncpy(chop_buffer, start, len);
	*(chop_buffer + len) = '\0';
	if (str_len != (size_t *) NULL) *str_len = len;
	start = start_next;
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return chop_buffer;
}

#if defined(TEST)

#include <stdio.h>

void testfn(const char *test, int len) {
	const char *ptr;
	char *next;
	size_t str_len;

	ptr = test;
	(void) printf("\n>>>chop length = %d\n", len);
	(void) printf(
		"         1         2         3         4"
		"         5         6         7         8\n"
		"1234567890123456789012345678901234567890"
		"1234567890123456789012345678901234567890\n");
	do {
		ptr = strchop(ptr, len, &str_len, &next);
		(void) printf("%s: => %d\n", ptr, str_len);
		ptr = (char *) NULL;
	} while (next != (char *) NULL);
}

void testfn2(const char *test, int begin, int len1, int len2) {
	const char *ptr;
	char *next;
	size_t str_len;

	(void) printf("\n>>>chop length = %d@%d, %d\n", begin, len1, len2);
	(void) printf(
		"         1         2         3         4"
		"         5         6         7         8\n"
		"1234567890123456789012345678901234567890"
		"1234567890123456789012345678901234567890\n");
	ptr = test;
	do {
		ptr = strchop(ptr, len1, &str_len, &next);
		(void) printf("%s: => %d\n", ptr, str_len);
		ptr = (char *) NULL;
	} while (next != (char *) NULL && --begin > 0);
	if (next != (char *) NULL) {
		ptr = next;
		do {
			ptr = strchop(ptr, len2, &str_len, &next);
			(void) printf("%s: => %d\n", ptr, str_len);
			ptr = (char *) NULL;
		} while (next != (char *) NULL);
	}
}

int main() {
	char *test =
"This is a test of some long text to see how well strchop can do in "
"breaking it up into bite-size pieces.  It does NOT know anything about "
"punctuation or apostrophes or abbreviations.  It just breaks lines "
"at spaces (according to isspace()). However, it may have a problem "
"with tabs since that is just a single character but may equal as "
"many as 8 spaces sent to a terminal screen.";
	char *test2 = "This_is_one_long_word";
	char *tmp;

	testfn(test, 60);
	testfn(test, 50);
	testfn2(test, 1, 50, 60);
	testfn2(test, 2, 40, 60);

	tmp = strchop(test2, 10, NULL, NULL);
	if (tmp == (char *) NULL) {
#ifdef RKOERROR
		if (rkoerrno != RKO_OK) {
			rkoperror("OK - main");
		}
#else
		printf("OK - Error chopping across long word\n");
#endif
	} else {
		printf("test string 1 = %s\n", tmp);
	}

	tmp = strchop(test2, 40, NULL, NULL);
	if (tmp == (char *) NULL) {
#ifdef RKOERROR
		if (rkoerrno != RKO_OK) {
			rkoperror("FAIL - main");
		}
#else
		printf("FAIL - shouldn't have an error here\n");
#endif
	} else {
		printf("test string 2 = %s\n", tmp);
	}

	tmp = strchop(test2, -1, NULL, NULL);
	if (tmp == (char *) NULL) {
#ifdef RKOERROR
		if (rkoerrno != RKO_OK) {
			rkoperror("FAIL - main");
		}
#else
		printf("FAIL - Error deallocating buffer\n");
#endif
	} else {
		printf("test string 3 = %s\n", tmp);
	}

	tmp = strchop(NULL, 40, NULL, NULL);
	if (tmp == (char *) NULL) {
#ifdef RKOERROR
		if (rkoerrno != RKO_OK) {
			rkoperror("OK - main");
		}
#else
		printf("OK - User error\n");
#endif
	} else {
		printf("test string 4 = %s\n", tmp);
	}

	return EXIT_SUCCESS;
}
#endif /* TEST */
