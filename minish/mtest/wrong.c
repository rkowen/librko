/* quick program to echo the command line and return with EXIT_FAILURE
 * by R.K.Owen,Ph.D.,  10/07/91
* :EXE: cc wrong.c
* :EXE: mv a.out wrong
* :EXE:END:
 */
#include <stdio.h>
#include <stdlib.h>

#ifndef EXIT_FAILURE
#  define EXIT_FAILURE 1
#endif

#ifdef __STDC__
main(int argc, char *argv[])
#else
main(argc,argv)
int argc;
char *argv[];
#endif
{
	int i;
	for (i=1; i < argc; i++) printf("%s ", argv[i]);
	printf("\n");
	fflush(stdout);
	return EXIT_FAILURE;
}
