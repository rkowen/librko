/* tdirtree - tests dirtree() & recurses through the current directory tree
 *
 * by R.K. Owen,Ph.D.	10/31/98
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#include "librko.h"

static const char *strfilemode(mode_t m) {
	static const char	reg[] = "",	/* regular file */
				sym[] = "@",	/* symbolic link - not posix */
				dir[] = "/",	/* directory */
				chr[] = "*c",	/* character device */
				blk[] = "*b",	/* block device */
				fifo[] = "|",	/* fifo */
				sock[] = "=",	/* socket - not posix */
				none[] = "?";	/* (unknown) */
	const char *ptr;
	if(S_ISREG(m)) {
		ptr = reg;
	} else if(S_ISDIR(m)) {
		ptr = dir;
#ifdef S_ISLNK
	} else if(S_ISLNK(m)) {
		ptr = sym;
#endif
	} else if(S_ISCHR(m)) {
		ptr = chr;
	} else if(S_ISBLK(m)) {
		ptr = blk;
	} else if(S_ISFIFO(m)) {
		ptr = fifo;
#ifdef S_ISSOCK
	} else if(S_ISSOCK(m)) {
		ptr = sock;
#endif
	} else {
		ptr = none;
	}
	return ptr;
}

int level = 0;

int enterdir(const char *dirname) {
	level++;
	return 0;
}

int leavedir(const char *dirname) {
	level--;
	return 0;
}

int trimlistdir(const char *dirname) {
	int i;
	char basename[FILENAME_MAX];
	char extension[FILENAME_MAX];

	for (i = 0; i < level; ++i) {
		printf("\t");
	}
	(void) strDBEcpy(NULL, basename, extension, 0, 0, dirname);
	printf(">>> %s%s\n", basename, extension);
	return 0;
}

int trimlistfile(const char *filename) {
	struct stat sbuf;
	int i;
	char basename[FILENAME_MAX];
	char extension[FILENAME_MAX];

	(void) lstat(filename, &sbuf);
	if (!S_ISDIR(sbuf.st_mode)) {
		for (i = 0; i < level; ++i) {
			printf("\t");
		}
		(void) strDBEcpy(NULL, basename, extension, 0, 0, filename);
		printf("%s%s%s\n", basename, extension,
			strfilemode(sbuf.st_mode));
	}
	return 0;
}

int trimlist(const char *filename) {
	struct stat sbuf;
	int i;
	char basename[FILENAME_MAX];
	char extension[FILENAME_MAX];

	(void) lstat(filename, &sbuf);
	for (i = 0; i < level; ++i) {
		printf("\t");
	}
	(void) strDBEcpy(NULL, basename, extension, 0, 0, filename);
	printf("%s%s%s\n", basename, extension,
		strfilemode(sbuf.st_mode));
	return 0;
}

int main(int argc, char *argv[]) {
	char	*PrgName;	/* program name			*/
	int err = 0;

	PrgName = *argv++;
	printf("\nSorted list with `basenames' only, no dir links\n");
	if (err = dirtree(-1, -1, 0, "tdir",
	trimlistdir, trimlistfile, enterdir, leavedir)) {
#ifdef RKOERROR
		rkoperror(PrgName);
#endif
		return err;
	}
	printf("\nSorted list with `basenames' of directories only\n");
	if (err = dirtree(-1, -1, -1, "tdir",
	trimlistdir, NULL, enterdir, leavedir)) {
#ifdef RKOERROR
		rkoperror(PrgName);
#endif
		return err;
	}
	printf("\nSorted list of directories, but don't follow links\n");
	if (err = dirtree(-1, -1, 0, "tdir",
	trimlistdir, NULL, enterdir, leavedir)) {
#ifdef RKOERROR
		rkoperror(PrgName);
#endif
		return err;
	}
	printf("\nSorted list of directories, follow 1 links\n");
	if (err = dirtree(-1, -1, 1, "tdir",
	trimlistdir, NULL, enterdir, leavedir)) {
#ifdef RKOERROR
		rkoperror(PrgName);
#endif
		return err;
	}
	printf("\nSorted list of directories, descend 2 dir levels, no links\n");
	if (err = dirtree(-1, 2, 0, "tdir",
	trimlistdir, NULL, enterdir, leavedir)) {
#ifdef RKOERROR
		rkoperror(PrgName);
#endif
		return err;
	}
	printf("\nSorted list of directories, no dir levels, no links\n");
	if (err = dirtree(-1, 0, 0, "tdir",
	trimlistdir, NULL, enterdir, leavedir)) {
#ifdef RKOERROR
		rkoperror(PrgName);
#endif
		return err;
	}
	return 0;
}
