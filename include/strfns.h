#ifndef _STRFNS_H_
#  define _STRFNS_H_
/* 
 * RCSID @(#)$Id: strfns.h,v 1.1 2002/06/30 04:07:13 rk Exp $
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
#include "config.h"

int	 istext(int c);

char	*strchop(const char *string, int chop_len,
        size_t *str_len, char **next);

char	*strnmalloc(char const *in, size_t n);
char	*strmalloc(char const *in);
int	 strfree(char **str);

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

#  ifdef __cplusplus
	}
#  endif
#endif /* _STRFNS_H_ */
