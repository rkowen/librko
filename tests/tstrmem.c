#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#include "librko.h"

int main() {
	char *name[] = {
		"aaa",
		"BbBB",
		"cCcCCC",
		"ddd"};
	char *NaMeSpAcE, **namespace;
	int i;
	size_t t, numentry, max_len = 0;

	numentry = sizeof(name)/sizeof(name[0]);

	for (i=0; i < numentry; ++i) {
		t = strlen(name[i]);
		max_len = (t < max_len ? max_len : t);
	}

	if (strmemalloc(&NaMeSpAcE, &namespace, numentry, max_len)) {
		rkoperror("TEST");
	}

	for (i=0; i < numentry; ++i) {
		strcpy(namespace[i],name[i]);
	}

	for (i=0; i < numentry; ++i) {
		printf("%s.\n", namespace[i]);
	}
	i=0;
	while (namespace[i] != (char *) NULL) {
		printf("--%s.\n", namespace[i++]);
	}

	strmemfree(&NaMeSpAcE, &namespace);
	return EXIT_SUCCESS;
}
