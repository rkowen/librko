#ifndef _AVEC_H_
#  define _AVEC_H_
/* 
 * RCSID @(#)$Id: avec.h,v 1.1 2002/02/08 23:10:22 rk Exp $
 */
/*
 *********************************************************************
 *
 *     This software is copyrighted by R.K.Owen,Ph.D. 2001
 *
 * The author, R.K.Owen, of this software is not liable for any
 * problems WHATSOEVER which may result from use  or  abuse  of
 * this software. The author, R.K.Owen, grants unlimited rights
 * to anyone who uses and modifies this  software  for  private
 * non-commercial  use  as  long  as  this copyright and rights
 * notice remains in this software and is made available to all
 * recipients of this software.
 *
 * last known email: librko@kooz.sj.ca.us
 *                   rk@owen.sj.ca.us
 *
 *********************************************************************
 */
#  ifdef __cplusplus
extern "C" {
#  endif

/* Associative Vector (hash array) package */

enum avec_def_fns {AVEC_DEFAULT, AVEC_USER, AVEC_STDC
#  ifdef HAVE_STRMALLOC
	, AVEC_STRMALLOC
#endif
	};


typedef struct {
	/* store the data type - user can ignore this */
	enum avec_def_fns	type;
	int  (*data_add)(void **,va_list); /* allocate a data element */
	int  (*data_del)(void **,va_list); /* dealloc  a data element */
} avec_fns;
	
typedef struct {
	char		 *key;		/* key for associative array */
	void		 *data;		/* container for datam */
} avec_element;

typedef struct {
	char		  tag[5];	/* name tag for this type */
	avec_element	**hash;		/* container for keys & data */
	int		  number;	/* current number of hash */
	int		  capacity;	/* the possible capacity of hash */
	avec_fns	  fns;		/* which alloc fns to use */
} avec;


int   avec_set_fns(enum avec_def_fns type, avec_fns *fns);
enum  avec_def_fns avec_get_fns(void);

avec  *avec_ctor_(int cap, avec_fns fns);
avec  *avec_ctor(int cap);
int    avec_dtor(avec **av, ...);
avec  *avec_alloc_(avec_fns strfns);
avec  *avec_alloc(void);
int    avec_dealloc(avec **av);
int    avec_init_(avec *av, int cap, avec_fns fns);
int    avec_init(avec *av, int cap);
int    avec_close(avec *av, ...);

int    avec_exists(avec const *av);
int    avec_capacity(avec const *av);
int    avec_number(avec const *av);
 
int    avec_increase(avec *av, int newcap);
int    avec_decrease(avec *av, int newcap);

int    avec_insert(avec *av, char const *key, ...);
int    avec_delete(avec *av, char const *key, ...);
void  *avec_lookup(avec *av, char const *key);
void  *avec_walk(avec *av);
void  *avec_walk_r(avec *av, void **ptrptr);

#  ifdef __cplusplus
	}
#  endif
#endif /* _AVEC_H_ */
