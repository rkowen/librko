###########################################################################
##
## File:        ./tests/Makefile.am
## Versions:    $Id: Makefile.am,v 1.2 2002/06/24 17:04:51 rk Exp $
## Created:     2002/06/24
##
###########################################################################
##
## run  automake to generate a Makefile.in
##	Makefile.am \
##	configure.ac >  Makefile.in
##	aclocal.m4  /
##

AUTOMAKE_OPTIONS=foreign no-installinfo no-installman

INCLUDES = -I$(top_srcdir)/include -I$(top_srcdir) -I$(srcdir)

helper_progs = doggy kitty birdy

test_progs = \
	tansi_seq.x tavec.x tclocker.x tdirtree.x tfilter.x	\
	tfoptim.x tfzeroin.x tgcd.x tinvoke.x tiprime.x		\
	tisqrt.x tlist.x tmacrosub.x tmemdbg.x tnameread.x	\
	tspawn.x tstrchop.x tstrdbecpy.x tstrmalloc.x tstrmem.x	\
	ttfgets.x tuvec.x twcstrcmp.x

noinst_PROGRAMS =	$(helper_progs)			\
			$(test_progs)

LDADD		= ../librko.a

# helper programs
doggy_SOURCES			= doggy.c
kitty_SOURCES			= kitty.c
birdy_SOURCES			= birdy.c

# test programs
tansi_seq_x_SOURCES		= tansi_seq.c
tavec_x_SOURCES			= tavec.c
tclocker_x_SOURCES		= tclocker.c
tdirtree_x_SOURCES		= tdirtree.c
tfilter_x_SOURCES		= tfilter.c
tfoptim_x_SOURCES		= tfoptim.c
tfzeroin_x_SOURCES		= tfzeroin.c
tgcd_x_SOURCES			= tgcd.c
tinvoke_x_SOURCES		= tinvoke.c
tiprime_x_SOURCES		= tiprime.c
tisqrt_x_SOURCES		= tisqrt.c
tlist_x_SOURCES			= tlist.c
tmacrosub_x_SOURCES		= tmacrosub.c
tmemdbg_x_SOURCES		= tmemdbg.c
tnameread_x_SOURCES		= tnameread.c
tspawn_x_SOURCES		= tspawn.c
tstrchop_x_SOURCES		= tstrchop.c
tstrdbecpy_x_SOURCES		= tstrdbecpy.c
tstrmalloc_x_SOURCES		= tstrmalloc.c
tstrmem_x_SOURCES		= tstrmem.c
ttfgets_x_SOURCES		= ttfgets.c
tuvec_x_SOURCES			= tuvec.c
twcstrcmp_x_SOURCES		= twcstrcmp.c

help : 
	@echo "Targets:"
	@echo "	all	- create all the test executables"
#	@echo "	mall	- create all the test memory debug executables"
	@echo "	runall	- run all the tests"
	@echo "	test	- both of above"
	@echo "	help	- this info"
	@echo "	clean	- remove .o files"
	@echo "	clobber	- do clean and remove all executables"
	
EXES = $(test_progs)

runall : $(EXES)
	-for f in $(EXES) ; do echo ===$$f===; ./$$f ; done 2>&1 | more

test : all runall

clean :
	-rm -f *.o *_m.o a.out core

clobber : clean
	-rm -f $(EXES)
