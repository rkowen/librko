###########################################################################
##
## File:        ./Makefile.am
## Versions:    $Id: Makefile.am,v 1.15 2002/10/21 19:46:44 rk Exp $
## Created:     2002/06/14
##
###########################################################################
##
## run  automake to generate a Makefile.in
##	Makefile.am \
##	configure.ac >  Makefile.in
##	aclocal.m4  /
##

AUTOMAKE_OPTIONS=foreign subdir-objects no-installinfo

ACLOCAL_AMFLAGS=-I config

SUBDIRS = . tests
AM_CPPFLAGS = -I$(srcdir)/include -I$(srcdir)

avec_srcs = \
	avec/avec_.c							\
	avec/avec_accessor.c avec/avec_alloc.c avec/avec_close.c	\
	avec/avec_ctor.c avec/avec_dealloc.c avec/avec_decrease.c	\
	avec/avec_delete.c avec/avec_dtor.c avec/avec_increase.c	\
	avec/avec_init.c avec/avec_insert.c avec/avec_keys.c		\
	avec/avec_lookup.c avec/avec_remove.c avec/avec_resize.c	\
	avec/avec_resize_percentage.c avec/avec_set_fns.c		\
	avec/avec_values.c avec/avec_walk.c avec/avec_walk_r.c

coexec_srcs = \
	coexec/invoke.c coexec/postfilter.c coexec/postprocess.c	\
	coexec/prefilter.c coexec/preprocess.c coexec/spawn.c		\
	coexec/spawn3.c

igcd_srcs = \
	igcd/chgcd.c igcd/hgcd.c igcd/igcd.c		\
	igcd/lgcd.c igcd/scgcd.c igcd/ucgcd.c		\
	igcd/ugcd.c igcd/uhgcd.c igcd/ulgcd.c

iprime_srcs = \
	iprime/iprime_lut.c					\
	iprime/chprime.c iprime/hprime.c iprime/iprime.c	\
	iprime/lprime.c iprime/scprime.c iprime/ucprime.c	\
	iprime/uhprime.c iprime/uiprime.c iprime/ulprime.c

isqrt_srcs = \
	isqrt/isqrt_lut.c				\
	isqrt/chsqrt.c isqrt/hsqrt.c isqrt/isqrt.c	\
	isqrt/lsqrt.c isqrt/scsqrt.c isqrt/ucsrt.c	\
	isqrt/uhsqrt.c isqrt/uisqrt.c isqrt/ulsqrt.c

list_srcs = \
	list/list_.c list/list_append.c list/list_insert.c		\
	list/list_delete.c list/list_swap.c list/list_qsort.c		\
	list/list_ctor.c list/list_dtor.c list/list_exists.c		\
	list/list_first.c list/list_last.c list/list_number.c		\
	list/list_pop.c list/list_push.c list/list_shift.c		\
	list/list_sizeof.c list/list_unshift.c

netfns = \
	netfns/gethostbyX.c netfns/tcp_connect.c

numfns = \
	numfns/divdiff.c numfns/foptim.c numfns/fzeroin.c

strfns = \
	strfns/istext.c strfns/macrosub.c strfns/nameread.c		\
	strfns/strchop.c strfns/strdbecpy.c strfns/strmalloc.c		\
	strfns/strmem.c strfns/timedfgets.c strfns/wcstrcmp.c

urand_srcs = \
	urand/urand.c

uvec_srcs =  \
	uvec/str2uvec.c uvec/uvec2str.c uvec/uvec2uvec.c		\
	uvec/uvec_accessor.c uvec/uvec_add.c uvec/uvec_alloc.c		\
	uvec/uvec_close.c uvec/uvec_copy.c uvec/uvec_copy_str.c		\
	uvec/uvec_copy_vec.c uvec/uvec_count_tok.c uvec/uvec_ctor.c	\
	uvec/uvec_dealloc.c uvec/uvec_dtor.c uvec/uvec_init.c		\
	uvec/uvec_insert.c uvec/uvec_push.c uvec/uvec_randomize.c	\
	uvec/uvec_reverse.c uvec/uvec_shift.c uvec/uvec_sort.c		\
	uvec/uvec_strfns.c uvec/uvec_strfree.c uvec/vec2uvec.c

include_HEADERS =		\
	include/ansi_seq.h	\
	include/avec.h		\
	include/clocker.h	\
	include/coexec.h	\
	include/igcd.h		\
	include/iprime.h	\
	include/isqrt.h		\
	include/istext.h	\
	include/librko.h	\
	include/list.h		\
	include/memdebug.h	\
	include/numfns.h	\
	include/rkoerror.h	\
	include/strchop.h	\
	include/strfns.h	\
	include/strmalloc.h	\
	include/urand.h		\
	include/uvec.h

lib_LIBRARIES		= librko.a

librko_a_SOURCES	=	\
	ansi_seq.c 		\
	$(avec_srcs) 		\
	clocker.c 		\
	$(coexec_srcs) 		\
	dirtree.c 		\
	$(igcd_srcs) 		\
	$(iprime_srcs) 		\
	$(isqrt_srcs) 		\
	$(list_srcs) 		\
	memdbg.c 		\
	memdebug.c 		\
	$(netfns) 		\
	$(numfns) 		\
	rkoerror.c 		\
	$(strfns) 		\
	$(urand_srcs) 		\
	$(uvec_srcs) 		\
	version.c.in

man3_MANS =  \
	docs/ansi_seq.man docs/avec.man docs/clocker.man 		\
	docs/coexec.man docs/dirtree.man docs/divdiff.man 		\
	docs/filter.man docs/foptim.man docs/fzeroin.man 		\
	docs/gcd.man docs/gethostbyX.man docs/invoke.man 		\
	docs/iprime.man docs/isqrt.man docs/istext.man 			\
	docs/Librko.man docs/list.man docs/macrosub.man 		\
	docs/memdbg.man docs/memdebug.man docs/nameread.man 		\
	docs/rkoerror.man docs/spawn.man docs/strchop.man 		\
	docs/strdbecpy.man docs/strmalloc.man docs/strmem.man 		\
	docs/tcp_connect.man docs/timedfgets.man docs/uvec.man 		\
	docs/urand.man docs/wcstrcmp.man

special_includes=	\
	avec/avec_.h igcd/igcd_.h iprime/iprime_.h isqrt/isqrt_.h	\
	list/list_.h

EXTRA_DIST=				\
		$(man3_MANS)		\
		include			\
		$(special_includes)	\
		urand			\
		config			\
		tests			\
		docs

dist-hook:
	rm -rf `find $(distdir) -name CVS -print`
	rm -rf `find $(distdir) -name Makefile -print`
	rm -rf `find $(distdir) -name .deps -print`

urand/urand.o : urand/urand.c urand/urand_.c clocker.o
	cd urand; ./all.csh $(CC)

help:
	@echo ""
	@echo "make		- compiles sources to library"
	@echo "make all		- same as above"
	@echo "make install	- copy library & man pages to destination"
	@echo "make clean	- cleans out most useless files"
	@echo "make dist-gzip	- makes a .tar.gz tarball"
	@echo "make distclean	- cleans & removes most made files"
	@echo ""
