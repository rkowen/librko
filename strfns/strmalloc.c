static const char RCSID[]="@(#)$Id: strmalloc.c,v 1.1 1999/09/09 05:46:49 rk Exp $";
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
 *
 *********************************************************************
 */

#include <strings.h>
#include "librko.h"
#ifdef RKOERROR
extern int rkoerrno;
#endif

static char id[3] = "STR";

char *strmalloc(const char *in) {
	char *out = (char *) NULL;
	size_t sizeit;
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	sizeit = strlen(in) + 1;
	if (in != out) {	/* test if input is non-zero */
		if(!(out = (char *) malloc(sizeit * sizeof(char)
		+ sizeof(size_t) + sizeof(id)))) {
#ifdef RKOERROR
			(void) rkocpyerror("strmalloc : malloc error!");
			rkoerrno = RKOMEMERR;
#endif
		}
		(void) strncpy(out, id, sizeof(id));
		out += sizeof(id);
		(void) memcpy(out, &sizeit, sizeof(size_t));
		out += sizeof(size_t);
		(void) strcpy(out, in);
#ifdef RKOERROR
	} else {
			(void) rkocpyerror("strmalloc : null input!");
			rkoerrno = RKOUSEERR;
#endif
	}
	return out;
}

void strfree(char **str) {
	size_t sizeit;
	char *ptr = *str - sizeof(size_t) - sizeof(id);
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	if (*str != (char *) NULL) {
		if (strncmp(ptr, id, sizeof(id))) {
			rkoerrno = RKOUSEERR;
			(void) rkocpyerror("strfree : invalid string object!");
		} else {
			(void) memset((void *) ptr, 0, sizeof(id));
			(void) memcpy(&sizeit, (void *) ptr + sizeof(id),
				sizeof(size_t));
			(void) memset((void *) *str, 0, strlen(*str));
			free((void *) ptr);
			*str = (char *) NULL;
		}
#ifdef RKOERROR
	} else {
			(void) rkocpyerror("strfree : null input!");
			rkoerrno = RKOUSEERR;
#endif
	}
}
