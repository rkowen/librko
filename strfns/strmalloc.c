static const char RCSID[]="@(#)$Id: strmalloc.c,v 1.4 2002/02/08 16:26:01 rk Exp $";
static const char AUTHOR[]="@(#)strmalloc 1.0 09/01/1999 R.K.Owen,Ph.D.";
/* strmalloc - adds a strdup-like routine for copying strings
 * strfree - frees the string memory storage.
 *
 * by R.K. Owen,Ph.D.   09/01/1999
 */
/*
 *********************************************************************
 *
 *     This software is copyrighted by R.K.Owen,Ph.D. 1999
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
 *                   librko@kooz.sj.ca.us
 *
 *********************************************************************
 */

#include <strings.h>
#include <stdlib.h>
#include "strmalloc.h"
#ifdef RKOERROR
#  include "rkoerror.h"
extern int rkoerrno;
#endif

static char id[3] = "STR";

char *strnmalloc(char const *in, size_t n) {
	char *out = (char *) NULL;
	size_t sizeit;
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	sizeit = n + 1;
	if (in != out) {	/* test if input is non-zero */
		if(!(out = (char *) calloc(sizeit * sizeof(char)
		+ sizeof(size_t) + sizeof(id),1))) {
#ifdef RKOERROR
			(void) rkocpyerror("strnmalloc : malloc error!");
			rkoerrno = RKOMEMERR;
#endif
		}
		(void) strncpy(out, id, sizeof(id));
		out += sizeof(id);
		(void) memcpy(out, &sizeit, sizeof(size_t));
		out += sizeof(size_t);
		(void) strncpy(out, in, n);
		/* *(out + n) = '\0'; */
#ifdef RKOERROR
	} else {
			(void) rkocpyerror("strnmalloc : NULL string!");
			rkoerrno = RKOUSEERR;
#endif
	}
	return out;
}

char *strmalloc(char const *in) {
	char *out = (char *) NULL;
	size_t sizeit;

#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	if (!in) {
#ifdef RKOERROR
		(void) rkocpyerror("strmalloc : NULL string!");
		rkoerrno = RKOUSEERR;
#endif
		return out;
	}

	sizeit = strlen(in);
	out = strnmalloc(in, sizeit);
	if (!out) {	/* test if input is non-zero */
#ifdef RKOERROR
		(void) rkopsterror("strmalloc : ");
#endif
	}
	return out;
}

int strfree(char **str) {
	size_t sizeit;
	char *ptr;
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	if (str != (char **) NULL && *str != (char *) NULL) {
		ptr = *str - sizeof(size_t) - sizeof(id);
		if (strncmp(ptr, id, sizeof(id))) {
#ifdef RKOERROR
			rkoerrno = RKOUSEERR;
			(void) rkocpyerror("strfree : invalid string object!");
#endif
			return -1;
		} else {
			(void) memset((void *) ptr, 0, sizeof(id));
			(void) memcpy(&sizeit, (void *) ptr + sizeof(id),
				sizeof(size_t));
			(void) memset((void *) *str, 0, strlen(*str));
			free((void *) ptr);
			*str = (char *) NULL;
		}
	}
	return 0;
}
