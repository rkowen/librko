#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif
#ifdef RKOERROR
#  include "rkoerror.h"
#else
#define rkoperror(a)
#endif
#include "list.h"
#ifdef MEMDEBUG
#  include "memdebug.h"
#endif

typedef struct mix mix;
struct mix {
	int ival;
	double fval;
};

/*----- handle a single element of "int" data -----*/
int addint(void **ptr, va_list ap) {
	int *iptr;
	if ((iptr = malloc(sizeof(int))) == (int *) NULL)
		return -1;
	*iptr = va_arg(ap, int);
	*ptr = iptr;
	return 0;
}

int delint(void **ptr, va_list ap) {
	free(*ptr);
	*ptr = (void *) NULL;
	return 0;
}

int cmpint(const void *e1, const void *e2) {
	/* printf ("<%d:%d>\t\t",**(int **) e1,**(int **) e2); */
	if (**(int **)e1 < **(int **)e2)
		return -1;
	else if (**(int **)e1 > **(int **)e2)
		return  1;
	else
		return 0;
}

/*----- handle a single element of "double" data -----*/
int addflt(void **ptr, va_list ap) {
	double *iptr;
	if ((iptr = malloc(sizeof(double))) == (double *) NULL)
		return -1;
	*iptr = va_arg(ap, double);
	*ptr = iptr;
	return 0;
}

int delflt(void **ptr, va_list ap) {
	free(*ptr);
	*ptr = (void *) NULL;
	return 0;
}

int cmpflt(const void *e1, const void *e2) {
	/* printf ("<%f:%f>\t\t",**(double **) e1,**(double **) e2); */
	if (**(double **)e1 < **(double **)e2)
		return -1;
	else if (**(double **)e1 > **(double **)e2)
		return  1;
	else
		return 0;
}

/*----- handle a single element of "struct" data -----*/
int addmix(void **ptr, va_list ap) {
	mix *iptr;
	if ((iptr = malloc(sizeof(mix))) == (mix *) NULL)
		return -1;
	iptr->ival = va_arg(ap, int);
	iptr->fval = va_arg(ap, double);
	*ptr = iptr;
	return 0;
}

int delmix(void **ptr, va_list ap) {
	free(*ptr);
	*ptr = (void *) NULL;
	return 0;
}

int cmpmix(const void *e1, const void *e2) {
	/* printf ("<%d:%d>\t\t",(*(mix **) e1)->ival,(*(mix **) e2)->ival); */
	if ((*(mix **)e1)->ival < (*(mix **)e2)->ival)
		return -1;
	else if ((*(mix **)e1)->ival > (*(mix **)e2)->ival)
		return  1;
	else
		return 0;
}

/*----- test, display & compare details -----*/
enum test_types {int_type, flt_type, mix_type};

int comptest(list *lst, const char *tag, enum test_types ttypes,
	char *expected, char *command) {

	char buffer[512];
	char ebuffer[64];
	list_elem *eptr;

	*buffer = '\0';
	sprintf(buffer,"=%d=", list_number(lst, tag));
	for (eptr = list_first(lst,tag);
	eptr != (list_elem *) NULL; eptr = eptr->next) {
		switch (ttypes) {
		case (int_type):
			sprintf(ebuffer,"%d:", *(int *)LIST_OBJECT(eptr));
			break;
		case (flt_type):
			sprintf(ebuffer,"%3.1f:", *(double *)LIST_OBJECT(eptr));
			break;
		case (mix_type):
			sprintf(ebuffer,"%d|%3.1f:",
				(((mix *)LIST_OBJECT(eptr))->ival),
				(((mix *)LIST_OBJECT(eptr))->fval));
			break;
		}
		strcat(buffer,ebuffer);
	}
	if (strcmp(buffer,expected)) {
		printf("    >%s\nFAIL:%s\n     %s <- expected\n",
			command, buffer, expected);
		return 1;
	} else {
		printf("    >%s\nOK  :%s\n", command, buffer);
		return 0;
	}
}

#  define _ZTEST(l, tg, tt, e, comm) \
	if (comm) rkoperror("test"); \
	count++;results+=comptest(l, tg, tt, e, #comm);

#  define _NTEST(l, tg, tt, e, comm) \
	if (!comm) rkoperror("test"); \
	count++;results+=comptest(l, tg, tt, e, #comm);

int main() {
	list *listobj = (list *) NULL;
	const char tagint[] = "INTS";
	const char tagflt[] = "FLOATS";
	const char tagmix[] = "MIXED";
	int results=0, count=0;

#ifdef MEMDEBUG
	memdebug_output(stdout);
#endif

/* add elements to int list */
	_ZTEST(listobj, tagint, int_type, "=-1=",
		list_exists(listobj, tagint))
	_NTEST(listobj, tagint, int_type, "=0=",
		(listobj = list_ctor(tagint, addint, delint)))
	_NTEST(listobj, tagint, int_type, "=0=",
		list_exists(listobj, tagint))
	_ZTEST(listobj, tagint, int_type, "=1=3:",
		list_push(listobj, tagint, 3))
	_ZTEST(listobj, tagint, int_type, "=2=3:2:",
		list_push(listobj, tagint, 2))
	_ZTEST(listobj, tagint, int_type, "=3=3:2:1:",
		list_push(listobj, tagint, 1))
	_ZTEST(listobj, tagint, int_type, "=4=2:3:2:1:",
		list_unshift(listobj, tagint, 2))
	_ZTEST(listobj, tagint, int_type, "=5=1:2:3:2:1:",
		list_unshift(listobj, tagint, 1))
	
	_ZTEST(listobj, tagint, int_type, "=4=1:2:3:2:",
		list_pop(listobj, tagint))
	_ZTEST(listobj, tagint, int_type, "=3=2:3:2:",
		list_shift(listobj, tagint))
	_ZTEST(listobj, tagint, int_type, "=2=2:3:",
		list_pop(listobj, tagint))
	_ZTEST(listobj, tagint, int_type, "=1=3:",
		list_shift(listobj, tagint))
	_ZTEST(listobj, tagint, int_type, "=0=",
		list_pop(listobj, tagint))
	_NTEST(listobj, tagint, int_type, "=0=",
		list_shift(listobj, tagint))
	_NTEST(listobj, tagint, int_type, "=0=",
		((list_sizeof(listobj, tagint) - sizeof(listobj)) == 4))

	_ZTEST(listobj, tagint, int_type, "=1=1:",
		list_unshift(listobj, tagint, 1))
	_ZTEST(listobj, tagint, int_type, "=0=",
		list_shift(listobj, tagint))
	_NTEST(listobj, tagint, int_type, "=0=",
		list_pop(listobj, tagint))
	
	_ZTEST(listobj, tagint, int_type, "=1=3:",
		list_push(listobj, tagint, 3))
	_ZTEST(listobj, tagint, int_type, "=2=3:2:",
		list_push(listobj, tagint, 2))
	_ZTEST(listobj, tagint, int_type, "=3=4:3:2:",
		list_unshift(listobj, tagint, 4))
	/*---*/
	_ZTEST(listobj, tagint, int_type, "=4=4:5:3:2:",
		list_append(listobj, tagint,list_first(listobj,tagint), 5))
	_ZTEST(listobj, tagint, int_type, "=5=4:5:3:6:2:",
		list_insert(listobj, tagint,list_last(listobj,tagint), 6))
	_ZTEST(listobj, tagint, int_type, "=5=2:3:4:5:6:",
		list_qsort(listobj, tagint,cmpint))
	_ZTEST(listobj, tagint, int_type, "=4=3:4:5:6:",
		list_delete(listobj, tagint,list_first(listobj,tagint)))
	_ZTEST(listobj, tagint, int_type, "=3=3:4:5:",
		list_delete(listobj, tagint,list_last(listobj,tagint)))
	_ZTEST(listobj, tagint, int_type, "=2=3:5:",
		list_delete(listobj, tagint,list_first(listobj,tagint)->next))
	_ZTEST(listobj, tagint, int_type, "=2=5:3:",
		list_swap(listobj, tagint,
		list_first(listobj,tagint), list_last(listobj,tagint)))
	/*---*/
	_ZTEST(listobj, tagint, int_type, "=-1=",
		list_dtor(&listobj, tagint))

/* add elements to flt list */
	_ZTEST(listobj, tagflt, flt_type, "=-1=",
		list_exists(listobj, tagflt))
	_NTEST(listobj, tagflt, flt_type, "=0=",
		(listobj = list_ctor(tagflt, addflt, delflt)))
	_NTEST(listobj, tagflt, flt_type, "=0=",
		list_exists(listobj, tagflt))
	_ZTEST(listobj, tagflt, flt_type, "=1=3.3:",
		list_push(listobj, tagflt, 3.3))
	_ZTEST(listobj, tagflt, flt_type, "=2=3.3:2.2:",
		list_push(listobj, tagflt, 2.2))
	_ZTEST(listobj, tagflt, flt_type, "=3=3.3:2.2:1.1:",
		list_push(listobj, tagflt, 1.1))
	_ZTEST(listobj, tagflt, flt_type, "=4=2.2:3.3:2.2:1.1:",
		list_unshift(listobj, tagflt, 2.2))
	_ZTEST(listobj, tagflt, flt_type, "=5=1.1:2.2:3.3:2.2:1.1:",
		list_unshift(listobj, tagflt, 1.1))
	
	_ZTEST(listobj, tagflt, flt_type, "=4=1.1:2.2:3.3:2.2:",
		list_pop(listobj, tagflt))
	_ZTEST(listobj, tagflt, flt_type, "=3=2.2:3.3:2.2:",
		list_shift(listobj, tagflt))
	_ZTEST(listobj, tagflt, flt_type, "=2=2.2:3.3:",
		list_pop(listobj, tagflt))
	_ZTEST(listobj, tagflt, flt_type, "=1=3.3:",
		list_shift(listobj, tagflt))
	_ZTEST(listobj, tagflt, flt_type, "=0=",
		list_pop(listobj, tagflt))
	_NTEST(listobj, tagflt, flt_type, "=0=",
		list_shift(listobj, tagflt))
	_NTEST(listobj, tagflt, flt_type, "=0=",
		((list_sizeof(listobj, tagflt) - sizeof(listobj)) == 6))

	_ZTEST(listobj, tagflt, flt_type, "=1=1.1:",
		list_unshift(listobj, tagflt, 1.1))
	_ZTEST(listobj, tagflt, flt_type, "=0=",
		list_shift(listobj, tagflt))
	_NTEST(listobj, tagflt, flt_type, "=0=",
		list_pop(listobj, tagflt))
	
	_ZTEST(listobj, tagflt, flt_type, "=1=3.3:",
		list_push(listobj, tagflt, 3.3))
	_ZTEST(listobj, tagflt, flt_type, "=2=3.3:2.2:",
		list_push(listobj, tagflt, 2.2))
	_ZTEST(listobj, tagflt, flt_type, "=3=4.4:3.3:2.2:",
		list_unshift(listobj, tagflt, 4.4))
	/*---*/
	_ZTEST(listobj, tagflt, flt_type, "=4=4.4:5.5:3.3:2.2:",
		list_append(listobj, tagflt,list_first(listobj,tagflt), 5.5))
	_ZTEST(listobj, tagflt, flt_type, "=5=4.4:5.5:3.3:6.6:2.2:",
		list_insert(listobj, tagflt,list_last(listobj,tagflt), 6.6))
	_ZTEST(listobj, tagflt, flt_type, "=5=2.2:3.3:4.4:5.5:6.6:",
		list_qsort(listobj, tagflt,cmpflt))
	_ZTEST(listobj, tagflt, flt_type, "=4=3.3:4.4:5.5:6.6:",
		list_delete(listobj, tagflt,list_first(listobj,tagflt)))
	_ZTEST(listobj, tagflt, flt_type, "=3=3.3:4.4:5.5:",
		list_delete(listobj, tagflt,list_last(listobj,tagflt)))
	_ZTEST(listobj, tagflt, flt_type, "=2=3.3:5.5:",
		list_delete(listobj, tagflt,list_first(listobj,tagflt)->next))
	_ZTEST(listobj, tagflt, flt_type, "=2=5.5:3.3:",
		list_swap(listobj, tagflt,
		list_first(listobj,tagflt), list_last(listobj,tagflt)))
	/*---*/
	_ZTEST(listobj, tagflt, flt_type, "=-1=",
		list_dtor(&listobj, tagflt))

/* add elements to mix list */
	_ZTEST(listobj, tagmix, mix_type, "=-1=",
		list_exists(listobj, tagmix))
	_NTEST(listobj, tagmix, mix_type, "=0=",
		(listobj = list_ctor(tagmix, addmix, delmix)))
	_NTEST(listobj, tagmix, mix_type, "=0=",
		list_exists(listobj, tagmix))
	_ZTEST(listobj, tagmix, mix_type, "=1=3|3.3:",
		list_push(listobj, tagmix, 3, 3.3))
	_ZTEST(listobj, tagmix, mix_type, "=2=3|3.3:2|2.2:",
		list_push(listobj, tagmix, 2, 2.2))
	_ZTEST(listobj, tagmix, mix_type, "=3=3|3.3:2|2.2:1|1.1:",
		list_push(listobj, tagmix, 1, 1.1))
	_ZTEST(listobj, tagmix, mix_type, "=4=2|2.2:3|3.3:2|2.2:1|1.1:",
		list_unshift(listobj, tagmix, 2, 2.2))
	_ZTEST(listobj, tagmix, mix_type, "=5=1|1.1:2|2.2:3|3.3:2|2.2:1|1.1:",
		list_unshift(listobj, tagmix, 1, 1.1))
	
	_ZTEST(listobj, tagmix, mix_type, "=4=1|1.1:2|2.2:3|3.3:2|2.2:",
		list_pop(listobj, tagmix))
	_ZTEST(listobj, tagmix, mix_type, "=3=2|2.2:3|3.3:2|2.2:",
		list_shift(listobj, tagmix))
	_ZTEST(listobj, tagmix, mix_type, "=2=2|2.2:3|3.3:",
		list_pop(listobj, tagmix))
	_ZTEST(listobj, tagmix, mix_type, "=1=3|3.3:",
		list_shift(listobj, tagmix))
	_ZTEST(listobj, tagmix, mix_type, "=0=",
		list_pop(listobj, tagmix))
	_NTEST(listobj, tagmix, mix_type, "=0=",
		list_shift(listobj, tagmix))
	_NTEST(listobj, tagmix, mix_type, "=0=",
		((list_sizeof(listobj, tagmix) - sizeof(listobj)) == 5))

	_ZTEST(listobj, tagmix, mix_type, "=1=1|1.1:",
		list_unshift(listobj, tagmix, 1, 1.1))
	_ZTEST(listobj, tagmix, mix_type, "=0=",
		list_shift(listobj, tagmix))
	_NTEST(listobj, tagmix, mix_type, "=0=",
		list_pop(listobj, tagmix))
	
	_ZTEST(listobj, tagmix, mix_type, "=1=3|3.3:",
		list_push(listobj, tagmix, 3, 3.3))
	_ZTEST(listobj, tagmix, mix_type, "=2=3|3.3:2|2.2:",
		list_push(listobj, tagmix, 2, 2.2))
	_ZTEST(listobj, tagmix, mix_type, "=3=4|4.4:3|3.3:2|2.2:",
		list_unshift(listobj, tagmix, 4, 4.4))
	/*---*/
	_ZTEST(listobj, tagmix, mix_type, "=4=4|4.4:5|5.5:3|3.3:2|2.2:",
		list_append(listobj, tagmix,list_first(listobj,tagmix), 5, 5.5))
	_ZTEST(listobj, tagmix, mix_type, "=5=4|4.4:5|5.5:3|3.3:6|6.6:2|2.2:",
		list_insert(listobj, tagmix,list_last(listobj,tagmix), 6, 6.6))
	_ZTEST(listobj, tagmix, mix_type, "=5=2|2.2:3|3.3:4|4.4:5|5.5:6|6.6:",
		list_qsort(listobj, tagmix,cmpmix))
	_ZTEST(listobj, tagmix, mix_type, "=4=3|3.3:4|4.4:5|5.5:6|6.6:",
		list_delete(listobj, tagmix,list_first(listobj,tagmix)))
	_ZTEST(listobj, tagmix, mix_type, "=3=3|3.3:4|4.4:5|5.5:",
		list_delete(listobj, tagmix,list_last(listobj,tagmix)))
	_ZTEST(listobj, tagmix, mix_type, "=2=3|3.3:5|5.5:",
		list_delete(listobj, tagmix,list_first(listobj,tagmix)->next))
	_ZTEST(listobj, tagmix, mix_type, "=2=5|5.5:3|3.3:",
		list_swap(listobj, tagmix,
		list_first(listobj,tagmix), list_last(listobj,tagmix)))
	/*---*/
	_ZTEST(listobj, tagmix, mix_type, "=-1=",
		list_dtor(&listobj, tagmix))

	if (results) {
		printf("There were %d failures in %d tests\n", results, count);
	} else {
		printf("There were no failures in %d tests\n", count);
	}
	return results;
}
