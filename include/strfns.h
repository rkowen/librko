#ifndef _STRFNS_H_
#  define _STRFNS_H_
/* 
 * RCSID @(#)$Id: strfns.h,v 1.3 2003/09/05 05:02:00 rk Exp $
 */
/** ** Copyright *********************************************************** **
 ** 									     **
 ** Copyright 2001 by R.K.Owen,Ph.D.		                      	     **
 ** last known email: librko@kooz.sj.ca.us				     **
 **                   rk@owen.sj.ca.us					     **
 ** see LICENSE.LGPL, which must be provided, for details		     **
 ** 									     **
 ** ************************************************************************ **/
#  ifdef __cplusplus
extern "C" {
#  endif


#include <stdlib.h>	/* size_t */
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#include "avec.h"

int	 istext(int c);

char	*strchop(const char *string, int chop_len,
        size_t *str_len, char **next);

/* old strmalloc interface */
char	*strnmalloc(	char const *in, size_t n);
char	*strmalloc(	char const *in);
int	 strfree(	char **str);

/* new strmalloc interface */
char	*str_nmalloc(	char const *in, size_t n);
char	*str_malloc(	char const *in);
int	 str_free(	char **str);
size_t	 str_sizeof(	char const *str);
char	*str_ncpy(	char **str, char *in, size_t n);
char	*str_cpy(	char **str, char *in);
char	*str_ncat(	char **str, char *in, size_t n);
char	*str_cat(	char **str, char *in);

int	 strmemalloc(char **ChArSpAcE, char ***charspace,
		size_t numstr, size_t maxstrlen);
void	 strmemfree(char **ChArSpAcE, char ***charspace);

int	 timedfgets(char *buf, int size, FILE *stream, int seconds);

int	 wcstrcmp(const char *str, const char *wcstr);

char	*macstrncpy(char *string, const char *mstring, size_t n);

extern char *MacroStr[];
extern int MacroTrf[];

#define NAMETOL(a,b)	nametol(a,#b,&b)
#define NAMETOD(a,b)	nametod(a,#b,&b)
int	 nametol(const char *line, const char *name, long *value);
int	 nametod(const char *line, const char *name, double *value);

avec *key_value(const char *string,
	const char *pair_delim, const char *ref_delim, int nowhitespace);

#  ifdef __cplusplus
	}
#  endif
#endif /* _STRFNS_H_ */
