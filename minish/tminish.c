/* tminish.c - test minish (mini-shell)
 */

#include <stdio.h>
#include <string.h>

#define MAXBUF 256

int main() {
	FILE *shtestfile;
	FILE *shtestdat;
	char buffer[MAXBUF];
	char commandline[MAXBUF];
	int retdat = 0, sigdat = 0;
	int retval = 0, sigval = 0;

	if (!(shtestfile = freopen("shtest.out", "a", stdout))) {
		fprintf(stderr,"Can't open shtest.out file for output\n");
		return 1;
	}

	if (!(shtestfile = freopen("shtest.dat", "r", stdin))) {
		fprintf(stderr,"Can't open shtest.dat file for input\n");
		return 2;
	}

	while (!(fgets(buffer,MAXBUF,stdin), feof(stdin))) {
		sscanf(buffer," %d  %d %[^\n]\n",
			&retdat, &sigdat, commandline);
		retval = minish(commandline, &sigval);
		if (retdat == 0 && retval != 0) rkoperror("main");
		if (retdat != 0) rkoperror("main");
		printf("ret=%3d sig=%3d\n", retval, sigval); fflush(stdout);
	}
	return 0;
}
