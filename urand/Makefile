#
# Makefile for minish
#
include ../Makefile.inc
.SUFFIXES: .c .o .man .3

#
OBJS	=urand.o
#requires GNU "make"
LIBOBJS	=$(OBJS:%.o=../$(LIB)(%.o))

.DEFAULT:
	$(MAKE) help

all: ../$(LIB) urand.3

../$(LIB) : $(LIBOBJS)
	$(AR) $(ARFLAGS) $@ $?
	-$(RM) -f $?

.c.o :
	./script.csh "$(CC) $(CFLAGS)"

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
