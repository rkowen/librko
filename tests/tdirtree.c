/* tdirtree - tests dirtree() & recurses through the current directory tree
 *
 * by R.K. Owen,Ph.D.	10/31/98
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
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

int listdir(const char *dirname) {
	int i;

	for (i = 0; i < level; ++i) {
		printf("\t");
	}
	printf(">>> %s\n", dirname);
	return 0;
}

int listfile(const char *filename) {
	struct stat sbuf;
	int i;

	(void) lstat(filename, &sbuf);
	if (!S_ISDIR(sbuf.st_mode)) {
		for (i = 0; i < level; ++i) {
			printf("\t");
		}
		printf("%s%s\n", filename, strfilemode(sbuf.st_mode));
	}
	return 0;
}

int main(int argc, char *argv[]) {
	char	*PrgName;	/* program name			*/
	int err = 0;

	PrgName = *argv++;
	listdir(".");
	if (err = dirtree(1,".", listdir, listfile, enterdir, leavedir)) {
#ifdef RKOERROR
		rkoperror(PrgName);
#endif
		return err;
	}
	return 0;
}
