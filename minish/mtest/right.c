/* quick program to echo the command line and return with EXIT_SUCCESS
 * by R.K.Owen,Ph.D.,  10/07/91
* :EXE: cc right.c
* :EXE: mv a.out right
* :EXE:END:
 */
#include <stdio.h>
#include <stdlib.h>

#ifndef EXIT_SUCCESS
#  define EXIT_SUCCESS 0
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
	return EXIT_SUCCESS;
}
