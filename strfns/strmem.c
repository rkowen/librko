static char USMID[]="%W%";
static const char RCSID[]="@(#)$Id: strmem.c,v 1.1 1998/10/07 16:19:56 rk Exp $";
static char AUTHOR[]="@(#)strmem 1.1 08/09/95 R.K.Owen,PhD";

/* strmem - allocates/deallocates collected string pool memory.  Instead of
 *	allocating each individual string ... strmem does a single allocation
 *	to a pool of memory and allocates an array of pointers to within
 *	this pool.
 *
 * prototype:
int strmemalloc(char **ChArSpAcE, char ***charspace,
	size_t numstr, size_t maxstrlen);
 *
 * allocates a pool of memory for character strings which are assumed to be
 * of uniform length.  The number of strings is given by numstr and the
 * maximum length by maxstrlen.  A second array is defined which points
 * to within this pool. maxstrlen is assumed to be consistent with the
 * output given by strlen() (i.e. the '\0' is not counted).  Therefore,
 * strmemalloc() will allocate numstr*(maxstrlen + 1) bytes.
 *
 * prototype:
void strmemfree(char **ChArSpAcE, char ***charspace);
 *
 * deallocates the memory allocated by strmemalloc(),
 *
 *	ChArSpAcE	pointer that receives a char memory pool allocation
 *	charspace	convenience array of ptrs to within the memory pool
 *	numstr		number of maxstrlen length strings
 *	maxstrlen	maximum string length
 *	/aborts on failure/
 *
 * author	R.K.Owen,Ph.D.	08/09/95
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
 * last known email: rk@owen.sj.ca.us
 *                   rkowen@kudonet.com
 *
 *********************************************************************
 */

#if 0
#  define TEST
#endif

#include <stdio.h>
#include <stdlib.h>
#ifdef RKOERROR
#  include "librko.h"
#endif

/* do 2 allocates, the 1st one is a collective pool of space ... the
 * second one is for an array of pointers into this collective pool
 * which is for convenience.
 */
int strmemalloc(char **ChArSpAcE, char ***charspace,
	size_t numstr, size_t maxstrlen) {

	char *ptr;
#ifdef RKOERROR
	char errmsg[100];
#endif
	size_t t = numstr*(++maxstrlen);
	int i;

	if (t <= 0) {
#ifdef RKOERROR
		(void) sprintf(errmsg,
			"strmemalloc : this should not happen ... "
			"trying to get %d<=0 space!\n",t);
		(void) rkocpyerror(errmsg);
		rkoerrno = RKOGENERR;
#endif
		return -1;
	}
	if ((*ChArSpAcE = (char *) malloc(t)) == (char *)NULL) {
#ifdef RKOERROR
		(void) sprintf(errmsg,
			"strmemalloc : could not allocate %d space!\n",t);
		(void) rkocpyerror(errmsg);
	rkoerrno = RKOMEMERR;
#endif
		return -1;
	}
	ptr = *ChArSpAcE;
	for (i = 0; i < t; ++i) {		/* zero space */
		*ptr++ = '\0';
	}
	if ((*charspace = (char **) malloc((numstr+1)*sizeof(char*)))
	== (char **)NULL) {
#ifdef RKOERROR
		(void) sprintf(errmsg,
	"strmemalloc : could not allocate auxillary %d space!\n",numstr+1);
		(void) rkocpyerror(errmsg);
	rkoerrno = RKOMEMERR;
#endif
		return -1;
	}
	ptr = *ChArSpAcE;
	for (i = 0; i < numstr; ++i) {	/* set convenience ptrs */
		(*charspace)[i] = ptr;
#if DEBUG > 1
		printf(": %p %p\n", &((*ChArSpAcE)[i*maxstrlen]),
			(*charspace)[i]);
#endif
		ptr += maxstrlen;
	}
	(*charspace)[numstr] = (char *) NULL;
#ifdef RKOERROR
	rkoerrno = RKO_OK;
#endif
	return 0;
}

void strmemfree(char **ChArSpAcE, char ***charspace) {

	free(*charspace);
	free(*ChArSpAcE);
	*charspace = (char **)NULL;
	*ChArSpAcE = (char *)NULL;
}

#if defined(TEST)

#include <strings.h>

int main() {
	char *name[] = {
		"aaa",
		"BbBB",
		"cCcCCC",
		"ddd"};
	char *NaMeSpAcE, **namespace;
	int i;
	size_t t, numentry, max_len = 0;

	numentry = sizeof(name)/sizeof(name[0]);

	for (i=0; i < numentry; ++i) {
		t = strlen(name[i]);
		max_len = (t < max_len ? max_len : t);
	}

	if (strmemalloc(&NaMeSpAcE, &namespace, numentry, max_len)) {
#ifdef RKOERROR
		rkoperror("TEST");
#else
		printf("FAIL - TEST error\n");
#endif
	}

	for (i=0; i < numentry; ++i) {
		strcpy(namespace[i],name[i]);
	}

	for (i=0; i < numentry; ++i) {
		printf("%s.\n", namespace[i]);
	}
	i=0;
	while (namespace[i] != (char *) NULL) {
		printf("--%s.\n", namespace[i++]);
	}

	strmemfree(&NaMeSpAcE, &namespace);
	return EXIT_SUCCESS;
}
#endif /* TEST */
