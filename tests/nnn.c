static const char USMID[]="%W%";
static const char RCSID[]="@(#)$Id: nnn.c,v 1.1 1998/10/07 16:19:56 rk Exp $";
static const char AUTHOR[]="@(#)nameread 1.0 01/01/94 R.K.Owen,Ph.D.";

/* nameread.c - reads a line of text and finds the integer variable name 
 *	and value to set it to.
 *	This is somewhat simular to the FORTRAN NAMELIST functionality
 *	(but not really).
 *	It ignores any line that begins with '#' as the first non-white
 *	space character.
 *	nameread returns 0 if it doesn't match the variable name,
 *	returns 1 if it successfully matches and sets the value,
 *	and returns < 0 if an error occurs.
 *
 *	Example Input:
		var	=	-1
		# comment
		index		3
		step=2
 *
 * author	R.K.Owen,Ph.D.	01/01/94
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

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#define NAMETOL(a,b)	nametol(a,#b,&b)
#define NAMETOD(a,b)	nametod(a,#b,&b)

static const char delimit[] = " \t\v\a\b";

int nametol(const char *line, const char *name, long *value) {
	int retval = 0;
	char *ptr = (char *) line;
	char *endptr, *tmp;
	int len = (int) strlen(name);
	long tmpval;

	while (isspace((int) *ptr)) ptr++;		/* skip white space */
	if (*ptr == '\0' || *ptr == '#')		/* at end or comment */
		return retval;
/* check for name match */
	if ((tmp = strtok(ptr, delimit)) == (char *) NULL) {
		return retval;				/* no  match */
	}
	if ((ptr = strstr(tmp, name)) == (char *) NULL) {
		return retval;				/* no  match */
	} else {					/* got match */
		retval = -1;				/* set for error */
		ptr += len;				/* look after name */
		while (isspace((int) *ptr)) ptr++;	/* skip white space */
		if ( *ptr == '\0' || *ptr == '#' )	/* at end or comment */
			return retval;
		if ( *ptr == '=' ) ++ptr;		/* at optional '=' */
		tmpval = strtol(ptr, &endptr, 0);	/* read number */
		if (ptr == endptr) return retval;	/* not valid number */
/* success */
		retval = 1;
		*value = tmpval;
		return retval;
	}
}

int nametod(const char *line, const char *name, double *value) {
	int retval = 0;
	char *ptr = (char *) line;
	char *endptr, *tmp;
	int len = (int) strlen(name);
	double tmpval;

	while (isspace((int) *ptr)) ptr++;		/* skip white space */
	if (*ptr == '\0' || *ptr == '#')		/* at end or comment */
		return retval;
/* check for name match */
	if ((tmp = strtok(ptr, delimit)) == (char *) NULL) {
		return retval;				/* no  match */
	}
	if ((ptr = strstr(tmp, name)) == (char *) NULL) {
		return retval;				/* no  match */
	} else {					/* got match */
		retval = -1;				/* set for error */
		ptr += len;				/* look after name */
		while (isspace((int) *ptr)) ptr++;	/* skip white space */
		if ( *ptr == '\0' || *ptr == '#' )	/* at end or comment */
			return retval;
		if ( *ptr == '=' ) ++ptr;		/* at optional '=' */
		tmpval = strtod(ptr, &endptr);		/* read number */
		if (ptr == endptr) return retval;	/* not valid number */
/* success */
		retval = 1;
		*value = tmpval;
		return retval;
	}
}

#ifdef TEST

#  include <stdio.h>

#  define ITEST(a,b)	if ( a == b ) printf("OK  ");\
			else {printf("FAIL");totresult++;}\
			printf(" %s = %d : %d\n", #a, a, b);
#  define RTEST(a,b)	if ( a == b ) printf("OK  ");\
			else {printf("FAIL");totresult++;}\
			printf(" %s = %g : %g\n", #a, a, b);
int main() {
long v=1, index=1, step=1, set=1;
long v1=1, v2=1, v3=1, v4=1;
double r1=1, r2=1, r3=1, r4=1;
char *text[] = {
		"v	=	-1",
		"# comment",
		"index		3",
		"r3=-3.05",
		"step=0x02",
		"	v1 017",
		"v2 0xFF",
		"r1 2.0 r2	=	  -0.5",
		" v3	=-0xFF,	v4=-027",
		};
int i, totresult = 0, num=sizeof(text)/sizeof(text[0]);

	printf("test input: %d lines\n",num);
	for (i=0; i < num; ++i) {
		printf("%s\n", text[i]);
	}
	printf("\n");

	for (i=0; i < num; ++i) {
		(void) NAMETOL(text[i], step);
		(void) NAMETOL(text[i], v);
		(void) NAMETOL(text[i], index);
		(void) NAMETOL(text[i], set);
		(void) NAMETOL(text[i], v1);
		(void) NAMETOL(text[i], v3);
		(void) NAMETOL(text[i], v2);
		(void) NAMETOL(text[i], v4);
		(void) NAMETOD(text[i], r2);
		(void) NAMETOD(text[i], r3);
		(void) NAMETOD(text[i], r1);
	}
/* compare to expected result */
	ITEST(v,-1);
	ITEST(index,3);
	ITEST(step,2);
	ITEST(set,1);
	ITEST(v1,15);
	ITEST(v2,255);
	ITEST(v3,-255);
	ITEST(v4,-23);
	RTEST(r1,2.0);
	RTEST(r2,-0.5);
	RTEST(r3,-3.05);
	RTEST(r4,1.0);
	fprintf(stderr,"\nThere were ");
	if (totresult == 0) fprintf(stderr,"NO");
	else fprintf(stderr,"%d",totresult);
	fprintf(stderr," nameread test failures\n");
	return totresult;
}
#endif /* TEST */
