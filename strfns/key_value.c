static const char RCSID[]="@(#)$Id: key_value.c,v 1.1 2003/09/05 05:02:00 rk Exp $";
static const char AUTHOR[]="@(#)keyvalue 1.0 1995/10/05 R.K.Owen,Ph.D.";

/* key_value - chops a string into key/value pairs
 *	and returns an avec
 *
 * prototype:
avec *key_value(const char *string,
	const char *pair_delim, const char *ref_delim, int nowhitepace);
 *
 *	string		= string to chop up into key/value pairs
 *	pair_delim	= token between key/value pairs
 *			  if NULL then use "||" by default
 *	ref_delim	= token between the key and value ... to refer to
 *			  the value from the key
 *			  if NULL then use "=>" by default
 *	nowhitespace	= set this non-zero if you want white space trimmed
 *			  around the keys and values
 *
 * author	R.K.Owen,Ph.D.	2003/09/04
 *
 */
/** ** Copyright *********************************************************** **
 ** 									     **
 ** Copyright 2003 by R.K.Owen,Ph.D.		                      	     **
 ** last known email: librko@kooz.sj.ca.us				     **
 **                   rk@owen.sj.ca.us					     **
 ** see LICENSE.LGPL, which must be provided, for details		     **
 ** 									     **
 ** ************************************************************************ **/
/*
 * Define:	RKOERROR and load with librko.a for error handling
 *		TEST to run a stand-alone test program
 */

#if 0
#  define TEST
#endif

#include <stdlib.h>
#include <string.h>
#ifdef MEMDEBUG
#  include "memdebug.h"
#endif
#ifdef RKOERROR
#  include "rkoerror.h"
#endif
#include "avec.h"

/* skip whitespace until hit boundary
 * direction	= 0 	=> don't bother
 * 		> 0	=> in forward direction
 * 		< 0	=> in reverse direction
 */
static char *skip_ws(char const *string, char const *boundary, int direction) {
	char const *ptr = string;
	if (!direction) return (char *) string;

	direction = (direction > 0 ? 1 : -1);
	
	while (isspace((int) *ptr)) {
		ptr += (size_t) direction;
		if (ptr == boundary) break;
	}
	return (char *) ptr;
}

/* take a key/value string and break it apart
 * the default is something like "key1=>val1||key2=>val2||...||keyN=>valN"
 */
avec *key_value(const char *string,
	const char *pair_delim, const char *ref_delim, int nowhitespace) {

	char const	*t_pair_delim,			/* usually || */
			*t_ref_delim,			/* usually => */
			*ptr = string,
			*pptr;
	char	*buffer,			/* temp str storage */
		*rptr, *pwsptr, *rwsptr, *v1ptr, *v2ptr;

	size_t	 l_pair_delim,			/* string length */
		 l_ref_delim,			/* string length */
		 blen;
	int	 c_pair_delim = 0,		/* token count */
		 c_ref_delim = 0;		/* token count */

	avec	*kv;				/* key/value pairs */
	int	 forward = 1,
		 reverse = -1;

	if (pair_delim && *pair_delim)
		t_pair_delim = pair_delim;
	else
		t_pair_delim = "||";
	l_pair_delim = strlen(t_pair_delim);

	if (ref_delim && *ref_delim)
		t_ref_delim = ref_delim;
	else
		t_ref_delim = "=>";
	l_ref_delim = strlen(t_ref_delim);

	if (!nowhitespace) {
		forward = 0;
		reverse = 0;
	}

	/* count tokens in string */
	while ((ptr = strstr(ptr, t_pair_delim))) {
		ptr += l_pair_delim;
		c_pair_delim++;
	}
	ptr = string;
	while ((ptr = strstr(ptr, t_ref_delim))) {
		ptr += l_ref_delim;
		c_ref_delim++;
	}

	/* always one more ref_delim than pair_delim */
	if (c_ref_delim != c_pair_delim + 1 || c_ref_delim == 0) {
#ifdef RKOERROR
		(void) rkocpyerror(
		"key_value : incorrect key/value delimiting with '");
		(void) rkocaterror(t_pair_delim);
		(void) rkocaterror("' and '");
		(void) rkocaterror(t_ref_delim);
		(void) rkocaterror("'");
		rkoerrno = RKOUSEERR;
#endif
		goto fail0;
	}

	/* construct avec - use STDC fns */
	if (!(kv = avec_ctor_(3*c_pair_delim,avec_get_std_fns(AVEC_STDC)))) {
#ifdef RKOERROR
		(void) rkopsterror( "key_value : ");
#endif
		goto fail0;
	}

	/* alloc temp space (equal to string) */
	if (!(buffer = (char *) malloc(strlen(string)+1))) {
#ifdef RKOERROR
		(void) rkocpyerror("key_value : malloc error");
		rkoerrno = RKOMEMERR;
#endif
		goto fail1;
	}

	/* cycle through string */
	ptr = skip_ws(string,string + strlen(string),forward);

	/* get pair delim else end of string */
	pptr = strstr(ptr,t_pair_delim);
	if (!pptr) pptr = string + strlen(string);
	pptr--;
	pwsptr = skip_ws(pptr, ptr, reverse);

	do {
		/* copy string to buffer */
		strncpy(buffer, ptr, pwsptr - ptr + 1);
		buffer[pwsptr - ptr + 1] = '\0';
		blen = strlen(buffer);

		/* find ref token in this string and zero */
		rptr = strstr(buffer,t_ref_delim);
		if (!rptr) {
#ifdef RKOERROR
			(void) rkocpyerror(
			"key_value : no reference delimiter of '");
			(void) rkocaterror(t_ref_delim);
			(void) rkocaterror("' in '");
			(void) rkocaterror(buffer);
			(void) rkocaterror("'");
			rkoerrno = RKOUSEERR;
#endif
			goto fail2;
		}
		rwsptr = skip_ws(rptr-1, buffer, reverse);
		*++rwsptr = '\0';
		v1ptr = skip_ws(rptr + l_ref_delim, buffer + blen, forward);
		v2ptr = skip_ws(buffer + blen - 1, rptr + l_ref_delim, reverse);
		v2ptr[1] = '\0';
		/* add to avec */
		if (avec_insert(kv, buffer,  v1ptr)) {
#ifdef RKOERROR
			(void) rkocpyerror(
			"key_value : avec insert error on '");
			(void) rkocaterror(buffer);
			(void) rkocaterror("' ");
			(void) rkocaterror(t_ref_delim);
			(void) rkocaterror(" '");
			(void) rkocaterror(rrptr + l_ref_delim);
			(void) rkocaterror("'");
			rkoerrno = RKOUSEERR;
#endif
			goto fail2;
		}

		ptr = pptr + l_pair_delim + 1;
		ptr = skip_ws(ptr, string + strlen(string), forward);
		/* end when past end of string */
		if (ptr >= string + strlen(string)) break;

		pptr = strstr(ptr,t_pair_delim);
		if (!pptr) pptr = string + strlen(string);
		pptr--;
		pwsptr = skip_ws(pptr, ptr, reverse);

	} while (1);
	
	free(buffer);
	return kv;

fail2:
	free(buffer);
fail1:
	(void) avec_dtor(&kv);
fail0:
	return (avec *) NULL;
}

#if defined(TEST)

#include <stdio.h>

int main() {

	avec *kv1, *kv2, *kv3;
	avec_element **elem = 0;

	kv1 = key_value("aaa=>one||bbb=>two||ccc=>three", NULL, NULL,0);
	kv2 = key_value("  aaa  =>  one  ||	bbb	=>	two	||ccc=>three", NULL, NULL,1);
	kv3 = key_value("  aaa  =>  one  ||	bbb	=>	two	||ccc=>three", NULL, NULL,0);

	while ((elem = avec_walk_r(kv1, elem))) {
	printf("'%s' => '%s'\n", AVEC_KEY(*elem), (char *) AVEC_DATA(*elem));
	}
	while ((elem = avec_walk_r(kv2, elem))) {
	printf("'%s' => '%s'\n", AVEC_KEY(*elem), (char *) AVEC_DATA(*elem));
	}
	while ((elem = avec_walk_r(kv3, elem))) {
	printf("'%s' => '%s'\n", AVEC_KEY(*elem), (char *) AVEC_DATA(*elem));
	}

	if (avec_dtor(&kv3)) return -7;
	if (avec_dtor(&kv2)) return -8;
	if (avec_dtor(&kv1)) return -9;
	return EXIT_SUCCESS;
}
#endif /* TEST */
