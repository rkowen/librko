static const char AUTHOR[]="@(#)netup 1.0 03/26/96 R.K.Owen,Ph.D.";

/* :*: gcc -o netup netup.c -I.. -L.. -lrko
 * :*:END:
 */
/* netup - connects to a host & port number (defaults to ECHO_PORT)
 *	with the TCP protocol.  The hostname can either be
 *	host.domain or 4 octet IP address.  If the port number is
 *	the ECHO_PORT, then it tests the connection.  returns 0 if
 *	the connection is up, else returns > 0 otherwise.
 *	This is mostly useful for a SLIP connection to test whether
 *	it's still up or not and to take corrective action or not.
 *
 * author	R.K.Owen,Ph.D.	03/36/94
 */
/*
 *********************************************************************
 *
 *     This software is copyrighted by R.K.Owen,Ph.D. 1996
 *
 * The author, R.K.Owen, of this software is not liable for any
 * problems WHATSOEVER which may result from use  or  abuse  of
 * this software. The author, R.K.Owen, grants unlimited rights
 * to anyone who uses and modifies this  software  for  private
 * non-commercial  use  as  long  as  this copyright and rights
 * notice remains in this software and is made available to all
 * recipients of this software.
 *
 * last known email: rk@owen.sj.ca.us
 *                   rkowen@ckns.net
 *
 *********************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "librko.h"

#define ECHO_PORT 7
#define BUFLEN 128

void usage(char *prgnm) {
	(void) printf(
"\n"
"%s - tests whether a host connection is up or not.\n"
"\n"
"usage: %s [-v][-p port#] host\n"
"\n"
"\twhere the host is either a host.domain name or an IP address\n"
"\tif the port number is not given then it defaults to the ECHO_PORT=7\n"
"\tand will test it with an echo test.  All other port numbers will do\n"
"\ta make-&-break test of the connection.\n"
"\tThe normal operation is to just set the return status to 0 if success\n"
"\tor non-zero on failure.\n"
"\tThe -v option causes some verbose output to stdout\n\n",
	prgnm, prgnm);
	exit(-1);
};

int main(int argc, char *argv[]) {
	int efd, n;
	char send[BUFLEN], receive[BUFLEN];
	int verbose = 0;
	int port = ECHO_PORT;
	char *ptr;

	char *ProgName = *argv++;
	char *Host = (char *) NULL;

	if (*argv == (char *) NULL) usage(ProgName);

	while ((ptr = *argv++) != (char *) NULL) {
		if (*ptr == '-') {
			ptr++;
			switch(*ptr) {
			case 'v':
			case 'V':
				verbose = 1;
				break;
			case 'p':
			case 'P':
				port = (int) atol(*argv++);
				if (port <= 0) usage(ProgName);
				break;
			default:
				usage(ProgName);
			}
		} else {
			/* should be at a host name */
			Host = ptr;
			break;
		}
	}

	if ((efd = tcp_connect(Host, port)) < 0) {
		if (verbose) rkoperror(ProgName);
		return 1;
	}
	if (port == ECHO_PORT) {
		/* send message to echo port */
		(void) sprintf(send, "%ld\n", getpid());
		n = strlen(send) + 1;
		if (write(efd, send, n) != n) {
			if (verbose) perror(ProgName);
			return 2;
		} else {
			(void) memset(receive, '\0', BUFLEN);
		}
		/* receive message it again */
		if ((n = read(efd, receive, BUFLEN)) < 0) {
			if (verbose) perror(ProgName);
			return 3;
		} else {
			receive[n] = '\0';
			if (strncmp(send, receive, BUFLEN)) {
				if(verbose) (void) fprintf(stderr,
					"%s : echo test fails!\n", ProgName);
				return 4;
			}
		}
	}
	/* close out connection */
	if (tcp_close(efd) < 0) {
		if (verbose) {
			rkocaterror(" : ");
			rkocaterror(Host);
			rkoperror(ProgName);
		}
		return 5;
	}
	
	if (verbose) (void) printf("%s : port %d is active\n", Host, port);
	return 0;	/* sweet success */
}
