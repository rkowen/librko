/*
 * :*: cc -o mexpn mexpn.c -I${HOME}/include -L${HOME}/lib -lrko
 * :*: && a.out rk xxx marsha
 * :*:END:
 */
/* mexpn - a little program that will open a connection to a mail
 *	server and query for a given user name
 *
 * by R.K.Owen,Ph.D. (rk@owen.sj.ca.us)
 *
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "librko.h"

#define MAIL_PORT	25
#define MAIL_SERV	"localhost"
#define TIME_OUT	2

int ReceiveString(int fd, char *buffer, int buf_len, int *len) {
	int i = 0, retval = 0, n;
	*len = 0;

	while(1) {
		if (i >= buf_len - 1) {
			retval = 1;	/* buffer overflow */
			break;
		}
		n = read(fd, buffer + i, 1);
		if (n < 0) {
			retval = 2;	/* read error */
			break;
		} else if (n == 0) {
			buffer[i] = '\0';
			*len = i;
			retval = -1;	/* EOF */
			break;
		} else if (buffer[i] == '\r')	continue;
		else if (buffer[i] == '\n')	break;
		i++;
	}
	buffer[i] = '\0';
	*len = i;
	return retval;
}

int read_mail_srv(FILE *rm, char *buffer, int buf_len, int seconds,
	char **response) {

	char *ptr = buffer;
	int c, status = 0, num = 0, response_num = 0, n = 0;
	int len = 0;

	(void) memset(buffer, '\0', buf_len);

#if 0
	status = timedfgets(buffer,buf_len,rm,seconds);
#else
	while (!(status = ReceiveString(fileno(rm), buffer, buf_len, &len)));
#endif

	if (status < 0) {
		return -1;
	} else {
		if (sscanf(buffer, "%d %n", &response_num, &n) <= 0) {
			return 0;
		}
		*response = buffer + n;
		return response_num;
	}
	do {
		c = fgetc(rm);
		num++;
		*ptr++ = (char) c;
	} while (c != '\0' && c != '\n' && num < buf_len);
	if (sscanf(buffer, "%d %n", &response_num, &n) <= 0) {
		return 0;
	}
	*response = buffer + n;
	return response_num;
}

void usage(char *Prg, char *DefHost) {
	(void) fprintf(stderr,
"\n%s - prints out the mail alias expansion from a given mail server\n"
"\n"
"usage: %s [-h hostname] username ...\n"
"	-h hostname	which host to query (default: %s)\n"
"	username	list of user names to expand mail aliases for\n\n",
		Prg, Prg, DefHost);
}

#define BUF_LEN	1024

int main (int argc, char **argv) {
	char *ProgramName;
	int md;
	FILE *MAILRCV, *MAILSND;
	char buffer[BUF_LEN], *ptr;
	char *mail_serv = MAIL_SERV;
	int rn;

	ProgramName = *argv++;

	if (*argv == (char *) NULL) {
		usage(ProgramName, (char *) MAIL_SERV);
		return 1;
	}

	if (!strcmp(*argv,"-h")) {
		argv++;
		mail_serv = *argv++;
	}

	if (*argv == (char *) NULL) {
		usage(ProgramName, (char *) MAIL_SERV);
		return 2;
	}

	if ((md = tcp_connect(mail_serv, MAIL_PORT)) < 0) {
		fprintf(stderr,"%s : failed to connect to mail server '%s'\n",
			ProgramName, mail_serv);
		return 3;
	}

	if ((MAILRCV = fdopen(md, "r")) == (FILE *) NULL) {
		fprintf(stderr,"%s : failed to open mail server read '%s'\n",
			ProgramName, mail_serv);
		return 4;
	}

	if ((MAILSND = fdopen(md, "w")) == (FILE *) NULL) {
		fprintf(stderr,"%s : failed to open mail server write '%s'\n",
			ProgramName, mail_serv);
		return 5;
	}

	/* get through the introduction */

	while ((rn = read_mail_srv(MAILRCV, buffer, BUF_LEN, TIME_OUT, &ptr))
		== 220) {

		if (buffer[3] == '-') continue;	/* continue to the next ``line'' */
		if (buffer[3] == ' ') break;	/* end of intro */

		if (rn < 0) {
			if (rkoerrno == RKO_OK) {
				if (errno == EINTR)
					continue;	/* timed out */
				if (errno == EIO)
					break;		/* no data read EOF */
			} else {
				rkoperror(ProgramName);
			}
		}
	}

	/* send expn requests */
	while (*argv != (char *) NULL) {
		fprintf(MAILSND, "expn %s\n", *argv);
		fflush(MAILSND);
		rn = read_mail_srv(MAILRCV, buffer, BUF_LEN, TIME_OUT, &ptr);
		if (rn != 250 && rn != 550) {
			if (rn < 0) {
				if (rkoerrno == RKO_OK) {
					if (errno == EINTR)
						continue;	/* timed out */
					if (errno == EIO)
						break;		/* no data read EOF */
				} else {
					rkoperror(ProgramName);
				}
			}
			fprintf(stderr,
				"%s : improper response number for '%s'\n%s",
				ProgramName, *argv, buffer);
		}
		printf("%-20.20s => %s", *argv++, ptr);
	}

	fprintf(MAILSND, "quit\n");
	fflush(MAILSND);

	tcp_close(md);

	return 0;
}
