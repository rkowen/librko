#include <stdio.h>
#include "librko.h"

void testout(	const char *tpath, const char *tbase, const char *text,
		char dirchar, char extchar,
		const char *filename) {

	char path[40], base[20], ext[10];

	strDBEcpy(path, base, ext, dirchar, extchar, filename);

	if (!strcmp(path, tpath) && !strcmp(base,tbase)
		&& !strcmp(ext,text)) {
		printf("OK  :");
	} else {
		printf("FAIL:");
	}
	printf("%18s %10s %5s : %s\n",
		path, base, ext, filename);
}

int main() {
	char *dir  = "/dir1/dir2/dir3/";
	char *dir1 = "\\dir1\\dir2\\dir3\\";
	char *dir2 = "/dir1/";
	char *basename = "basename";
	char *ext  = ".ext";
	char *ext1 = ":ext";
	char *ext2 = ".";
	char *null = "";

	testout(dir1, basename, ext1, '\\', ':',
		"\\dir1\\dir2\\dir3\\basename:ext");
	testout(dir, basename, ext, 0, 0,
		"/dir1/dir2/dir3/basename.ext");
	testout(dir, basename, ext, '/', '.',
		"/dir1/dir2/dir3/basename.ext");
	testout(dir2, basename, ext, 0, 0,
		"/dir1/basename.ext");
	testout(dir, basename, null, 0, 0,
		"/dir1/dir2/dir3/basename");
	testout(dir, null, ext, 0, 0,
		"/dir1/dir2/dir3/.ext");
	testout(dir, null, null, 0, 0,
		"/dir1/dir2/dir3/");
	testout(null, basename, ext, 0, 0,
		"basename.ext");
	testout(null, basename, ext2, 0, 0,
		"basename.");
	testout(null, basename, null, 0, 0,
		"basename");
	testout(null, null, ext, 0, 0,
		".ext");

	return 0;
}
