#
# Makefile for minish
#
include ../Makefile.inc

.SUFFIXES:
.SUFFIXES: .c .o .man .3

#
OBJS	=urand.o
#requires GNU "make"
LIBOBJS	=$(OBJS:%.o=../$(LIB)(%.o))
MLIBOBJS	=$(OBJS:%_m.o=../$(MLIB)(%_m.o))

.DEFAULT:
	$(MAKE) help

all: ../$(LIB)(urand.o) urand.3

mall: ../$(MLIB)(urand.o)

../$(LIB)(urand.o) : urand.o
	$(AR) $(ARFLAGS) $@ $?
#	-$(RM) -f $?

../$(MLIB)(urand.o) : urand.o
	$(AR) $(ARFLAGS) $@ $?
#	-$(RM) -f $?

.c.o :
	@$(ECHO) ">>>> .c.o $* $< $@"
	./script.csh "$(CC) $(CFLAGS)"

.c_m.o :
	@$(ECHO) ">>>> .c_m.o $* $< $@"
	./script.csh "$(CC) $(CFLAGS) -DMEMDEBUG"

.man.3 :
	$(NROFF) $(MAN) $< > $@

test :
	@$(ECHO) "urand - no tests"

help:
	@$(ECHO) "make		- makes the urand object file"
	@$(ECHO) "make all	- same as above"
#	@$(ECHO) "make runtest	- make & run all tests"
	@$(ECHO) "make clean	- clean out .o files"
	@$(ECHO) "make wipe	- do clean and remove tests"
	@$(ECHO) "make clobber	- do wipe and lex/yacc intermediate files"
	@$(ECHO) "make distclean	- don't do this"

clean :
	-$(RM) -f *.o a.out core

wipe : clean
	-$(RM) -f .opt testf testhf testi tmadd tmdiv tmsub

clobber: wipe

distclean: clobber
