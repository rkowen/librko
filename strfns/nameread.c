static const char RCSID[]="@(#)$Id: nameread.c,v 1.7 2007/07/18 21:49:22 rk Exp $";
static const char AUTHOR[]="@(#)nameread 1.0 01/01/94 R.K.Owen,Ph.D.";

/* nameread.c - reads a line of text and finds the variable name 
 *	and value to set it to.
 *	This is somewhat simular to the FORTRAN NAMELIST functionality
 *	(but not really).
 *	It ignores any line that begins with '#' as the first non-white
 *	space character.
 *	nameread returns 0 if it doesn't match the variable name
 *	or value is set,
 *	returns 1 if it successfully matches and sets the value,
 *
 *	Example Input:
		var	=	-1
		# comment
		index	=	3
		step=2
		 text	=	any amount of text on the line

 *
 * See the test text below for an example of how to read a data file
 * for a bunch of variables.  Note that it does not overwrite the existing
 * values in the variables if nameread line is not found
 *
 * author	R.K.Owen,Ph.D.	1994/01/01
 * modified	R.K.Owen,Ph.D.	2007/07/07 added nameto_() and nametos()
 */
/** ** Copyright *********************************************************** **
 ** 									     **
 ** Copyright 1997 by R.K.Owen,Ph.D.		                      	     **
 ** last known email: librko@kooz.sj.ca.us				     **
 **                   rk@owen.sj.ca.us					     **
 ** see LICENSE.LGPL, which must be provided, for details		     **
 ** 									     **
 ** ************************************************************************ **/

/* define TEST to create a stand-alone program to test the routine */
#if 1
#  define TEST
#endif

#include <ctype.h>
#include <string.h>
#include <stdlib.h>

typedef enum NameTo {
	nameto_long,
	nameto_double,
	nameto_string
}	nameto;

typedef union {
	long	l;
	double	d;
	char *	s;
} 	nameto_t;

#define NAMETOL(a,b)	nametol(a,#b,&b)
#define NAMETOD(a,b)	nametod(a,#b,&b)
#define NAMETOS(a,b,lb)	nametos(a,#b,b,lb)

int nameto_(nameto type, const char *line, const char *name,
	nameto_t *value, size_t length) {

	int	 retval = 0,
		 len = (int) strlen(name);
	char	*ptr = (char *) line,
		*endptr,
		*tp,
		*eq;
	nameto_t tmpval;

	while (isspace((int) *ptr)) ptr++;		/* skip white space */
	if (*ptr == '\0' || *ptr == '#')		/* at end or comment */
		return retval;
/* check for name match */
	if ((ptr = strstr(ptr, name)) == (char *) NULL)
		return retval;				/* no match */
/* find the '=' */
	if ((eq = strchr(ptr, (int) '=')) == (char *) NULL) {
		return retval;				/* no  match */
	}
	endptr = eq - 1;
	while (isspace((int) *endptr)) endptr--;	/* skip trailing ws */

	if (((int) (endptr - ptr + 1)) != len )
		return retval;				/* no match */

	if (strncmp(ptr, name, len)) {
		return retval;				/* no  match */
	} else {					/* got match */
		/* retval = -1;				** set for error */
		ptr = eq + 1;				/* set after '=' */
		while (isspace((int) *ptr)) ptr++;	/* skip ws */
		if ( *ptr == '\0' || *ptr == '#' )	/* at end or comment */
			return retval;
		switch (type) {
		case nameto_long:
			tmpval.l = strtol(ptr, &endptr, 0);
			break;
		case nameto_double:
			tmpval.d = strtod(ptr, &endptr);
			break;
		case nameto_string:
			strncpy((char *) value,ptr,length);
			/* null out  trailing ws */
			tp = (char *) value + (strlen(ptr) < length
				? strlen(ptr) : length) - 1;
			while (isspace((int) *tp) && tp >= (char *) value) {
				*tp = '\0';
				tp--;
			};
			endptr = tp + 1;
			break;
		}

		if (ptr == endptr) return retval;	/* not valid number */
/* success */
		retval = 1;
		switch (type) {
		case nameto_long:
			value->l = tmpval.l;
			break;
		case nameto_double:
			value->d = tmpval.d;
			break;
		case nameto_string:
			break;
		}
		return retval;
	}
}

int nametol(const char *line, const char *name, long *value) {
	return nameto_(nameto_long, line, name, (nameto_t *) value, 0);
}

int nametod(const char *line, const char *name, double *value) {
	return nameto_(nameto_double, line, name, (nameto_t *) value, 0);
}

int nametos(const char *line, const char *name, char *value, size_t n) {
	return nameto_(nameto_string, line, name, (nameto_t *) value, n);
}

#ifdef TEST

#  include <stdio.h>

#  define ITEST(a,b)	if ( a == b ) printf("OK  ");\
			else {printf("FAIL");totresult++;}\
			printf(" %s = %d : %d\n", #a, a, b);
#  define RTEST(a,b)	if ( a == b ) printf("OK  ");\
			else {printf("FAIL");totresult++;}\
			printf(" %s = %g : %g\n", #a, a, b);
#  define STEST(a,b)	if ( ! strcmp(a,b)) printf("OK  ");\
			else {printf("FAIL");totresult++;}\
			printf(" %s = '%s' : '%s'\n", #a, a, b);

int main() {
	long	v=1, index=1, step=1, set=1;
	long	v1=1, v2=1, v3=1, v4=1, v5=1;
	double	r1=1, r2=1, r3=1, r4=1;
	char	s1[40] = "aaa", s2[40] = "aaa",
		s3[40] = "aaa", s4[10] = "aaa",
		s5[10] = "aaa";
	char	*text[] = {
		"v	=	-1",
		"# comment",
		"index	=	3",
		"r3=-3.05",
		"step=0x02",
		"	v1 = 017",
		"v2 = 0xFF",
		"r1=2.0 r2	=	  -0.5",
		" v3	=-0xFF,	v4=-027",
		"s1=some random string",
		"	s2	=	random = s	",
		"  s3  =  =nothing=  ",
		"  	s4  	=  	1234567890abcdef  ",
		"#EOD"
		};
	int	i, total = 0, totresult = 0, num=sizeof(text)/sizeof(text[0]);

	printf("test input: %d lines\n",num);
	for (i=0; i < num; ++i) {
		printf("%s\n", text[i]);
	}
	printf("\n");

	for (i=0; i < num; ++i) {
		total += NAMETOL(text[i], step);
		total += NAMETOL(text[i], v);
		total += NAMETOL(text[i], index);
		total += NAMETOL(text[i], set);
		total += NAMETOL(text[i], v1);
		total += NAMETOL(text[i], v2);
		total += NAMETOL(text[i], v3);
		total += NAMETOL(text[i], v4);
		total += NAMETOL(text[i], v5);
		total += NAMETOD(text[i], r1);
		total += NAMETOD(text[i], r2);
		total += NAMETOD(text[i], r3);
		total += NAMETOD(text[i], r4);
		total += NAMETOS(text[i], s1, 40);
		total += NAMETOS(text[i], s2, 40);
		total += NAMETOS(text[i], s3, 40);
		total += NAMETOS(text[i], s4, 10);
		total += NAMETOS(text[i], s5, 10);
	}
/* compare to expected result */
	ITEST(total,num);
	ITEST(v,-1);
	ITEST(index,3);
	ITEST(step,2);
	ITEST(set,1);
	ITEST(v1,15);
	ITEST(v2,255);
	ITEST(v3,-255);
	ITEST(v4,-23);
	ITEST(v5,1);
	RTEST(r1,2.0);
	RTEST(r2,-0.5);
	RTEST(r3,-3.05);
	RTEST(r4,1.0);
	STEST(s1,"some random string");
	STEST(s2,"random = s");
	STEST(s3,"=nothing=");
	STEST(s4,"1234567890");
	STEST(s5,"aaa");
	fprintf(stderr,"\nThere were ");
	if (totresult == 0) fprintf(stderr,"NO");
	else fprintf(stderr,"%d",totresult);
	fprintf(stderr," nameread test failures\n");
	return totresult;
}
#endif /* TEST */
