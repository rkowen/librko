#
# Makefile to create and run all the librko tests
#
SRCS = \
	tansi_seq.c tfoptim.c tfzeroin.c tmacrosub.c \
	tmemory.c tnameread.c tstrchop.c tstrmem.c tstrtok.c \
	ttfgets.c twcstrcmp.c tuvec.c tstrdbecpy.c tlist.c \
	tdirtree.c

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
	@echo "	help	- this info"
	@echo "	clean	- remove .o files"
	@echo "	clobber	- do clean and remove all executables"
	
all : $(EXES)

runall : $(EXE)
	-for f in $(EXES) ; do echo ===$$f===; ./$$f ; done 2>&1 | more

.c.o :
	$(CC) -DRKOERROR $(CFLAGS) -c $< -I..

.o.x :
	$(CC) $(CFLAGS) -o $@ $< -L.. -lrko -lm

clean :
	-$(RM) *.o a.out core

clobber : clean
	-$(RM) *.x
