/* tmemdbg - performs some common memory alloc/frees to
 * test out memdbg
 */

#include <stdio.h>
#include <stdlib.h>
#define MEMDEBUG
#include "memdebug.h"

#define LEN 100

int main () {
	char *a[LEN], *b[LEN], *c[LEN];
	int i;
	int size = 8;

	memdebug_output(stdout);

	for (i=0; i < LEN; i++)
		c[i] = (char *) NULL;

	/* alloc some memory */
	printf("===alloc===\n");
	for (i=0; i < LEN; i++)
		if(!(a[i] = (char *) malloc(size)))
			printf("FAIL: malloc:%d\n",i);
		else
			printf("OK  : malloc:%d\n",i);

	for (i=0; i < LEN; i++)
		if(!(b[i] = (char *) calloc(size,1)))
			printf("FAIL: calloc:%d\n",i);
		else
			printf("OK  : calloc:%d\n",i);

	for (i=0; i < LEN; i++)
		if(!(c[i] = (char *) realloc(c[i],size)))
			printf("FAIL:realloc:%d\n",i);
		else
			printf("OK  :realloc:%d\n",i);

	/* realloc */
	printf("===realloc===\n");
	for (i=0; i < LEN; i++)
		if(!(a[i] = (char *) realloc(a[i],2*size)))
			printf("FAIL:realloc:%d\n",i);
		else
			printf("OK  :realloc:%d\n",i);

	for (i=0; i < LEN; i++)
		if(!(b[i] = (char *) realloc(b[i],2*size)))
			printf("FAIL:realloc:%d\n",i);
		else
			printf("OK  :realloc:%d\n",i);

	for (i=0; i < LEN; i++)
		if(!(c[i] = (char *) realloc(c[i],2*size)))
			printf("FAIL:realloc:%d\n",i);
		else
			printf("OK  :realloc:%d\n",i);

	/* free */
	printf("===free===\n");
	for (i=0; i < LEN; i++)
		free(a[i]);

	for (i=0; i < LEN; i++)
		free(b[i]);

	for (i=0; i < LEN; i++)
		free(c[i]);

	return 0;
}
