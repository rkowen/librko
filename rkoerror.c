static const char USMID[]="%W%";
static const char RCSID[]="@(#)$Id: rkoerror.c,v 1.2 1998/10/14 15:16:10 rk Exp $";
static const char AUTHOR[]="@(#)rkoerror 1.0 03/26/96 R.K.Owen,Ph.D.";
/* rkoerror - contains the librko perror type stuff
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
 * last known email: rkowen@ckns.net
 *                   rk@owen.sj.ca.us
 *
 *********************************************************************
 */

#define MAXRKOMSG 1023

char rkoerrormsg[MAXRKOMSG + 1];

int rkoerrno = 0;

#include <stdio.h>
#include <string.h>

/* must set rkoerrno to non-zero for any output to occur */
void rkoperror(const char *s) {
	if (rkoerrno != 0) {
		rkoerrormsg[MAXRKOMSG] = '\0';
		fprintf(stderr,"%s : %s\n", s, rkoerrormsg);
	}
}

char *rkostrerror(void) {
	if (rkoerrno != 0) {
		rkoerrormsg[MAXRKOMSG] = '\0';
		return rkoerrormsg;
	} else {
		return "";
	}
}

char *rkocpyerror(const char *s) {
	rkoerrormsg[MAXRKOMSG] = '\0';
	return strncpy(rkoerrormsg, s, MAXRKOMSG);
}

char *rkocaterror(const char *s) {
	rkoerrormsg[MAXRKOMSG] = '\0';
	return strncat(rkoerrormsg, s, MAXRKOMSG - strlen(rkoerrormsg));
}

char *rkopsterror(const char *s) {
	int lens, lene;

	rkoerrormsg[MAXRKOMSG] = '\0';
	lens = strlen(s);
	if (lens >= MAXRKOMSG) return (char *) NULL;
	lene = strlen(rkoerrormsg);
	(void) memmove(rkoerrormsg + lens, rkoerrormsg,
		(lene < MAXRKOMSG - lens ? lene : MAXRKOMSG - lens));
	rkoerrormsg[MAXRKOMSG] = '\0';
	(void) memcpy(rkoerrormsg, s, lens);

	return rkoerrormsg;
}

#ifdef TEST
int main() {

	(void) fprintf(stderr, "%s\n", rkocpyerror("aaaa : "));
	(void) fprintf(stderr, "%s\n", rkocaterror("bbbb : "));
	(void) fprintf(stderr, "%s\n", rkocaterror("cccc : "));
	(void) fprintf(stderr, "%s\n", rkopsterror("1111 : "));
	(void) fprintf(stderr, "%s\n", rkopsterror("0000 : "));
	(void) fprintf(stderr, "%s\n", rkostrerror());
	rkoperror("main");
}
#endif /* TEST */
