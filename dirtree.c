static const char RCSID[]="@(#)$Id: dirtree.c,v 1.7 1998/11/23 15:55:12 rk Exp $";
static const char AUTHOR[]="@(#)dirtree 1.0 10/31/98 R.K.Owen,Ph.D.";
/* dirtree - recurses through a directory tree and executes
 *	four user functions
 *		1) entry into dirtree() & directory
 *		2) function regarding directory in the list
 *		3) function regarding files in the list
 *		4) leaving dirtree() & directory
 *	dirtree does not change the current working directory, that
 *	can be performed by the user functions.  If a given
 *	user function is NULL, then that action will be skipped.
 *		sort =	if non-zero dirtree sorts all lists and perform
 *			all recursions and file actions on these ASCII
 *			alphabetically sorted lists, else dirtree will
 *			process the lists as given in the directory
 *			files.
 *		dirlvl=	if > 0 then descend only to the dirlvl-th
 *			subdirectory.  To descend into all the
 *			directories then set < 0.  If set = 0 then
 *			process only the entries in the given directory.
 *		lnklvl=	if > 0 then follow only the lnklvl-th
 *			subdirectory symbolic link.  To follow all the
 *			directory links then set < 0.  If set = 0 then
 *			don't follow any symbolic links.
 *
 * by R.K. Owen,Ph.D.   10/29/98
 */
/*
 *********************************************************************
 *
 *     This software is copyrighted by R.K.Owen,Ph.D. 1998
 *
 * The author, R.K.Owen, of this software is not liable for any
 * problems WHATSOEVER which may result from use  or  abuse  of
 * this software. The author, R.K.Owen, grants unlimited rights
 * to anyone who uses and modifies this  software  for  private
 * non-commercial  use  as  long  as  this copyright and rights
 * notice remains in this software and is made available to all
 * recipients of this software.
 *
 * last known email: rkowen@ckns.net
 *                   rk@owen.sj.ca.us
 *
 *********************************************************************
 */

#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>		/* stat/lstat */
#include <dirent.h>
#include <unistd.h>		/* stat/lstat, getcwd, chdir */
#include "librko.h"
#ifdef RKOERROR
extern int rkoerrno;
#endif

int dirtree(int sort, int dirlvl, int lnklvl, const char *dir,
	int (dirfn)(const char *), int (filefn)(const char *),
	int (direnter)(const char *), int (dirleave)(const char *)) {

	DIR	*dr;		/* Directory file descriptor    */
	struct	dirent *de;	/* Directory entry              */
	uvec	*dirvec = (uvec *) NULL;	/* list of sub dirs */
	uvec	*regvec = (uvec *) NULL;	/* list of regular files */
	char	**vecptr;			/* for cycling through list */
	struct stat sbuf;			/* file stat 	*/
	char	thislevel[FILENAME_MAX];	/* full path name */
	char	nextlevel[FILENAME_MAX];	/* subdirectory name */
	int	err;
	int	nextlnklvl = lnklvl-1;		/* what to pass to next level */

	/* execute user direnter function */
	if (direnter != (int (*)(const char *)) NULL) {
		if (direnter(dir) < 0) {
#ifdef RKOERROR
			rkocpyerror("dirtree : user direnter fn error");
			rkoerrno = RKOUSEERR;
#endif
			return -1;
		}
	}

	/* see if full path is given else do tricks to get full path name */
	if (*dir != '/' || strstr(dir, "./") || dir[strlen(dir)-1] == '.') {
		if (getcwd(nextlevel, FILENAME_MAX) == (char *) NULL) {
#ifdef RKOERROR
			rkocpyerror("dirtree : getcwd this dir  error");
			rkoerrno = RKOIOERR;
#endif
			return -1;
		}
		/* go to the specified directory */
		if (chdir(dir) < 0) {
#ifdef RKOERROR
			rkocpyerror("dirtree : chdir error");
			rkoerrno = RKOIOERR;
#endif
			return -1;
		}
		/* get full path */
		if (getcwd(thislevel, FILENAME_MAX) == (char *) NULL) {
#ifdef RKOERROR
			rkocpyerror("dirtree : getcwd full dir  error");
			rkoerrno = RKOIOERR;
#endif
			return -1;
		}
		/* return back again */
		if (chdir(nextlevel) < 0) {
#ifdef RKOERROR
			rkocpyerror("dirtree : chdir return error");
			rkoerrno = RKOIOERR;
#endif
			return -1;
		}
	} else {
		(void) strcpy(thislevel, dir);
		if (thislevel[strlen(thislevel)-1] == '/') {
			thislevel[strlen(thislevel)-1] = '\0';
		}
	}

	/* Open the directory for processing */
	if((dr = opendir(thislevel)) == NULL) {
#ifdef RKOERROR
		rkocpyerror("dirtree : opendir error");
		rkoerrno = RKOIOERR;
#endif
		return -2;
	}
	/* setup vectors for dirs & files */
	if ((dirvec = uvec_ctor(10)) == (uvec *) NULL) {
#ifdef RKOERROR
		rkopsterror("dirtree : dirvec error : ");
#endif
		return -3;
	}
	if ((regvec = uvec_ctor(10)) == (uvec *) NULL) {
#ifdef RKOERROR
		rkopsterror("dirtree : regvec error : ");
#endif
		return -4;
	}

	/* Read the directory */
	while ((de = readdir(dr)) != (struct dirent *) NULL) {
		if (*(de->d_name) == '.') {	/* check for dot files */
			if (de->d_name[1] == '\0')	continue; /* skip . */
			if (de->d_name[1] == '.'
			&&  de->d_name[2] == '\0')	continue; /* skip .. */
		}
		(void) strcpy(nextlevel, thislevel);
		(void) strcat(nextlevel, "/");	/* posix defined */
		(void) strcat(nextlevel, de->d_name);
#ifdef S_ISLNK		/* only worry about soft links if they exist at all */
		if (lnklvl) {
#endif
			if((err = stat(nextlevel, &sbuf))) {
				continue;
			}
#ifdef S_ISLNK
		} else {
			nextlnklvl = 0;	/* pass 0 to next level */
			if((err = lstat(nextlevel, &sbuf))) {
				continue;
			}
		}
#endif
		/* put directory entries into a list */
		if (S_ISDIR(sbuf.st_mode)) {
			if (uvec_add(dirvec, de->d_name)) {
#ifdef RKOERROR
				rkopsterror("dirtree : dirvec error : ");
#endif
				return -5;
			}
		}
		/* put all directory entrie into list ... let user decide
		   what to do with them */
		if (uvec_add(regvec, de->d_name)) {
#ifdef RKOERROR
			rkopsterror("dirtree : regvec error : ");
#endif
			return -6;
		}
	}
	/* sort dir & file vectors */
	if (sort) {
		if (uvec_sort(dirvec, UVEC_ASCEND)) {
#ifdef RKOERROR
			rkopsterror("dirtree : dirvec error : ");
#endif
			return -7;
		}
		if (uvec_sort(regvec, UVEC_ASCEND)) {
#ifdef RKOERROR
			rkopsterror("dirtree : regvec error : ");
#endif
			return -8;
		}
	}

	/* perform action on files */
#ifdef RKOERROR
	rkocpyerror(": dirtree");
#endif
	if (filefn != (int (*)(const char *)) NULL) {
		for (vecptr = uvec_vector(regvec);
		*vecptr != (char *) NULL; ++vecptr) {
			(void) strcpy(nextlevel, thislevel);
			(void) strcat(nextlevel, "/");
			(void) strcat(nextlevel, *vecptr);

			if ((err = filefn(nextlevel)) != 0) {
#ifdef RKOERROR
				rkopsterror(")");
				rkopsterror(*vecptr);
				rkopsterror("file(");
				rkoerrno = RKOUSEERR;
#endif
				return err;
			}
		}
	}
	/* now recurse through tree - if desired */
#ifdef RKOERROR
	rkocpyerror(": dirtree");
#endif
	for (vecptr = uvec_vector(dirvec);
	*vecptr != (char *) NULL; ++vecptr) {
		(void) strcpy(nextlevel, thislevel);
		(void) strcat(nextlevel, "/");		/* posix defined */
		(void) strcat(nextlevel, *vecptr);

		/* execute user function on dir */
		if (dirfn != (int (*)(const char *)) NULL) {
			(void) dirfn(nextlevel);
		}
		if (dirlvl) {
			if ((err = dirtree(sort, dirlvl-1, nextlnklvl, nextlevel,
			dirfn, filefn, direnter, dirleave)) != 0) {
#ifdef RKOERROR
				rkopsterror(") ");
				rkopsterror(*vecptr);
				rkopsterror("dir(");
				rkoerrno = RKOUSEERR;
#endif
				return err;
			}
		}
	}

	(void) uvec_dtor(&dirvec);
	(void) uvec_dtor(&regvec);
	(void) closedir(dr);

	/* execute user dirleave function */
	if (dirleave != (int (*)(const char *)) NULL) {
		if (dirleave(dir) < 0) {
#ifdef RKOERROR
			rkocpyerror("dirtree : dirleave error");
			rkoerrno = RKOUSEERR;
#endif
			return -9;
		}
	}
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}
