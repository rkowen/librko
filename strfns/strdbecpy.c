static const char RCSID[]="@(#)$Id: strdbecpy.c,v 1.6 2002/08/23 15:18:11 rk Exp $";
static const char AUTHOR[]="@(#)strdbecpy 1.1 11/14/97 R.K.Owen,PhD";

/* strDBEcpy - breaks apart a UNIX filename into its path, basename, and
 *	extension.  As an example: /dir1/dir2/file.d would be decomposed to
 *	path = "/dir1/dir2/", base = "file", ext = ".d"
 *	where the path is tagged by dirchar and the extension with extchar,
 *	if they are NULL, then they will be set to the default of '/' & '.'
 *	respectively.
 *	If an output variables is NULL then that value will not be copied.
 *
 * prototype:
 * char *strDBEcpy(char *path, char *base, char *ext,
 *			char dirchar, char extchar, const char *filename);
 *
 *	O path		character array to contain the path
 *	O base		character array to contain the basename
 *	O ext		character array to contain the extension
 *	I dirchar	path character
 *	I extchar	extension character
 *	I filename	the filename to decompose
 *
 *	returns a pointer to filename
 *
 * author	R.K.Owen,Ph.D.	1994/09/23
 * modified	1997/11/14
 * modified	1998/11/04
 */
/** ** Copyright *********************************************************** **
 ** 									     **
 ** Copyright 1994 by R.K.Owen,Ph.D.		                      	     **
 ** last known email: librko@kooz.sj.ca.us				     **
 **                   rk@owen.sj.ca.us					     **
 ** see LICENSE.LGPL, which must be provided, for details		     **
 ** 									     **
 ** ************************************************************************ **/

#include <stdio.h>
#include <string.h>

/* define default delimiters for directories & extensions */
#define DIRCHAR '/'
#define EXTCHAR '.'

char *strDBEcpy(char *path, char *base, char *ext,
	char dirchar, char extchar, const char *filename) {

	const char *name = filename;
	char *ptr;
	size_t len;

/* set defaults - if needed */
	if (dirchar == '\0') dirchar = DIRCHAR;
	if (extchar == '\0') extchar = EXTCHAR;

/* find the path name */
	ptr = strrchr(name, (int) dirchar);
	if (ptr == (char *) NULL) {		/* no path found */
		if (path != (char *) NULL) {
			*path = '\0';	
		}
	} else {
		len = ptr - name + 1;
		if (path != (char *) NULL) {
			(void) strncpy(path, name, len);
			path[len] = '\0';
		}
		name = ptr + 1;
	}
/* find the base name */
	ptr = strrchr(name, (int) extchar);
	if (ptr == (char *) NULL) {		/* no extension found */
		if (base != (char *) NULL) {
			(void) strcpy(base, name);
		}
	} else {
		len = ptr - name;
		if (base != (char *) NULL) {
			(void) strncpy(base, name, len);
			base[len] = '\0';
		}
		name = ptr;
	}
/* find the extension */
	if (ptr == (char *) NULL) {		/* no extension */
		if (ext != (char *) NULL) {
			*ext = '\0';
		}
	} else {
		if (ext != (char *) NULL) {
			(void) strcpy(ext, name);
		}
	}
	return (char *)filename;
}
