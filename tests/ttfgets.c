#define _POSIX_SOURCE

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>		/* exit() */
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#include "librko.h"

main(int argc, char **argv) {
	char buffer[512];
	int status;
	int seconds = 4 ;
	int i;
	
	for (i=1; i <= 2; ++i ) {
		fprintf (stderr, "You have %d seconds to enter some text: ",
		i*seconds);

		status = timedfgets(buffer, sizeof (buffer), stdin, i*seconds);

		if (status < 0) {
			if (rkoerrno == RKO_OK) {
				if (errno == EINTR) {
					fprintf (stderr,
				"Time out! %d seconds have elapsed!\n",
						i*seconds);
				}
				if (errno == EIO) {
					fprintf (stderr,
				"No data read, hit EOF within %d seconds.\n",
						i*seconds);
				}
			} else {
				rkoperror(argv[0]);
			}
		} else {
			fprintf (stderr, "Input received: %s", buffer);
		}
	}

	return status;
}
