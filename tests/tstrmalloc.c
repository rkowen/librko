
#include <stdio.h>
#define RKOERROR
#include "rkoerror.h"
#include "strmalloc.h"

size_t lengthstr(const char *str) {
	size_t sizeit;
	(void) memcpy(&sizeit, (void *)(str - sizeof(size_t)), sizeof(size_t));
	return sizeit;
}

int comparestr(const char *malstr, const char *teststr) {
	while (*teststr != '\0') {
		if (*teststr == '_' && *malstr != '\0') return 1;
		if (*teststr != '_' && *teststr != *malstr) return -1;
		teststr++;
		malstr++;
	}
	return 0;
}

int trystr(const char *malstr, const char *teststr) {
	int errval = 0;
	if ((errval = comparestr(malstr, teststr))) {
		printf("FAIL: strmalloc: %d : `%s' != `%s'\n", errval, malstr, teststr);
		return 1;
	} else if (lengthstr(malstr) != strlen(teststr)+1) {
		printf("FAIL: strmalloc: %d-`%s' != %d-%s'\n",
			(int) lengthstr(malstr), malstr,
			(int) strlen(teststr), teststr);
		return 1;
	} else {
		printf("OK  : strmalloc: `%s'\n", malstr);
		return 0;
	}
}

int tryfree(int errvalue, char *errmsg) {
	if (rkoerrno != errvalue) {
		printf("FAIL: strfree: incorrect rkoerrno = %d != %d\n",
			rkoerrno, errvalue);
		return 1;
	} else if (strcmp(errmsg, rkostrerror())) {
		printf("FAIL: strfree: incorrect error message : %s\n",
			rkostrerror());
		return 1;
	} else {
		printf("OK  : strfree: correct rkoerrno = %d\n",
			rkoerrno);
		return 0;
	}
}

int main() {
	char *test[6];
	char *teststr[7] = {
		"",
		"This is a Test String",
		"This is a Test_String",
		"This is another Test String",
		"This is another Test_String",
		"This",
		"is"
	};

	int results = 0;

	test[0] = (char *) NULL;
	test[1] = strmalloc(teststr[1]);
	results += trystr(test[1], teststr[1]);
	test[2] = strmalloc("This is another Test String");
	results += trystr(test[2], teststr[3]);
	test[3] = test[1];
	results += trystr(test[3], teststr[1]);
	test[1][14] = '\0';
	results += trystr(test[1], teststr[2]);
	test[2][20] = '\0';
	results += trystr(test[2], teststr[4]);
	test[4] = strnmalloc(test[3],4);
	results += trystr(test[4], teststr[5]);
	test[5] = strnmalloc(teststr[3]+2,2);
	results += trystr(test[5], teststr[6]);

	strfree(&test[0]);
#if 0
	results += tryfree(RKOUSEERR, "strfree : null input!");
#else
	results += tryfree(RKO_OK, "");
#endif
	strfree(&test[1]);
	results += tryfree(RKO_OK, "");
	strfree(&test[2]);
	results += tryfree(RKO_OK, "");
	strfree(&test[3]);
	results += tryfree(RKOUSEERR, "strfree : invalid string object!");
	strfree(&test[4]);
	results += tryfree(RKO_OK, "");
	strfree(&test[5]);
	results += tryfree(RKO_OK, "");
	/* bunch of error stuff */
	test[1] = strmalloc((char *) NULL);
	results += tryfree(RKOUSEERR, "strmalloc : NULL string!");
	test[1] = strnmalloc((char *) NULL, 3);
	results += tryfree(RKOUSEERR, "strnmalloc : NULL string!");

	if (results) {
		printf("There were %d test failures\n", results);
	} else {
		printf("There were no test failures\n");
	}
	return results;
}
