static const char RCSID[]="@(#)$Id: avec_set_fns.c,v 1.4 2003/09/05 05:02:00 rk Exp $";
static const char AUTHOR[]="@(#)avec 1.0 2002/02/08 R.K.Owen,Ph.D.";
/* avec.c -
 * This could have easily been made a C++ class, but is
 * confined to C so that it can be maximally portable.
 *
 * This is an associative vector package (a hash array)
 * that uses quadratic hashing.
 */
/** ** Copyright *********************************************************** **
 ** 									     **
 ** Copyright 2002 by R.K.Owen,Ph.D.		                      	     **
 ** last known email: librko@kooz.sj.ca.us				     **
 **                   rk@owen.sj.ca.us					     **
 ** see LICENSE.LGPL, which must be provided, for details		     **
 ** 									     **
 ** ************************************************************************ **/

#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#ifdef MEMDEBUG
#  include "memdebug.h"
#endif
#include "avec.h"
#include "iprime.h"
#ifdef HAVE_STRMALLOC
#  include "strmalloc.h"
#endif
#ifdef RKOERROR
#  include "rkoerror.h"
#endif

/* ---------------------------------------------------------------------- */
/* Implement AVEC_COUNT
 * put the count into the (void *) ptr
 * but limited to the unsigned integer size that fits
 */
static int avec_count_insert (void **data, va_list ap) {
	int retval = 1;
	unsigned short us;
	unsigned int ui;
	unsigned long ul;
	if (!data) return -1;

	if (*data == (void *) NULL) retval = 0;

	if (sizeof(void *) == sizeof(unsigned int)) {
		ui = (unsigned int) *data;
		*data = (void *) ++ui;
	} else if (sizeof(void *) == sizeof(unsigned short)) {
		us = (unsigned short) *data;
		*data = (void *) ++us;
	} else if (sizeof(void *) == sizeof(unsigned long)) {
		ul = (unsigned long) *data;
		*data = (void *) ++ul;
	} else {	/* hope for the best */
		ui = (unsigned int) *data;
		*data = (void *) ++ui;
	}
	return retval;
}

static int avec_count_delete (void **data, va_list ap) {
	int retval = 1;
	unsigned short us;
	unsigned int ui;
	unsigned long ul;
	if (!data) return -1;

	if (*data == (void *) NULL) retval = 0;

	if (sizeof(void *) == sizeof(unsigned int)) {
		ui = (unsigned int) *data;
		*data = (void *) --ui;
	} else if (sizeof(void *) == sizeof(unsigned short)) {
		us = (unsigned short) *data;
		*data = (void *) --us;
	} else if (sizeof(void *) == sizeof(unsigned long)) {
		ul = (unsigned long) *data;
		*data = (void *) --ul;
	} else {	/* hope for the best */
		ui = (unsigned int) *data;
		*data = (void *) --ui;
	}

	if (*data == (void *) NULL) retval = 0;

	return retval;
}

static int avec_count_rm (void **data, va_list ap) {
	if (!data) return -1;
	*data = (void *) NULL;
	return 0;
}

static avec_fns count_fns = {
	AVEC_COUNT,
	avec_count_insert,
	avec_count_delete,
	avec_count_rm
};

/* ---------------------------------------------------------------------- */
/* wrappers for the StdC string functions
 */
static int stdc_malloc (void **data, va_list ap) {
	char const *str = va_arg(ap,char *);
	if (!data) return -1;
	if (*data) return 1;
	if((*data = calloc((strlen(str)+1),sizeof(char)))) {
		(void) strcpy((char *) *data, str);
		return 0;
	}
	return -2;
}
static int stdc_free (void **data, va_list ap) {
	free(*data);
	*data = (char *) NULL;
	return 0;
}

static avec_fns stdc_fns = {
	AVEC_STDC,
	stdc_malloc,
	stdc_free,
	stdc_free
};

/* ---------------------------------------------------------------------- */
#ifdef HAVE_STRMALLOC
/* ---------------------------------------------------------------------- */
/* wrappers for the strmalloc string functions
 */
static int str_malloc_ (void **data, va_list ap) {
	char const *str = va_arg(ap,char *);
	if (!data) return -1;
	if (*data) return 1;
	if((*data = (void *) strmalloc(str))) return 0;
	return -2;
}

static int str_free_ (void **data, va_list ap) {
	strfree((char **) data);
	return 0;
}

static avec_fns strmalloc_fns = {
	AVEC_STRMALLOC,
	str_malloc_,
	str_free_,
	str_free_
};

avec_fns default_fns = {
	AVEC_STRMALLOC,
	str_malloc_,
	str_free_,
	str_free_
};
#else
avec_fns default_fns = {
	AVEC_STDC,
	stdc_malloc,
	stdc_free,
	stdc_free
};
#endif
/* return the data functions to use
 */
static avec_fns *avec_set_fns_(enum avec_def_fns type, avec_fns *fns) {
	static avec_fns t_fns;
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	if (type == AVEC_DEFAULT) {
#ifdef HAVE_STRMALLOC
		type = AVEC_STRMALLOC;
#else
		type = AVEC_STDC;
#endif
	}
	if (type == AVEC_STDC) {
		t_fns.data_add = stdc_fns.data_add;
		t_fns.data_del = stdc_fns.data_del;
		t_fns.data_rm = stdc_fns.data_rm;
#ifdef HAVE_STRMALLOC
	} else if (type == AVEC_STRMALLOC) {
		t_fns.data_add = stdc_fns.data_add;
		t_fns.data_del = stdc_fns.data_del;
		t_fns.data_rm = stdc_fns.data_rm;
#endif
	} else if (type == AVEC_COUNT) {
		t_fns.data_add = count_fns.data_add;
		t_fns.data_del = count_fns.data_del;
		t_fns.data_rm = count_fns.data_rm;
	} else if (type == AVEC_USER) {
		if (fns == (avec_fns*) NULL
		|| fns->data_add == NULL
		|| fns->data_del == NULL
		|| fns->data_rm == NULL) {
#ifdef RKOERROR
			(void) rkocpyerror(
				"null data functions!");
			rkoerrno = RKOUSEERR;
#endif
			return (avec_fns *) NULL;
		}
		t_fns.data_add = fns->data_add;
		t_fns.data_del = fns->data_del;
		t_fns.data_rm = fns->data_rm;
	}
	t_fns.type = type;
	return &t_fns;
}

/* set the default set of data functions to use
 */
int avec_set_def_fns(enum avec_def_fns type, avec_fns *fns) {
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	avec_fns	*t_fns;

	if (!(t_fns = avec_set_fns_(type,fns))) {
#ifdef RKOERROR
		(void) rkopsterror("avec_set_def_fns :");
#endif
		return -1;
	}

	default_fns.data_add = t_fns->data_add;
	default_fns.data_del = t_fns->data_del;
	default_fns.data_rm = t_fns->data_rm;
	default_fns.type = t_fns->type;
	return 0;
}
/* return what type of data functions are currently default
 */
enum avec_def_fns avec_get_def_fns_enum(void) {
	return default_fns.type;
}

/* return the current default data functions struct
 */
avec_fns *avec_get_def_fns(void) {
	return &default_fns;
}

/* return the current data functions struct for this avec object
 */
int avec_set_fns(avec *av, enum avec_def_fns type, avec_fns *fns) {
	avec_fns *t_fns;
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	if (!avec_exists(av)) {
#ifdef RKOERROR
	rkoerrno = RKOUSEERR;
	(void) rkocpyerror("avec_set_fns : null avec!");
#endif
		return -1;
	}
	if (!(t_fns = avec_set_fns_(type,fns))) {
#ifdef RKOERROR
		(void) rkopsterror("avec_set_fns :");
#endif
		return -2;
	}

	(av->fns).data_add = t_fns->data_add;
	(av->fns).data_del = t_fns->data_del;
	(av->fns).data_rm = t_fns->data_rm;
	(av->fns).type = t_fns->type;
	return 0;
}
/* return the current data functions struct for this avec object
 */
avec_fns *avec_get_fns(avec *av) {
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	if (avec_exists(av)) {
		return &(av->fns);
	} else {
#ifdef RKOERROR
	rkoerrno = RKOUSEERR;
	(void) rkocpyerror("avec_get_fns : null avec!");
#endif
		return (avec_fns *) NULL;
	}
}

/* return a pointer to one of the internal standard avec fn set
 */
avec_fns *avec_get_std_fns(enum avec_def_fns type) {
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	if (type == AVEC_DEFAULT) {
		return &default_fns;
	} else if (type == AVEC_STDC) {
		return &stdc_fns;
#ifdef HAVE_STRMALLOC
	} else if (type == AVEC_STRMALLOC) {
		return &strmalloc_fns;
#endif
	} else if (type == AVEC_COUNT) {
		return &count_fns;
	} else if (type == AVEC_USER) {
#ifdef RKOERROR
		rkoerrno = RKOUSEERR;
		(void) rkocpyerror("avec_get_std_fns : AVEC_USER not standard");
#endif
		return (avec_fns *) NULL;
	}
}
