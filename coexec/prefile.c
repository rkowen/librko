static const char RCSID[]="@(#)$Id: prefile.c,v 1.1 2003/08/25 18:12:50 rk Exp $";
static const char AUTHOR[]="@(#)coexec 1.0 2002/02/28 R.K.Owen,Ph.D.";
/** ** Copyright *********************************************************** **
 ** 									     **
 ** Copyright 2003 by R.K.Owen,Ph.D.		                      	     **
 ** last known email: librko@kooz.sj.ca.us				     **
 **                   rk@owen.sj.ca.us					     **
 ** see LICENSE.LGPL, which must be provided, for details		     **
 ** 									     **
 ** ************************************************************************ **/
#define _POSIX_SOURCE

#include <stdio.h>
#include <stdlib.h>			/* memset,getenv */
#include <sys/types.h>			/* wait */
#include <sys/wait.h>			/* wait */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#include "coexec.h"			/* invoke */
#ifdef RKOERROR
#  include "rkoerror.h"
#endif

#if defined(GZIP)
	#define filepos 3
	char *pargv[] = {
		GZIP,
		"-c",
		"-d",
		NULL,
		NULL
	};
#elif defined(UNCOMPRESS)
	#define filepos 2
	char *pargv[] = {
		UNCOMPRESS,
		"-c",
		NULL,
		NULL
	};
#else
#  define DO_NOTHING
#endif

char pfile[FILENAME_MAX];	/* static buffer for temporary file name */
char zfile[FILENAME_MAX];	/* static buffer of file which was found */

/* return 1 if file exists, else 0 */
static int tryfile(char *infile, char *ext) {
	FILE *t;
	strcpy(zfile, infile);
	strcat(zfile, ext);
	if (!(t = fopen(zfile,"r"))) return 0;	/* not there or readable */
	fclose(t);
	return 1;
}

#ifndef DO_NOTHING
/* return temporary file name else NULL use $TMPDIR if there */
static char * temp_filename(void) {
	char *TMPDIR, *TMPFILE, *ptr;
	/* check if TMPDIR exists else use /tmp */
	if (!(TMPDIR = getenv("TMPDIR")))
		TMPDIR = "/tmp";
	/* get a temp filename - std C */
	if (!(TMPFILE = tmpnam(NULL)))
		return (char *) NULL;		/* failure */
	/* just get file name - strip out any possible path */
	if (!(ptr = strrchr(TMPFILE,'/')))	/* whole name - no path */
		ptr = TMPFILE;
	else	ptr++;

	strcpy(pfile, TMPDIR);
	strcat(pfile, "/");
	strcat(pfile, ptr);

	return pfile;
}

static char * uncompress_file(void) {
	FILE	*out;
	FILE	*tmp;
	int	 c;

	/* get a tempfile name */
	if (!temp_filename())			/* failure */
		goto fail1;
	/* open tempfile */
	if (!(tmp = fopen(pfile, "wb")))	/* failure */
		goto fail1;
	/* set the file name for argv */
	pargv[filepos] = zfile;
	/* invoke a sub-process */
	if (!(out = invoke(pargv)))		/* failure */
		goto fail2;

	/* read compressed file  and send to tempfile
	 * (assume buffering is really good)
	 */
	while (!(c = fgetc(out), feof(out)))	fputc(c,tmp);

	fclose(tmp);
	fclose(out);
	(void) wait(NULL);

	return pfile;

fail2:
	fclose(tmp);
fail1:
	return (char *) NULL;
}
#endif

/* prefile.c - checks to see if the file exists, then checks some common
 *	compressed forms, then uncompress the file (if needed) and pass
 *	back the new temporary file name.
 *	If the file can not be found or uncompressed then returns NULL.
 */

char *prefile(char *infile) {
	memset(pfile, 0, FILENAME_MAX);
	if (!infile)	return (char *) NULL;

	int ilen = strlen(infile);
	if (!ilen)	return (char *) NULL;

	/* see if extension is given explicitly */
#ifdef GZIP
	if (!strcmp(infile + ilen - 3, ".gz")) { 	/* gzip */
		if (tryfile(infile, ""))
			return uncompress_file();
	} else
#endif
#if defined(GZIP) || defined(UNCOMPRESS)
	if (!strcmp(infile + ilen - 2, ".Z")) {		/* compress */
		if (tryfile(infile, ""))
			return uncompress_file();
	} else
#endif
#ifdef GZIP
	if (!strcmp(infile + ilen - 2, ".z")) {		/* pack */
		if (tryfile(infile, ""))
			return uncompress_file();
	} else
#endif
	if (tryfile(infile, "")) {			/* plain */
		return infile;
	} else
#ifdef GZIP
	if (tryfile(infile, ".gz")) {			/* gzip */
		return uncompress_file();
	} else
#endif
#if defined(GZIP) || defined(UNCOMPRESS)
	if (tryfile(infile, ".Z")) {			/* compress */
		return uncompress_file();
	} else
#endif
#ifdef GZIP
	if (tryfile(infile, ".z")) {			/* pack */
		return uncompress_file();
	} else
#endif
	{
		return (char *) NULL;
	}
}

/* unlink_prefile - removes the temporary file (if one was created) */
int unlink_prefile(void) {
	int retval = 0;
	if (*pfile) {
		if (!(retval = remove(pfile)))	/* OK */
			memset(pfile, 0, FILENAME_MAX);
	}
	return retval;
}

#if 0
int main () {
	/*
	int i;
	for (i = 0; i < 10; i++) {
		printf("%s\n", temp_filename());
	}
	*/
	
	printf("%s\n", prefile("xxx.c"));
	sleep(60);
	unlink_prefile();

	return 0;
}
#endif
