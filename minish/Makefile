#
# Makefile for minish
#
include ../Makefile.inc
.SUFFIXES: .c .o

YYSED	=../etc/yy-sed
#
#MSHFLAGS=-I.. -DTEST
MSHFLAGS=-I..
OBJS	=minish.o minish-yacc.o mshexec.o fdlist.o
#requires GNU "make"
LIBOBJS	=$(OBJS:%.o=../$(LIB)(%.o))

.DEFAULT:
	$(MAKE) help

all: ../$(LIB)

../$(LIB) : $(LIBOBJS)
	$(AR) $(ARFLAGS) $@ $?
#	-$(RM) -f $?

tall: $(OBJS)

TESTS	= shtest exectest yacctest lextest

help:
	@$(ECHO) "make		- makes all the minish object files"
	@$(ECHO) "make all	- same as above"
	@$(ECHO) "make test	- make & run minish test"
	@$(ECHO) "make runtest	- make & run all tests"
	@$(ECHO) "make clean	- clean out .o files"
	@$(ECHO) "make wipe	- do clean and remove tests"
	@$(ECHO) "make clobber	- do wipe and lex/yacc intermediate files"
	@$(ECHO) "make distclean	- don't do this"

test : runshtest

.c.o :
	$(CC) -c $(MSHFLAGS) $(CFLAGS) $<

shtest : minish.o minish-yacc.o mshexec.o fdlist.o minish-tab.h
	$(CC) $(MSHFLAGS) $(CFLAGS) -o shtest tminish.c \
		minish.o minish-yacc.o mshexec.o fdlist.o \
		-ll -L.. -lrko

exectest : mshexec.c fdlist.o
	$(CC) $(MSHFLAGS) $(CFLAGS) -o exectest -DEXECTEST mshexec.c fdlist.o \
		-ll -L.. -lrko
	-$(RM) mshexec.o

#yacctest : y.tab.c lex.yy.c fdlist.o
yacctest : minish-yacc.c minish-lex.c fdlist.o
	$(CC) $(MSHFLAGS) $(CFLAGS) -o yacctest -DYACCTEST minish-yacc.c fdlist.o \
		-ll -L.. -lrko
	-$(RM) minish-yacc.o

minish-yacc.o : minish-yacc.c minish-lex.c minish-tab.h
	$(CC) -c $(MSHFLAGS) $(CFLAGS) -DFLEX_STRING $<

minish-yacc.c : y.tab.c
	$(YYSED) --prefix minish_yy < $< > minish-yacc.c

minish-lex.c : lex.yy.c
	$(YYSED) --prefix minish_yy < $< > minish-lex.c

minish-tab.h : y.tab.h
	$(CP) $< minish-tab.h

y.tab.c y.tab.h : minish.yacc
	$(YACC) $(YYFLAGS) minish.yacc

#lextest : lex.yy.c y.tab.h
lextest : minish-lex.c minish-tab.h
	$(CC) $(MSHFLAGS) $(CFLAGS) -o lextest -DLEXTEST minish-lex.c
	-$(RM) minish-lex.o

lex.yy.c : minish.lex
	$(LEX) $(LEXFLAGS) minish.lex

fdtest/fdtest : fdlist.c
	$(CC) $(MSHFLAGS) $(CFLAGS) -o fdtest/fdtest -DFDTEST fdlist.c -L.. -lrko

mtest : mtest/right mtest/wrong mtest/rsignal

mtest/right : mtest/right.c
	$(CC) $(MSHFLAGS) $(CFLAGS) -o mtest/right $<

mtest/wrong : mtest/wrong.c
	$(CC) $(MSHFLAGS) $(CFLAGS) -o mtest/wrong $<

mtest/rsignal : mtest/rsignal.c
	$(CC) $(MSHFLAGS) $(CFLAGS) -o mtest/rsignal $<

runtest : runfdtest runlextest runyacctest runexectest runshtest

runshtest : shtest mtest
	-@./shtest
	-@if diff shtest.out cmp/shtest.cmp ; then \
		echo shtest OK ; else echo shtest FAILED ; fi

runexectest : exectest mtest
	-@./exectest
	-@if diff exectest.out cmp/exectest.cmp ; then \
		echo exectest OK ; else echo exectest FAILED ; fi

runyacctest : yacctest
	-@./yacctest > yacctest.out
	-@if diff yacctest.out cmp/yacctest.cmp ; then \
		echo yacctest OK ; else echo yacctest FAILED ; fi

runlextest : lextest
	-@./lextest > lextest.out
	-@if diff lextest.out cmp/lextest.cmp ; then \
		echo lextest OK ; else echo lextest FAILED ; fi

runfdtest : fdtest/fdtest
	-@(cd fdtest; ./fdtest.sh)

clean :
	-$(RM) -f *.o a.out core y.output
	-$(RM) -f fdtest/fdtest fdtest/fdtest.*out
	-$(RM) -f mtest/right mtest/wrong mtest/rsignal mtest/core
	-$(RM) -f lextest.out yacctest.out exectest.out shtest.out

wipe : clean
	-$(RM) $(TESTS)

clobber: wipe

distclean: clobber
	-$(RM) -f lex.yy.c y.tab.c
	-$(RM) -f minish-lex.c minish-yacc.c minish-tab.h y.tab.h
