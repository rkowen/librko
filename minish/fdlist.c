/* fdlist - tracks the file descriptors / files & actions
 * by R.K. Owen,Ph.D. 10/14/1998
 */

typedef enum {	MINISH_FD_NONE, MINISH_FD_READ, MINISH_FD_WRITE,
		MINISH_FD_APPEND, MINISH_FD_READWRITE} fd_action;

typedef struct minish_fdlist minish_fdlist;	/* forward declaration */
typedef struct minish_fdlist_elem minish_fdlist_elem;

struct minish_fdlist {
	char tag[7];			/* name tag for type	*/
	minish_fdlist_elem *first;	/* head of list		*/
	minish_fdlist_elem *last;	/* last in list		*/
}

struct minish_fdlist_elem {
	int fd;			/* file descriptor	*/
	int fd2;		/* redirected to	*/
	char *file;		/* file name		*/
	fd_action action;	/* action to perform	*/
	minish_fdlist *next;	/* next in chain	*/
};

#include <string.h>

static char TAG[7] = "FDLIST";

int minish_fdlist_ctor(minish_fdlist *fdl) {
	if (fdl == (minish_fdlist *) NULL) {
#ifdef RKOERROR
		(void) rkocpyerror("minish_fdlist_ctor : null pointer!");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
/* can't guarantee that struct will be initialized to 0 hence use "tag" */
	if (!strncmp(fdl->tag,TAG,7)) {
#ifdef RKOERROR
		(void) rkocpyerror("minish_fdlist_ctor : already initialized!");
		rkoerrno = RKOUSEERR;
#endif
		return -2;
	}
	/* put into a known state */
	(void) strcpy(fdl->tag, TAG);
	fdl->first = fdl->last = (minish_fdlist_elem *) NULL;
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}

/* destroy the fdlist */
int minish_fdlist_dtor(minish_fdlist *fdl) {
	minish_fdlist_elem *ptr = fdl->first;
	minish_fdlist_elem *next;

	if (fdl == (minish_fdlist *) NULL) {
#ifdef RKOERROR
		(void) rkocpyerror("minish_fdlist_dtor : null pointer!");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
	*(fdl->tag) = '\0';
	while (ptr != (minish_fdlist_elem *) NULL) {
		next = ptr->next;
		free(ptr);
		ptr = next;
	}
	fdl->first = fdl->next = (minish_fdlist_elem *) NUL;

#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}
