###########################################################################
##
## File:        ./Makefile.am
## Versions:    $Id: Makefile.am,v 1.5 2002/06/27 22:07:46 rk Exp $
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

ACLOCAL_AMFLAGS=-I $(srcdir)/config

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
	list/list_ctor.c list/list_dtor.c list/list_exists.c		\
	list/list_first.c list/list_last.c list/list_number.c		\
	list/list_pop.c list/list_push.c list/list_shift.c		\
	list/list_sizeof.c list/list_unshift.c

numfns = \
	numfns/divdiff.c numfns/foptim.c numfns/fzeroin.c

strfns = \
	strfns/istext.c strfns/macrosub.c strfns/nameread.c		\
	strfns/strchop.c strfns/strdbecpy.c strfns/strmalloc.c		\
	strfns/strmem.c strfns/timedfgets.c strfns/wcstrcmp.c

uvec_srcs =  \
	uvec/str2uvec.c uvec/uvec2str.c uvec/uvec2uvec.c		\
	uvec/uvec_accessor.c uvec/uvec_add.c uvec/uvec_alloc.c		\
	uvec/uvec_close.c uvec/uvec_copy.c uvec/uvec_copy_str.c		\
	uvec/uvec_copy_vec.c uvec/uvec_count_tok.c uvec/uvec_ctor.c	\
	uvec/uvec_dealloc.c uvec/uvec_dtor.c uvec/uvec_init.c		\
	uvec/uvec_insert.c uvec/uvec_push.c uvec/uvec_randomize.c	\
	uvec/uvec_reverse.c uvec/uvec_shift.c uvec/uvec_sort.c		\
	uvec/uvec_strfns.c uvec/vec2uvec.c

lib_LIBRARIES		= librko.a

librko_a_SOURCES	= \
	ansi_seq.c \
	$(avec_srcs) \
	clocker.c \
	coexec.c \
	dirtree.c \
	gethostbyX.c \
	$(igcd_srcs) \
	$(iprime_srcs) \
	$(isqrt_srcs) \
	$(list_srcs) \
	memdbg.c \
	memdebug.c \
	$(numfns) \
	rkoerror.c \
	$(strfns) \
	tcp_connect.c \
	$(uvec_srcs) \
	version.c

man3_MANS =  \
	docs/ansi_seq.man docs/avec.man docs/clocker.man \
	docs/coexec.man docs/dirtree.man docs/divdiff.man \
	docs/filter.man docs/foptim.man docs/fzeroin.man \
	docs/gcd.man docs/gethostbyX.man docs/invoke.man \
	docs/iprime.man docs/isqrt.man docs/istext.man \
	docs/Librko.man docs/list.man docs/macrosub.man \
	docs/memdbg.man docs/memdebug.man docs/nameread.man \
	docs/rkoerror.man docs/spawn.man docs/strchop.man \
	docs/strdbecpy.man docs/strmalloc.man docs/strmem.man \
	docs/tcp_connect.man docs/timedfgets.man docs/uvec.man \
	docs/wcstrcmp.man

