#ifndef _MINISH_H_
#  define _MINISH_H_
/* 
 * RCSID @(#)$Id: minish.h,v 1.3 1998/10/21 22:30:32 rk Exp $
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

#  ifdef __cplusplus
extern "C" {
#  endif

/* fdlist - tracks the file descriptors / files & actions
 */

typedef enum {	MINISH_FD_NONE, MINISH_FD_CLOSE,
		MINISH_FD_READ, MINISH_FD_WRITE,
		MINISH_FD_APPEND, MINISH_FD_READWRITE,
		MINISH_FD_REDIRECT } minish_fd_action;

typedef struct minish_fdlist minish_fdlist;	/* forward declaration */
typedef struct minish_fdlist_elem minish_fdlist_elem;

struct minish_fdlist {
	char tag[7];			/* name tag for type	*/
	minish_fdlist_elem *first;	/* head of list		*/
	minish_fdlist_elem *last;	/* last in list		*/
};

struct minish_fdlist_elem {
	int fd;				/* file descriptor	*/
	minish_fd_action action;	/* file access action	*/
	minish_fdlist_elem *next;	/* next in chain	*/
	int fd2;			/* redirected to	*/
	char *file;			/* file name		*/
};

minish_fdlist *minish_fdlist_ctor();
int minish_fdlist_init(minish_fdlist *fdl);
int minish_fdlist_close(minish_fdlist *fdl);
int minish_fdlist_dtor(minish_fdlist **fdl);
int minish_fdlist_exists(minish_fdlist *fdl);
int minish_fdlist_add(minish_fdlist *fdl, minish_fd_action action, int fd, ...);
int minish_fdlist_dump(minish_fdlist *fdl, FILE *file);
int minish_fdlist_process(minish_fdlist *fdl);

#  ifdef __cplusplus
	}
#  endif
#endif /* _MINISH_H_ */
