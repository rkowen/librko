#include <stdio.h>
#include <stdlib.h>
#include "strchop.h"
#ifdef RKOERROR
#  include "rkoerror.h"
#endif

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
