static const char RCSID[]="@(#)$Id: rkoerror.c,v 1.6 2002/09/13 01:50:44 rk Exp $";
static const char AUTHOR[]="@(#)rkoerror 1.0 03/26/96 R.K.Owen,Ph.D.";
/* rkoerror - contains the librko perror type stuff
 */
/** ** Copyright *********************************************************** **
 ** 									     **
 ** Copyright 1997 by R.K.Owen,Ph.D.		                      	     **
 ** last known email: librko@kooz.sj.ca.us				     **
 **                   rk@owen.sj.ca.us					     **
 ** see LICENSE.LGPL, which must be provided, for details		     **
 ** 									     **
 ** ************************************************************************ **/

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
	if (!s) return (char *) NULL;
	rkoerrormsg[MAXRKOMSG] = '\0';
	return strncpy(rkoerrormsg, s, MAXRKOMSG);
	return (char *) NULL;
}

char *rkocaterror(const char *s) {
	if (!s) return (char *) NULL;
	rkoerrormsg[MAXRKOMSG] = '\0';
	return strncat(rkoerrormsg, s, MAXRKOMSG - strlen(rkoerrormsg));
}

char *rkopsterror(const char *s) {
	int lens, lene;

	if (!s) return (char *) NULL;
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
