#ifndef _UVEC_H_
#  define _UVEC_H_
/* 
 * RCSID @(#)$Id: uvec.h,v 1.2 2002/02/08 23:10:22 rk Exp $
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
/* define HAVE_STRCASECMP if function exists
 */
#  ifdef __cplusplus
extern "C" {
#  endif

/* Unix Vector enums */

enum uvec_order {UVEC_ASCEND, UVEC_DESCEND
#ifdef HAVE_STRCASECMP
	, UVEC_CASE_ASCEND, UVEC_CASE_DESCEND
#endif
	};

enum uvec_def_str_fns {UVEC_DEFAULT, UVEC_USER, UVEC_STDC
#ifdef HAVE_STRMALLOC
	, UVEC_STRMALLOC
#endif
	};

/* Unix Vector package string functions */

typedef struct {
	/* store the string function type - user can ignore this */
	enum uvec_def_str_fns type;
	/* allocate a string of n+1 chars and put str in it */
	char	*(*str_alloc)(char const *str, size_t n);
	/* free up the string storage */
	int	 (*str_free)(char **str);
} uvec_str;

/* Unix Vector package */

typedef struct {
	char	  tag[5];		/* name tag for this type */
	char	**vector;		/* vector of strings */
	int	  number;		/* current number of list */
	int	  capacity;		/* the possible capacity of vector */
	uvec_str  str_fns;		/* which string functions to use */
} uvec;

int    uvec_set_strfns(enum uvec_def_str_fns type, uvec_str *strfns);
enum   uvec_def_str_fns uvec_get_strfns(void);

uvec  *uvec_ctor_(int cap, uvec_str strfns);
uvec  *uvec_ctor(int cap);
int    uvec_dtor(uvec **uv);
uvec  *uvec_alloc_(uvec_str strfns);
uvec  *uvec_alloc(void);
int    uvec_dealloc(uvec **uv);
int    uvec_init_(uvec *uv, int cap, uvec_str strfns);
int    uvec_init(uvec *uv, int cap);
int    uvec_close(uvec *uv);

int    uvec_exists(uvec const *uv);
int    uvec_capacity(uvec const *uv);
int    uvec_number(uvec const *uv);
int    uvec_end(uvec const *uv);
char **uvec_vector(uvec const *uv);

int    uvec_ninsert(uvec *uv, char const *str, size_t n, int place);
int    uvec_insert(uvec *uv, char const *str, int place);
int    uvec_delete(uvec *uv, int place);
int    uvec_nadd(uvec *uv, char const *str, size_t n);
int    uvec_add(uvec *uv, char const *str);
int    uvec_addl(uvec *uv, ...);
int    uvec_push(uvec *uv, char const *str);
int    uvec_pop(uvec *uv);
int    uvec_unshift(uvec *uv, char const *str);
int    uvec_shift(uvec *uv);

int    uvec_count_tok(char const *token, char const *string);
int    uvec_copy_str(uvec *u, char const *token, char const *string);
int    uvec_copy_vec(uvec *u, char const * const *vec, int number);
int    uvec_copy(uvec *u, uvec const *v);
int    uvec_sort(uvec *uv, enum uvec_order type);
int    uvec_find(uvec *uv, char const *str, enum uvec_order type);
int    uvec_uniq(uvec *uv, enum uvec_order type);
int    uvec_reverse(uvec const *uv);
int    uvec_randomize(uvec const *uv, int seed);
uvec  *str2uvec(char const *token, char const *string);
uvec  *vec2uvec(char const * const *vec, int num);
uvec  *uvec2uvec(uvec const *uv);
char  *uvec2str(uvec const *uv, char const *token);

#  ifdef __cplusplus
	}
#  endif
#endif /* _UVEC_H_ */
