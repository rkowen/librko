#include <stdio.h>
#include "librko.h"

void testout(	char *path, char *base, char *ext,
		const char *tpath, const char *tbase, const char *text,
		char dirchar, char extchar,
		const char *filename) {

	int err = 0;
	char *null = "(null)";

	strDBEcpy(path, base, ext, dirchar, extchar, filename);

	if (path != (char *) NULL) {
		err += (strcmp(path, tpath) ? 1 : 0);
	}
	if (base != (char *) NULL) {
		err += (strcmp(base, tbase) ? 1 : 0);
	}
	if (ext != (char *) NULL) {
		err += (strcmp(ext, text) ? 1 : 0);
	}

	if (err) {
		printf("FAIL:");
	} else {
		printf("OK  :");
	}
	if (path != (char *) NULL) {
		printf("%18s ", path);
	} else {
		printf("%18s ", null);
	}
	if (base != (char *) NULL) {
		printf("%10s ", base);
	} else {
		printf("%10s ", null);
	}
	if (ext != (char *) NULL) {
		printf("%5s : ", ext);
	} else {
		printf("%5s : ", null);
	}
	printf("%s\n", filename);
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

	char path[40], base[20], ex[10];

	testout(path, base, ex, dir1, basename, ext1, '\\', ':',
		"\\dir1\\dir2\\dir3\\basename:ext");
	testout(path, base, ex, dir, basename, ext, 0, 0,
		"/dir1/dir2/dir3/basename.ext");
	testout(path, base, ex, dir, basename, ext, '/', '.',
		"/dir1/dir2/dir3/basename.ext");
	testout(path, base, ex, dir2, basename, ext, 0, 0,
		"/dir1/basename.ext");
	testout(path, base, ex, dir, basename, null, 0, 0,
		"/dir1/dir2/dir3/basename");
	testout(path, base, ex, dir, null, ext, 0, 0,
		"/dir1/dir2/dir3/.ext");
	testout(path, base, ex, dir, null, null, 0, 0,
		"/dir1/dir2/dir3/");
	testout(path, base, ex, null, basename, ext, 0, 0,
		"basename.ext");
	testout(path, base, ex, null, basename, ext2, 0, 0,
		"basename.");
	testout(path, base, ex, null, basename, null, 0, 0,
		"basename");
	testout(path, base, ex, null, null, ext, 0, 0,
		".ext");

	testout(NULL, base, ex, dir, basename, ext, 0, 0,
		"/dir1/dir2/dir3/basename.ext");
	testout(path, NULL, ex, dir, basename, ext, 0, 0,
		"/dir1/dir2/dir3/basename.ext");
	testout(path, base, NULL, dir, basename, ext, 0, 0,
		"/dir1/dir2/dir3/basename.ext");
	testout(NULL, NULL, ex, dir, basename, ext, 0, 0,
		"/dir1/dir2/dir3/basename.ext");
	testout(path, NULL, NULL, dir, basename, ext, 0, 0,
		"/dir1/dir2/dir3/basename.ext");
	testout(NULL, base, NULL, dir, basename, ext, 0, 0,
		"/dir1/dir2/dir3/basename.ext");
	testout(NULL, NULL, NULL, dir, basename, ext, 0, 0,
		"/dir1/dir2/dir3/basename.ext");

	return 0;
}
