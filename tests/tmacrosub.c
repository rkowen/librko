#include <stdio.h>
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#include "librko.h"

#define MAXTEST 15
/* the following is for brain-dead K&R C compilers found on Sun's */
#define HACK(a,b,c) str[3*i] = a; str[3*i+1] = b; str[3*i+2] = c;  i++;

char *FileN = "File";

int main(int argc, char *argv[]) {
	int i=0, result, ret, len, totresult = 0;
	char buffer[80];
	char *str[3*MAXTEST];
	char *ptr;
	/*	macro str,	result,				success */
	HACK(	"xxx$$xxx",	"xxx$xxx",				"1")
	HACK(	"$a=abcdef",	"",					"1")
	HACK(	"$b=yy$$$a$$yy",	"",				"1")
	HACK(	"uuu$buuu",	"uuuyy$abcdef$yyuuu",			"1")
	HACK(	"set $c=$bzz",	"set ",					"1")
	HACK(	"$d=aa$c",	"",					"1")
	HACK(	":$d:$d:",	":aayy$abcdef$yyzz:aayy$abcdef$yyzz:",	"1")
	HACK(	"$a=mnopqr",	"",					"1")
	HACK(	":$d:$d:",	":aayy$mnopqr$yyzz:aayy$mnopqr$yyzz:",	"1")
	HACK(	"$b=$$$@$$",	"",					"1")
	HACK(	":$d:$d:",	":aa$File$zz:aa$File$zz:",		"1")
	HACK(	"xx$.$$z",	"xx",					"0")
	HACK(	":$d:$d:",	":aa$File$zz:aa$F",			"1")
	HACK(	"$b=-$d-",	"",					"1")
	HACK(	":$d:$d:",	":aa$File$zz:aa$",			"0")

	MacroStr[MacroTrf['@']] = FileN;

	for (i = 0; i < MAXTEST; ++i) {
		*buffer = '\0';
		if (i != 12) {
			len = 80;
			ptr = macstrncpy(buffer,str[3*i],len);
		} else {
			len = 16;
			ptr = macstrncpy(buffer,str[3*i],len);
		}
		if (ptr == (char *) NULL) {
			rkoperror(*argv);
			ret = 0;
		} else {
			ret = 1;
		}
		result = strncmp(buffer, str[3*i+1],len);
		if ((*str[3*i+2] == '1' && result != 0)
		 || (*str[3*i+2] == '0' && ret != 0)) {
			totresult++;
			printf("FAIL :");
		} else {
			printf("OK   :");
		}
		printf(" %15.15s %35.35s : %d\n", str[3*i], buffer, ret);
	}

	fprintf(stderr,"\nThere were ");
	if (totresult == 0) fprintf(stderr,"NO");
	else fprintf(stderr,"%d", totresult);
	fprintf(stderr," macrosub test failures.\n\n");
	return totresult;
}
