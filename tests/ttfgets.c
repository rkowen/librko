#define _POSIX_SOURCE

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>		/* exit() */
#include <signal.h>
#include <setjmp.h>
#include "librko.h"

main(int argc, char **argv) {
	char buffer[512];
	int status ;
	int seconds = 5 ;
	
	fprintf (stderr, "You have %d seconds to enter some text: ", seconds);

	status = timedfgets(buffer, sizeof (buffer), stdin, seconds) ;

	if (status < 0) {
		if (rkoerrno == RKO_OK) {
			if (errno == EINTR) {
				fprintf (stderr,
					"Time out! %d seconds have elapsed!\n",
					seconds);
			}
			if (errno == EIO) {
				fprintf (stderr,
				"No data read, hit EOF within %d seconds.\n",
					seconds);
			}
		} else {
			rkoperror(argv[0]);
		}
	} else {
		fprintf (stderr, "\nInput received: %s", buffer);
	}

	exit (status) ;
}
