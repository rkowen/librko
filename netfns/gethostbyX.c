
static const char USMID[]="%W%";
static const char RCSID[]="@(#)$Id: gethostbyX.c,v 1.1 1998/10/07 16:19:56 rk Exp $";
static const char AUTHOR[]="@(#)gethostbyX 1.0 03/26/96 R.K.Owen,Ph.D.";

/* gethostbyX.c - accepts either input sutiable for gethostbyname
 *	(host.domain) or gethostbyaddr (4 octet IP address), and
 *	returns a hostent struct pointer.
 *
 * author	R.K.Owen,Ph.D.	03/36/94
 */
/*
 *********************************************************************
 *
 *     This software is copyrighted by R.K.Owen,Ph.D. 1997
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
 *
 *********************************************************************
 */

/* define TEST to create a stand-alone program to test the routine */
#if 0
#  define TEST
#endif

#include <netdb.h>		/* gethostbyname, gethostbyaddr */
#include <sys/socket.h>		/* inet_addr */
#include <netinet/in.h>		/* inet_addr */
#include <arpa/inet.h>		/* inet_addr */
#include <string.h>
#include <ctype.h>

struct hostent *gethostbyX(char *hostX) {
	struct hostent *hostinfo;

/* if 1st char is number & there is a `.' then assume to be an IP addr */
	if (isdigit(*hostX) && strchr(hostX, '.')) {
		unsigned long addr = inet_addr(hostX);
/* invalid addr */
		if (addr < 0l) return (struct hostent *) NULL;

		hostinfo = gethostbyaddr((void *)(&addr), sizeof(addr),
			AF_INET);
	} else {
		hostinfo = gethostbyname(hostX);
	}

/* check if valid hostinfo */
	if (hostinfo == (struct hostent *) NULL)
		return (struct hostent *) NULL;		/* nope */

/* only want IP address mapping */
	if (hostinfo->h_addrtype != AF_INET)
		return (struct hostent *) NULL;		/* not IP addr */

/* success */
	return hostinfo;
}

#ifdef TEST

#  include <stdio.h>

/* this may need to be tailored to your own host set-up */
void outent(char *host, struct hostent *hostinfo) {
	char **ptr;
	int i;

	printf("================================= %s\n", host);
	printf("official name   = %s\n", hostinfo->h_name);
	for (ptr = hostinfo->h_aliases; *ptr != (char *) NULL; ++ptr) {
		printf("alias    name   = %s\n", *ptr);
	}
	printf("address type    = %d\n", hostinfo->h_addrtype);
	printf("address length  = %d\n", hostinfo->h_length);
	for (ptr = hostinfo->h_addr_list; *ptr != (char *) NULL; ++ptr) {
		printf("address list    = %s\n",
			inet_ntoa(*(struct in_addr *)*ptr));
	}
}

int main() {
	char *list[] = {
		"localhost",
		"127.0.0.1",
		"owen.sj.ca.us",
		"owen",
		"kooz",
		"205.179.57.217",
		(char *) NULL
	};
	char **ptr;
	struct hostent *hostinfo;

	for (ptr = list; *ptr != (char *) NULL; ++ptr) {
		hostinfo = gethostbyX(*ptr);
		outent(*ptr, hostinfo);
	}
	return 0;
}
#endif /* TEST */
