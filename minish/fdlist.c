/* fdlist - tracks the file descriptors / files & actions
 * by R.K. Owen,Ph.D. 10/14/1998
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

#include <string.h>
#include <stdarg.h>
#include <sys/types.h>	/* open */
#include <sys/stat.h>	/* open */
#include <fcntl.h>	/* open */
#include <unistd.h>	/* dup2, close */
#include "librko.h"
#include "minish.h"

#define TAG_LEN		7
static char TAG[TAG_LEN] = "FDLIST";

static const char *str_action(minish_fd_action action) {
	static const char null[] = "(null)";
	static const char *list[] = 
	{"NONE", "CLOSE", "READ", "WRITE", "APPEND", "READWRITE", "REDIRECT"};
	static const char *ptr = null;
	switch (action) {
	case MINISH_FD_NONE:		ptr = list[0]; break;
	case MINISH_FD_CLOSE:		ptr = list[1]; break;
	case MINISH_FD_READ:		ptr = list[2]; break;
	case MINISH_FD_WRITE:		ptr = list[3]; break;
	case MINISH_FD_APPEND:		ptr = list[4]; break;
	case MINISH_FD_READWRITE:	ptr = list[5]; break;
	case MINISH_FD_REDIRECT:	ptr = list[6]; break;
	}
	return ptr;
}

/* process a single element */
static int minish_fdlist_single(minish_fdlist_elem *e) {
	int retval = 0;
	int oflag = 0;
	char buffer[120];
	int fid;

	switch (e->action) {
	case MINISH_FD_NONE:	/* do nothing */
		break;
	case MINISH_FD_CLOSE:
		if(close(e->fd) < 0) {
#ifdef RKOERROR
			(void) sprintf(buffer,"can't close fd='%d'", e->fd);
			(void) rkocpyerror(buffer);
			rkoerrno = RKOIOERR;
#endif
			retval = -1;
		}
		break;
	case MINISH_FD_REDIRECT:
		if (dup2(e->fd2, e->fd)) {
#ifdef RKOERROR
			(void) sprintf(buffer,"can't dup2 fd='%d'->'%d'",
				e->fd, e->fd2);
			(void) rkocpyerror(buffer);
			rkoerrno = RKOIOERR;
#endif
			retval -2;
		}
		break;
	case MINISH_FD_READ:		oflag = O_RDONLY;
		break;
	case MINISH_FD_WRITE:		oflag = O_WRONLY | O_TRUNC | O_CREAT;
		break;
	case MINISH_FD_APPEND:		oflag = O_WRONLY | O_APPEND | O_CREAT;
		break;
	case MINISH_FD_READWRITE:	oflag = O_RDWR | O_CREAT;
		break;
	}
	if (oflag != 0) {	/* must be a file request */
		if ((fid = open(e->file, oflag, 0666)) < 0) {
#ifdef RKOERROR
			(void) sprintf(buffer,"can't open '%s' for fd='%d'",
				e->file, e->fd);
			(void) rkocpyerror(buffer);
			rkoerrno = RKOIOERR;
#endif
			retval -3;
		}
		if (dup2(fid, e->fd)) {
#ifdef RKOERROR
			(void) sprintf(buffer,"can't dup2 '%s' to fd='%d'",
				e->file, e->fd);
			(void) rkocpyerror(buffer);
			rkoerrno = RKOIOERR;
#endif
			retval -4;
		}
	}
	return retval;
}

/*======================================================================*/
/* Public Interface                                                     */
/*======================================================================*/

/* minish_fdlist "initializer"
 */
int minish_fdlist_init(minish_fdlist *fdl) {
	if (fdl == (minish_fdlist *) NULL) {
#ifdef RKOERROR
		(void) rkocpyerror("minish_fdlist_init : null pointer!");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	}
/* can't guarantee that struct will be initialized to 0 hence use "tag" */
	if (!strncmp(fdl->tag,TAG,TAG_LEN)) {
#ifdef RKOERROR
		(void) rkocpyerror("minish_fdlist_init : already initialized!");
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

/* minish_ctor "constructor"
 */
minish_fdlist *minish_fdlist_ctor(void) {
	minish_fdlist *fdl = (minish_fdlist *) NULL;

	if (!(fdl = (minish_fdlist *) malloc(sizeof(minish_fdlist)))) {
#ifdef RKOERROR
		(void) rkocpyerror("minish_fdlist_ctor : malloc error!");
		rkoerrno = RKOMEMERR;
#endif
		return fdl;
	}
	if (minish_fdlist_init(fdl)) {
		free(fdl);
		fdl = (minish_fdlist *)NULL;
	}
	return fdl;
}

/* minish_fdlist "close" - destroy contents
 */
int minish_fdlist_close(minish_fdlist *fdl) {
	minish_fdlist_elem *ptr;
	minish_fdlist_elem *next;

	if (fdl == (minish_fdlist *) NULL) {
#ifdef RKOERROR
		(void) rkocpyerror("minish_fdlist_close : null pointer!");
		rkoerrno = RKOUSEERR;
#endif
		return -1;
	} 
	*(fdl->tag) = '\0';
	ptr = fdl->first;
	while (ptr != (minish_fdlist_elem *) NULL) {
		next = ptr->next;
		free(ptr);
		ptr = next;
	}
	fdl->first = fdl->last = (minish_fdlist_elem *) NULL;

#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}

int minish_fdlist_dtor(minish_fdlist **fdl) {
	int retval = 0;
	if ((retval = minish_fdlist_close(*fdl))) return retval;
	free(*fdl);
	return retval;
}

/* tests if minish_fdlist is properly defined */
int minish_fdlist_exists(minish_fdlist *fdl) {
	int retval = 0;
	if (fdl == (minish_fdlist *) NULL) {
#ifdef RKOERROR
		(void) rkocpyerror("minish_fdlist_exists : null pointer!");
		rkoerrno = RKOUSEERR;
#endif
		retval = 0;
	} else {
		if (strncmp(fdl->tag, TAG, TAG_LEN)) {
#ifdef RKOERROR
		(void) rkocpyerror(
			"minish_fdlist_exists : fdlist doesn't exist!");
		rkoerrno = RKOUSEERR;
#endif
			retval = 0;
		} else {
#ifdef RKOERROR
			rkoerrno = RKO_OK;
#endif
			retval = 1;
		}
	}
	return retval;
}

int minish_fdlist_add(minish_fdlist *fdl, minish_fd_action action,
	int fd, ...) {

	char *str;
	int len;
	va_list vptr;
/* initialize an element */
	minish_fdlist_elem elem =
		{-1, MINISH_FD_NONE, (minish_fdlist_elem *) NULL,
			-1, (char *) NULL};
		
	minish_fdlist_elem *this;

	elem.fd = fd;
	elem.action = action;

	if (!minish_fdlist_exists(fdl)) {
#ifdef RKOERROR
		(void) rkopsterror("minish_fdlist_add : ");
#endif
		return -1;
	}
	if (fd < 0) {
#ifdef RKOERROR
		(void) rkocpyerror("minish_fdlist_add : invalid fd");
		rkoerrno = RKOUSEERR;
#endif
		return -2;
	}
/* find action */
	switch (action) {
	case MINISH_FD_CLOSE:
		break;

	case MINISH_FD_READ:
	case MINISH_FD_WRITE:
	case MINISH_FD_APPEND:
	case MINISH_FD_READWRITE:
		va_start(vptr, fd);
		/* get file name */
		if ((str = va_arg(vptr, char *)) == (char *) NULL)
			goto strerror;
		len = strlen(str);
		va_end(vptr);
		if (len <= 0 || len > FILENAME_MAX) strerror: {
#ifdef RKOERROR
			(void) rkocpyerror(
				"minish_fdlist_add : invalid file name");
			rkoerrno = RKOUSEERR;
#endif
			return -3;
		}
		if ((elem.file = (char *) malloc(len + 1)) == (char *) NULL) {
#ifdef RKOERROR
			(void) rkocpyerror(
				"minish_fdlist_add : filename malloc error");
			rkoerrno = RKOMEMERR;
#endif
			return -4;
		}
		(void) strcpy(elem.file, str);
		break;
	case MINISH_FD_REDIRECT:
		va_start(vptr, fd);
		elem.fd2 = va_arg(vptr, int);	/* get 2nd fd */
		va_end(vptr);
		if (elem.fd2 < 0) {
#ifdef RKOERROR
			(void) rkocpyerror(
				"minish_fdlist_add : 2nd fd error");
			rkoerrno = RKOUSEERR;
#endif
			return -5;
		}
		break;
	default:
#ifdef RKOERROR
			(void) rkocpyerror(
				"minish_fdlist_add : impossible action error");
			rkoerrno = RKOGENERR;
#endif
			return -6;
	}

/* successful so far - now create next element in list */
	if ((this = (minish_fdlist_elem *) malloc(sizeof(minish_fdlist_elem)))
	== (minish_fdlist_elem *) NULL) {
#ifdef RKOERROR
		(void) rkocpyerror(
			"minish_fdlist_add : element malloc error");
		rkoerrno = RKOMEMERR;
#endif
		free(elem.file); /* release this if necessary */
		return -7;
	}
/* copy info into elem (use shallow copy) */
	(void) memcpy(this, &elem, sizeof(minish_fdlist_elem));

/* append element onto list */
	if (fdl->first == (minish_fdlist_elem *) NULL) fdl->first = this;
	if (fdl->last  != (minish_fdlist_elem *) NULL) fdl->last->next = this;
	fdl->last = this;

/* all done */
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}

/* dump all elements to stderr - for debug purposes */
int minish_fdlist_dump(minish_fdlist *fdl, FILE *file) {
	minish_fdlist_elem *ptr;
	char *str;
	char none[] = "(none)";

	if (! minish_fdlist_exists(fdl)) {
#ifdef RKOERROR
		(void) rkopsterror("minish_fdlist_dump : ");
#endif
		return -1;
	}
	ptr = fdl->first;
	(void) fprintf(file, "<minish_fdlist_dump>\n");
	while (ptr != (minish_fdlist_elem *) NULL) {
		if (ptr->file == (char *) NULL) {
			str = none;
		} else {
			str = ptr->file;
		}
		(void) fprintf(file,
#if 0
		"  prt=%p fd=%2d fd2=%2d file=%-15.15s action=%9s next=%p\n",
			ptr, ptr->fd, ptr->fd2, str,
			str_action(ptr->action), ptr->next
#else
		"  fd=%2d fd2=%2d file=%-25.25s action=%9s\n",
			ptr->fd, ptr->fd2, str,
			str_action(ptr->action)
#endif
		);
		ptr = ptr->next;
	}

#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}

/* process all the elements
 *	only return an error if fdlist is bogus, else left to
 *	the application to check rkoerrno;
 */
int minish_fdlist_process(minish_fdlist *fdl) {
	minish_fdlist_elem *ptr;
	int oflag = 0;

	if (! minish_fdlist_exists(fdl)) {
#ifdef RKOERROR
		(void) rkopsterror("minish_fdlist_process : ");
#endif
		return -1;
	}
	ptr = fdl->first;
	while (ptr != (minish_fdlist_elem *) NULL) {
		if (minish_fdlist_single(ptr) < 0) {
#ifdef RKOERROR
			rkoperror("minish");
#endif
		}
		ptr = ptr->next;
	}

#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}

#ifdef FDTEST

int main () {
	minish_fdlist *listA = (minish_fdlist *) NULL;
	char buffer[200];

	if (minish_fdlist_dump(listA, stderr)) rkoperror("main");
	if (!(listA = minish_fdlist_ctor())) rkoperror("main");
	if (minish_fdlist_dump(listA, stderr)) rkoperror("main");
	if (minish_fdlist_add(listA, MINISH_FD_REDIRECT, -1, 99))
		rkoperror("main");
	if (minish_fdlist_add(listA, MINISH_FD_REDIRECT, 0, 99))
		rkoperror("main");
	if (minish_fdlist_add(listA, MINISH_FD_WRITE,
		1, "stdout_file")) rkoperror("main");
	if (minish_fdlist_add(listA, MINISH_FD_APPEND,
		2, "stderr_file")) rkoperror("main");
	if (minish_fdlist_add(listA, MINISH_FD_READ,
		3, "file_3_read")) rkoperror("main");
	if (minish_fdlist_add(listA, MINISH_FD_WRITE,
		3, "file_3_write")) rkoperror("main");
	if (minish_fdlist_add(listA, MINISH_FD_READWRITE,
		4, "file_4_readwrite")) rkoperror("main");
	if (minish_fdlist_add(listA, MINISH_FD_APPEND,
		5, "file_5_append")) rkoperror("main");
	if (minish_fdlist_add(listA, MINISH_FD_CLOSE,
		6)) rkoperror("main");
	if (minish_fdlist_dump(listA, stderr)) rkoperror("main");

	if (minish_fdlist_close(listA)) rkoperror("main");

/* this is the actual list to I/O */
	if (!(listA = minish_fdlist_ctor())) rkoperror("main");
/* send stdout to "fdtest.out" */
	if (minish_fdlist_add(listA, MINISH_FD_WRITE, 
		STDOUT_FILENO, "fdtest.out")) rkoperror("main");
/* redirect stderr to stdout */
	if (minish_fdlist_add(listA, MINISH_FD_REDIRECT, 
		STDERR_FILENO, STDOUT_FILENO)) rkoperror("main");
	if (minish_fdlist_add(listA, MINISH_FD_READWRITE, 
		3, "fdtest.3inout")) rkoperror("main");
	if (minish_fdlist_add(listA, MINISH_FD_APPEND, 
		4, "fdtest.4out")) rkoperror("main");

	if (minish_fdlist_dump(listA, stderr)) rkoperror("main");

/* process file requests */
	if (minish_fdlist_process(listA)) rkoperror("main");

/* write something to the files */
	strcpy(buffer,"fdtest -> stdout\n");
	write(STDOUT_FILENO, buffer, strlen(buffer));
	strcpy(buffer,"fdtest -> stderr\n");
	write(STDERR_FILENO, buffer, strlen(buffer));
	read(3, buffer, strlen("fdlist data in \n"));
	buffer[12] = 'o';
	buffer[13] = 'u';
	buffer[14] = 't';
	buffer[16] = '\0';
	write(3, buffer, strlen(buffer));
	strcpy(buffer,"fdtest -> readwrite\n");
	write(3, buffer, strlen(buffer));
	strcpy(buffer,"fdtest -> append\n");
	write(4, buffer, strlen(buffer));

	if (minish_fdlist_dtor(&listA)) rkoperror("main");

	return 0;
}
#endif /* FDTEST */
