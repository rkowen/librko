static const char AUTHOR[]="@(#)tcp_connect 1.0 03/26/96 R.K.Owen,Ph.D.";

/* tcp_connect - connects to a host & port number with the TCP protocol.
 *	The hostname can either be host.domain or 4 octet IP address.
 *	returns a file descriptor int.  If there is a problem then it
 *	returns < 0.
 *
 * tcp_close - closes the connection, returns 0 if successful, < 0 otherwise.
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
 *                   rkowen@kudonet.com
 *                   smbd89a@prodigy.com
 *
 *********************************************************************
 */

/* define TEST to create a stand-alone program to test the routine */
#if 0
#  define TEST
#endif

#ifdef _AIX
#  define	_POSIX_SOURCE
#  define	_XOPEN_SOURCE_EXTENDED	1
#endif
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "librko.h"

#if	!(defined(_CRAY) || defined(_AIX))
#  define HERRNO_LEN 128
/* quick routine that should have been provided with herror */
static const char *hstrerror(int hh_errno) {
	extern int h_errno;
	char *ptr;

	switch(h_errno) {
	case HOST_NOT_FOUND:	ptr = "HOST NOT FOUND";	break;
	case TRY_AGAIN:		ptr = "TRY AGAIN";	break;
	case NO_RECOVERY:	ptr = "NO RECOVERY";	break;
	case NO_DATA:		ptr = "NO DATA";	break;
	default:		ptr = "UNKNOWN";
	}
	return ptr;
}
#endif

int tcp_connect(char *hostname, int port_num) {
	struct hostent *hostinfo;
	struct sockaddr_in sock_addr;
	int sd, status;
#if !defined(_AIX)
	extern int h_errno;
#endif

	hostinfo = gethostbyX(hostname);

	if (hostinfo == (struct hostent *)NULL) {
#ifdef RKOERROR
		rkocpyerror("tcp_connect : failed connection : ");
		rkocaterror(hstrerror(h_errno));
#endif
		return -1;
	}

	if (hostinfo->h_addrtype != AF_INET) {
#ifdef RKOERROR
		rkocpyerror("tcp_connect : not TCP/IP : ");
		rkocaterror(hstrerror(h_errno));
#endif
		return -1;
	}

	memset(&sock_addr, 0, sizeof(struct sockaddr_in));
	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons((unsigned short) port_num);
	memcpy(&(sock_addr.sin_addr), hostinfo->h_addr, hostinfo->h_length);

	sd = socket(AF_INET, SOCK_STREAM, PF_UNSPEC);

	if (sd < 0) {
#ifdef RKOERROR
		rkocpyerror("tcp_connect: failed socket opening");
#endif
		return -1;
	}

	status = connect(sd, (struct sockaddr *) &sock_addr,
		sizeof(struct sockaddr_in));

	if (status < 0) {
#ifdef RKOERROR
		rkocpyerror("tcp_connect: failed port connection");
#endif
		close(sd);
		return -1;
	}
	return sd;
}

int tcp_close(int sd) {
	if (shutdown(sd,2) < 0) {
#ifdef RKOERROR
		rkocpyerror("tcp_close: shutdown error");
#endif
		return -1;
	}
	if (close(sd) < 0) {
#ifdef RKOERROR
		rkocpyerror("tcp_close: close error");
#endif
		return -1;
	}
		return 0;
}

#ifdef TEST

/* :*: cc tcp_connect.c -L. -lrko
 * :*: && a.out
 * :*:END:
 */

#  include <stdio.h>
#  include <string.h>
#  include <sys/types.h>
#  include <unistd.h>
#  define ECHO_PORT 7
#  define BUFLEN 1024

int main(int argc, char *argv[]) {
	int efd, n;
	char buffer[BUFLEN];

	if ((efd = tcp_connect("localhost", ECHO_PORT)) < 0) {
#  ifdef RKOERROR
		rkoperror(*argv);
#  endif
		return -1;
	}
/* send message to echo port */
	sprintf(buffer, "Hello Echo Port\n");
	n = strlen(buffer) + 1;
	if (write(efd, buffer, n) != n) {
		perror(*argv);
	} else {
		(void) memset(buffer, '\0', BUFLEN);
	}
/* receive message it again */
	if ((n = read(efd, buffer, BUFLEN)) < 0) {
		perror(*argv);
		printf("no reply from echo port!\n");
	} else {
		buffer[n] = '\0';
		printf("reply from echo port: %s", buffer);
	}
/* close out connection */
	if (tcp_close(efd) < 0) {
#  ifdef RKOERROR
		rkoperror(*argv);
#  endif
		return -1;
	}
	
	return 0;
}
#endif /* TEST */
