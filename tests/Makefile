#
# Makefile to create and run all the librko tests
#
SRCS = \
	tansi_seq.c tfoptim.c tfzeroin.c tmacrosub.c \
	tmemory.c tnameread.c tstrchop.c tstrmem.c \
	ttfgets.c twcstrcmp.c tuvec.c tstrdbecpy.c tlist.c \
	tdirtree.c tinvoke.c tspawn.c tgcd.c tisqrt.c tiprime.c \
	tstrmalloc.c

XEXES = birdy kitty doggy

# requires a GNU "make" for the following
OBJS = $(SRCS:.c=.o)
EXES = $(SRCS:.c=.x)

include ../Makefile.inc

.SUFFIXES:
.SUFFIXES: .o .c .x

.DEFAULT:
	$(MAKE) help

help : 
	@echo "Targets:"
	@echo "	all	- create all the test executables"
	@echo "	runall	- run all the tests"
	@echo "	test	- both of above"
	@echo "	help	- this info"
	@echo "	clean	- remove .o files"
	@echo "	clobber	- do clean and remove all executables"
	
all : $(EXES) $(XEXES) tdir

$(EXES) : ../librko.a

runall : $(EXE)
	-for f in $(EXES) ; do echo ===$$f===; ./$$f ; done 2>&1 | more

test : all runall

.c.o :
	$(CC) -DRKOERROR $(CFLAGS) -c $< -I..

.o.x :
	$(CC) $(CFLAGS) -o $@ $< -L.. -lrko -lm

birdy : birdy.c
	$(CC) -DRKOERROR $(CFLAGS) -o birdy $<

kitty : kitty.c
	$(CC) -DRKOERROR $(CFLAGS) -o kitty $<

doggy : doggy.c
	$(CC) -DRKOERROR $(CFLAGS) -o doggy $<

tdir :
	tar -xf tdir.tar

clean :
	-$(RM) *.o a.out core

clobber : clean
	-$(RM) *.x
	-$(RM) -rf tdir
