#ifndef _LIBRKO_H_
#  define _LIBRKO_H_
/* 
 * RCSID @(#)$Id: librko.h,v 1.15 1999/09/09 21:30:43 rk Exp $
 */
/*
 *********************************************************************
 *
 *     This software is copyrighted by R.K.Owen,Ph.D. 1997
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
/* define NO_STRCASECMP if function doesn't exist
 */
#  if 0
#    define NO_STRCASECMP
#  endif

#  include <stdio.h>
#  include <stdarg.h>
#  include <netdb.h>		/* gethostbyname, gethostbyaddr */

#  ifdef __cplusplus
extern "C" {
#  endif

#  ifndef _PRECISION_
#    define _PRECISION_
     typedef double PRECISION;
#  endif

#  ifndef _INTEGER_
#    define _INTEGER_
     typedef int INTEGER;
#  endif

FILE * invoke(char ** argv);
FILE * prefilter(FILE *instream, char ** argv);
FILE * postfilter(FILE *outstream, char ** argv);
int closefilter(FILE *filterstream);
int timedfgets(char *buf, int size, FILE *stream, int seconds);
struct hostent *gethostbyX(char *hostX);
int tcp_connect(char *hostname, int port_num);
int tcp_close(int sd);
int dirtree(int sort, int dirlvl, int lnklvl, const char *dir,
	int (dirfn)(const char *), int (filefn)(const char *),
	int (direnter)(const char *), int (dirleave)(const char *));

char *strchop(const char *string, int chop_len,
        size_t *str_len, char **next);
char *strmalloc(const char *in);
void strfree(char **str);
int strmemalloc(char **ChArSpAcE, char ***charspace,
		size_t numstr, size_t maxstrlen);
void strmemfree(char **ChArSpAcE, char ***charspace);
int wcstrcmp(const char *str, const char *wcstr);
char *macstrncpy(char *string, const char *mstring, size_t n);
extern char *MacroStr[];
extern int MacroTrf[];
#define NAMETOL(a,b)	nametol(a,#b,&b)
#define NAMETOD(a,b)	nametod(a,#b,&b)
int nametol(const char *line, const char *name, long *value);
int nametod(const char *line, const char *name, double *value);
PRECISION foptim(int iopt, PRECISION ax, PRECISION bx,
	PRECISION (*f)(PRECISION), PRECISION tol);
PRECISION fzeroin(PRECISION ax, PRECISION bx,
	PRECISION y0, PRECISION (*f)(PRECISION), PRECISION tol);
int istext(int c);

/* useful integer functions */
/* GCD - greatest common divisor */
#define __GCD(NM, TYPE) TYPE NM(TYPE a, TYPE b);
__GCD(chgcd,char)
__GCD(scgcd,signed char)
__GCD(ucgcd,unsigned char)
__GCD(hgcd,short)
__GCD(uhgcd,unsigned short)
__GCD(igcd,int)
__GCD(ugcd,unsigned int)
__GCD(lgcd,long)
__GCD(ulgcd,unsigned long)
#undef __GCD

/* integer sqrt */
#define __ISQRT(NM, TYPE) TYPE NM(TYPE a);
__ISQRT(chsqrt,char)
__ISQRT(scsqrt,signed char)
__ISQRT(ucsqrt,unsigned char)
__ISQRT(hsqrt,short)
__ISQRT(uhsqrt,unsigned short)
__ISQRT(isqrt,int)
__ISQRT(usqrt,unsigned int)
__ISQRT(lsqrt,long)
__ISQRT(ulsqrt,unsigned long)
#undef __ISQRT

/* urand declarations */
INTEGER irand(void);
PRECISION urand(void);
void setseed(INTEGER ity);
INTEGER getseed(void);
INTEGER getirand(void);
PRECISION geturand(void);

extern INTEGER IRAND_MAX;

/* metropolis declarations */
/* user defined precision - those who don't want the default double
 * may need to define a exp( ) replacement.
 *
 * The following will use the <math.h> double exp( ) and create a cast
 * to (double) for the argument, if the "mpls_exp" macro is not defined.
 */
#  ifndef mpls_exp
#    define mpls_exp(a)	exp((double) a)
#  endif

#  define Mpls_RAN_INDEX	1
#  define Mpls_RAN_SWAP		2
#  define Mpls_RAN_PERMUTE	4
#  define Mpls_RAN_REVERSE	8
#  define Mpls_FIX_INITIAL	16
#  define Mpls_FIX_FINAL	32

typedef enum  {_NONE, _SWAP, _PERM, _REVR} swaptype;

int metropolis(int flag, int maxstep, int indexdim, int *index,
		PRECISION (*cost)(int,int*,swaptype,int,int),
		PRECISION (*temp)(int,int),
		PRECISION (*ranfn)(void));
int ranindex(int low, int high, int indexdim, int *index,
		PRECISION (*ranfn)(void));
void ranswap(int low, int high, int *swap1, int *swap2,
		PRECISION (*ranfn)(void));
void ranperm(int low, int high, int *cut, PRECISION (*ranfn)(void));
void idxswap(int *index, int swap1, int swap2);
void idxperm(int low, int high, int *index, int *work, int cut);
void idxrevr(int *index, int cut1, int cut2);

/* ANSI_SEQ definitions */

/* define this if there is no isatty() call available */
#  if 0
#    define NO_ISATTY
#  endif


#  define ATTRIB_NUM	6
typedef enum {
   NONE,   BOLD,   UNDERSCORE,   BLINK,   REVERSE,   CONCEALED}
	ansi_attributes;

#  define COLOR_NUM	8
typedef enum {
   BLACK,   RED,   GREEN,   YELLOW,   BLUE,   MAGENTA,   CYAN,   WHITE}
	ansi_colors;

extern const char *ANSI_Colors[COLOR_NUM];
extern const char *ANSI_Attributes[ATTRIB_NUM];

#  define ANSISEQ_LEN	16
#  define ANSISEQ_NUM	32

int set_ansi_tty(int ans);
const char *ansi_seq(ansi_attributes a, ansi_colors b, ansi_colors f);
const char *ansi_color(ansi_colors b, ansi_colors f);
const char *ansi_fgcolor(ansi_colors f);
const char *ansi_bgcolor(ansi_colors b);
const char *ansi_attribute(ansi_attributes a);

/* Unix Vector package */

typedef struct {
	char tag[5];			/* name tag for this type */
	char **vector;
	int number;			/* current number of list */
	int capacity;			/* the possible capacity of vector */
} uvec;

enum uvec_order {UVEC_ASCEND, UVEC_DESCEND
#ifndef NO_STRCASECMP
	, UVEC_CASE_ASCEND, UVEC_CASE_DESCEND
#endif
	};

uvec *uvec_ctor(int cap);
int uvec_dtor(uvec **uv);
int uvec_init(uvec *uv, int cap);
int uvec_close(uvec *uv);
int uvec_exists(uvec const *uv);
int uvec_capacity(uvec const *uv);
int uvec_number(uvec const *uv);
int uvec_end(uvec const *uv);
char ** uvec_vector(uvec const *uv);
int uvec_insert(uvec *uv, char const *str, int place);
int uvec_delete(uvec *uv, int place);
int uvec_add(uvec *uv, char const *str);
int uvec_addl(uvec *uv, ...);
int uvec_pop(uvec *uv);
int uvec_copy_vec(uvec *u, char **vec, int number);
int uvec_copy(uvec *u, uvec const *v);
int uvec_sort(uvec *uv, enum uvec_order type);
int uvec_find(uvec *uv, char const *str, enum uvec_order type);
int uvec_uniq(uvec *uv, enum uvec_order type);
int uvec_reverse(uvec const *uv);
int uvec_randomize(uvec const *uv, int seed);

/* generic list ``object'' */

/* warning sizeof(list) does not necessarily give the correct memory size
 * since we make it "expandable" to handle a variable size tag
 */
typedef struct list list;		/* forward declaration */
typedef struct list_elem list_elem;

struct list {
	list_elem *first;		/* head of list */
	list_elem *last;		/* last in list */
	int number;			/* number in list */
	int (*addfn)(void **, va_list);	/* user fn to add user data */
	int (*delfn)(void **, va_list);	/* user fn to del user data */
	char tag[1];			/* name tag for list */
/* followed by expanded memory allocation to contain rest of tag with
 * terminating NULL.  A list object must only be defined as "list *"
 * and set = to list_ctor(TAG,INIT,ADDFN,DELFN);
 */
};

struct list_elem {
	list_elem *prev;		/* previous one in list */
	list_elem *next;		/* next one in list */
	void *object;			/* pointer to object of interest */
};

/* shield users from  certain internal details */
#define LIST_OBJECT(le)	((le)->object)
#define LIST_NEXT(le)	((le)->next)
#define LIST_PREV(le)	((le)->prev)

list *list_ctor(const char *tag,
	int (addfn)(void **, va_list), int (delfn)(void **, va_list));
int list_dtor(list **lst, char const *tag, ...);
int list_exists(list const *lst, const char *tag);
int list_sizeof(list const *lst, const char *tag);
int list_number(list const *lst, const char *tag);
list_elem *list_first(list const *lst, const char *tag);
list_elem *list_last(list const *lst, const char *tag);
int list_add(list *lst, char const *tag, ...);
int list_del(list *lst, char const *tag, ...);
int list_push(list *lst, char const *tag, ...);
int list_pop(list *lst, char const *tag, ...);

/* librko - error message package */

extern int rkoerrno;

#define	RKO_OK		0
#define	RKOGENERR	1
#define	RKOSIGERR	2
#define	RKOSIGNAL	3
#define	RKOMEMERR	4
#define	RKOIOERR	5
#define	RKOUSEERR	6

void rkoperror(const char *s);
char *rkostrerror(void);
char *rkocpyerror(const char *s);
char *rkocaterror(const char *s);
char *rkopsterror(const char *s);

/* memory - provides a front-end for the memory allocation routines to
 *	help find memory leaks.  Supports only the ANSI-C routines:
 *		  calloc,   free,   malloc,   realloc
 *	call with the following names:
 *		m_calloc, m_free, m_malloc, m_realloc
 *	the output goes to stderr.
 */

#  ifdef MEMDEBUG
#    ifndef _MEMORY_H_
#      define _MEMORY_H_


void *m_calloc(size_t nelem, size_t size, char *file, int line);
void m_free(void *ptr, char *file, int line);
void *m_malloc(size_t size, char *file, int line);
void *m_realloc(void *rptr, size_t size, char *file, int line);

#      define   calloc(a,b)	m_calloc((a),(b),	__FILE__, __LINE__)
#      define     free(a)	m_free((a),		__FILE__, __LINE__)
#      define   malloc(a)	m_malloc((a),		__FILE__, __LINE__)
#      define  realloc(a,b)	m_realloc((a),(b),	__FILE__, __LINE__)

#    endif /* _MEMORY_H_ */
#  endif /* MEMDEBUG */
#  ifdef __cplusplus
	}
#  endif
#endif /* _LIBRKO_H_ */
