/* :OK: cc tmemdebug.c
 * :OK: a.out
 * :OK: && echo ran OK
 * :OK:END:
 */
/* :*: cc -c memdebug.c
 * :*: cc -DMEMDEBUG tmemdebug.c memdebug.o
 * :*: a.out
 * :*:END:
 */
#include <stdlib.h>
#include <stdio.h>

#define MEMDEBUG
#include "memdebug.h"		/* include this AFTER stdlib.h */

typedef struct Trial {
	int a;
	double b;
	char c[5];
	struct Trial *next;
} trial;

int main() {

	size_t num = 20;
	int *ia = (int *) NULL;
	double *da = (double *) NULL;
	char *ca = (char *) NULL;
	trial *ta = (trial *) NULL;

	 printf("Some regular output\n");

	memdebug_output(stdout);

	if ((ia = (int *) malloc(num * sizeof(int))) == NULL)
		(void) fprintf(stderr,"malloc error\n");
	free(ia);
	ia = (int *) NULL;

	if ((ia = (int *) malloc(2 * num * sizeof(int))) == NULL)
		(void) fprintf(stderr,"malloc error\n");
	if ((ia = (int *) realloc(ia, 4 * num * sizeof(int))) == NULL)
		(void) fprintf(stderr,"realloc error\n");

	if ((da = (double *) malloc(num * sizeof(double))) == NULL)
		(void) fprintf(stderr,"malloc error\n");

	if ((ta = (trial *) calloc(num, sizeof(trial))) == NULL)
		(void) fprintf(stderr,"malloc error\n");
	free(ta);
	ta = (trial *) NULL;

	if ((ta = (trial *) calloc(2 * num, sizeof(trial))) == NULL)
		(void) fprintf(stderr,"malloc error\n");

	if ((da = (double *) realloc(da, 2 * num * sizeof(double))) == NULL)
		(void) fprintf(stderr,"realloc error\n");
	free(ia);
	ia = (int *) NULL;

	if ((ca = (char *) malloc(num * sizeof(char))) == NULL)
		(void) fprintf(stderr,"malloc error\n");

	 printf("Some more regular output\n");

	free(da);
	free(ta);
	free(ca);
	free(ia);

	return 0;
}
