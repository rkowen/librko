/* rudimentary test code */

#include <stdio.h>
#include <string.h>
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#ifdef RKOERROR
#  include "rkoerror.h"
#endif
#include "uvec.h"
#include "avec.h"
#include "strfns.h"

char testbuf[512];

int printout(avec *av, char const *head, char const *ans) {
	char const * const * vec;
	char **vptr;
	char *sptr;
	uvec *uk;
	sprintf(testbuf,"c:%d n:%d",
		avec_capacity(av), avec_number(av));
	if (avec_exists(av)) {
		strcat(testbuf," k:");
		vec = avec_keys(av);
		uk = uvec_alloc();
		uvec_copy_vec(uk, vec, 0);
		uvec_sort(uk, UVEC_ASCEND);
		strcat(testbuf, sptr = uvec2str(uk, "|"));
		strfree(&sptr);
		free((void *)vec);

		vptr = uvec_vector(uk);
		strcat(testbuf," v:");
		while (*vptr) {
			strcat(testbuf,avec_lookup(av,*vptr));
			vptr++;
			if (*vptr) strcat(testbuf,"|");
		}

		uvec_dtor(&uk);
	}
	if (strcmp(testbuf, ans)) {
		printf("FAIL:%-20s=\n    <\t%s\n    >\t%s\n",head,testbuf,ans);
		return 1;
	} else {
		printf("OK  :%-20s=\n\t%s\n",head,testbuf);
		return 0;
	}
# ifdef RKOERROR
	if (rkoerrno != RKO_OK) {
		printf("%s\n", rkostrerror());
	}
# endif
}

#define _CHECK(c, v, a) \
	count++; c; \
	results += printout(&(v), #c, a); \
	avec_dtor(&x);

int main () {
	avec *x;
	int count=0, results=0;

	_CHECK(x=key_value("first=>=1=||second=>=2=||third=>=3=",NULL,NULL,0),
		*x, "c:11 n:3 k:first|second|third v:=1=|=2=|=3=");
	_CHECK(x=key_value("first>>=1=^second>>=2=^third>>=3=","^",">>",0),
		*x, "c:11 n:3 k:first|second|third v:=1=|=2=|=3=");
	_CHECK(x=key_value("first===1=%second===2=%third===3=","%","==",0),
		*x, "c:11 n:3 k:first|second|third v:=1=|=2=|=3=");
	_CHECK(x=key_value(
		"  first  =>  =1=  ||	second	=> 	=2= 	||third=>=3=",
		NULL,NULL,1),
		*x, "c:11 n:3 k:first|second|third v:=1=|=2=|=3=");
	_CHECK(x=key_value("first=>=1=",NULL,NULL,0),
		*x, "c:3 n:1 k:first v:=1=");
	_CHECK(x=key_value("first>>=1=",NULL,">>",0),
		*x, "c:3 n:1 k:first v:=1=");
	_CHECK(x=key_value("first===1=",NULL,"==",0),
		*x, "c:3 n:1 k:first v:=1=");
	_CHECK(x=key_value("  first  >>	=1=	",NULL,">>",0),
		*x, "c:3 n:1 k:  first   v:	=1=	");
	_CHECK(x=key_value("  first  >>	=1=	","%",">>",1),
		*x, "c:3 n:1 k:first v:=1=");
	_CHECK(x=key_value("first=>=1=||second=>=2=||third=>=3=","%",NULL,0),
		*x, "c:-1 n:-1");
	_CHECK(x=key_value("first=>=1=||second=>=2=||third=>=3=",NULL,">>",0),
		*x, "c:-1 n:-1");
	_CHECK(x=key_value("first>>=1=||second=>=2=||third=>=3=",NULL,NULL,0),
		*x, "c:-1 n:-1");
	_CHECK(x=key_value("first=>=1=||second=>=2=%third=>=3=",NULL,NULL,0),
		*x, "c:-1 n:-1");

	printf("========================\n");

	if (results) {
		printf("There were %d failures in %d tests\n", results, count);
	} else {
		printf("There were no failures in %d tests\n", count);
	}
	return results;
}
