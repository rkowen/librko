#
# Makefile for minish
#
include ../Makefile.inc

.SUFFIXES:
.SUFFIXES: .c .o _m.o

YYSED	=../etc/yy-sed
MEMDBG	=../memdbg
#
#MSHFLAGS=-I.. -DTEST
MSHFLAGS=-I..
OBJS	=minish.o minish-yacc.o mshexec.o fdlist.o
#requires GNU "make"
LIBOBJS	=$(OBJS:%.o=../$(LIB)(%.o))
MLIBOBJS=$(OBJS:%.o=../$(LIB)(%_m.o))

.DEFAULT:
	$(MAKE) help

all: $(LIBOBJS)

mall: $(MLIBOBJS)
	@$(ECHO) "minish not doing anything"

../$(LIB) : $(LIBOBJS)
	$(AR) $(ARFLAGS) $@ $?
#	-$(RM) -f $?

../$(MLIB) : $(MLIBOBJS)
	$(AR) $(ARFLAGS) $@ $?
#	-$(RM) -f $?

tall: $(OBJS)

TESTS	= shtest   exectest   yacctest   lextest
TESTS_M	= shtest_m exectest_m yacctest_m lextest_m

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
	@$(ECHO) ">>> .c.o $* $< $@"
	$(CC) -c $(MSHFLAGS) $(CFLAGS) $<

.c_m.o :
	@$(ECHO) ">>> .c_m.o $* $< $@"
	$(CC) -o $@ -c $(MSHFLAGS) $(CFLAGS) -DMEMDEBUG $<

shtest : minish.o minish-yacc.o mshexec.o fdlist.o minish-tab.h
	$(CC) $(MSHFLAGS) $(CFLAGS) -o shtest tminish.c \
		minish.o minish-yacc.o mshexec.o fdlist.o \
		-ll -L.. -lrko

shtest_m : minish_m.o minish-yacc_m.o mshexec_m.o fdlist_m.o minish-tab.h
	$(CC) $(MSHFLAGS) $(CFLAGS) -DMEMDEBUG -o shtest_m tminish.c \
		minish_m.o minish-yacc_m.o mshexec_m.o fdlist_m.o \
		-ll -L.. -lrko

exectest : mshexec.c fdlist.o
	$(CC) $(MSHFLAGS) $(CFLAGS) -o exectest -DEXECTEST mshexec.c fdlist.o \
		-ll -L.. -lrko
	-$(RM) mshexec.o

exectest_m : mshexec.c fdlist_m.o
	$(CC) $(MSHFLAGS) $(CFLAGS) -DMEMDEBUG -o exectest_m -DEXECTEST \
		mshexec.c fdlist_m.o \
		-ll -L.. -lrko
	-$(RM) mshexec_m.o

yacctest : minish-yacc.c minish-lex.c fdlist.o
	$(CC) $(MSHFLAGS) $(CFLAGS) -o yacctest \
		-DYACCTEST minish-yacc.c fdlist.o \
		-ll -L.. -lrko
	-$(RM) minish-yacc.o

yacctest_m : minish-yacc.c minish-lex.c fdlist_m.o
	$(CC) $(MSHFLAGS) $(CFLAGS) -DMEMDEBUG -o yacctest_m \
		-DYACCTEST minish-yacc.c fdlist_m.o \
		-ll -L.. -lrko
	-$(RM) minish-yacc_m.o

minish-yacc.o : minish-yacc.c minish-lex.c minish-tab.h
	$(CC) -c $(MSHFLAGS) $(CFLAGS) -DFLEX_STRING $<

minish-yacc_m.o : minish-yacc.c minish-lex.c minish-tab.h
	$(CC) -o minish-yacc_m.o -c $(MSHFLAGS) $(CFLAGS) -DMEMDEBUG \
		-DFLEX_STRING $<

# more direct targets
minish-yacc.c minish-tab.h : minish.yacc
	# creates y.tab.c y.tab.h
	$(YACC) $(YYFLAGS) minish.yacc
	$(YYSED) --prefix minish_yy < y.tab.c > minish-yacc.c
	$(CP) y.tab.h minish-tab.h
	-$(RM) y.tab.c y.tab.h

minish-lex.c : minish.lex
	$(LEX) $(LEXFLAGS) minish.lex
	# creates lex.yy.c
	$(YYSED) --prefix minish_yy < lex.yy.c > minish-lex.c
	-$(RM) lex.yy.c

# explicit targets
#minish-yacc.c : y.tab.c
#	$(YYSED) --prefix minish_yy < $< > minish-yacc.c

#minish-lex.c : lex.yy.c
#	$(LEX) $(LEXFLAGS) minish.lex
#	$(YYSED) --prefix minish_yy < $< > minish-lex.c

#minish-tab.h : y.tab.h
#	$(CP) $< minish-tab.h

y.tab.c y.tab.h : minish.yacc
	$(YACC) $(YYFLAGS) minish.yacc

#lextest : lex.yy.c y.tab.h
lextest : minish-lex.c minish-tab.h
	$(CC) $(MSHFLAGS) $(CFLAGS) -o lextest -DLEXTEST minish-lex.c
	-$(RM) minish-lex.o

lextest_m : minish-lex.c minish-tab.h
	$(CC) $(MSHFLAGS) $(CFLAGS) -DMEMDEBUG -o lextest_m -DLEXTEST \
		minish-lex.c
	-$(RM) minish-lex_m.o

lex.yy.c : minish.lex
	$(LEX) $(LEXFLAGS) minish.lex

fdtest/fdtest : fdlist.c
	$(CC) $(MSHFLAGS) $(CFLAGS) -o fdtest/fdtest \
		-DFDTEST fdlist.c -L.. -lrko

fdtest/fdtest_m : fdlist.c
	$(CC) $(MSHFLAGS) $(CFLAGS) -DMEMDEBUG -o fdtest/fdtest_m \
		-DFDTEST fdlist.c -L.. -lrko

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

runtest_m : runfdtest_m runlextest_m runyacctest_m runexectest_m runshtest_m

runshtest_m : shtest_m mtest_m
	-@./shtest_m
	-@if diff shtest.out cmp/shtest.cmp ; then \
		echo shtest OK ; else echo shtest FAILED ; fi

runexectest_m : exectest_m mtest_m
	-@./exectest_m
	-@if diff exectest.out cmp/exectest.cmp ; then \
		echo exectest OK ; else echo exectest FAILED ; fi

runyacctest_m : yacctest_m
	-@./yacctest_m > yacctest.out
	-@if diff yacctest.out cmp/yacctest.cmp ; then \
		echo yacctest OK ; else echo yacctest FAILED ; fi

runlextest_m : lextest_m
	-@./lextest_m > lextest.out
	-@if diff lextest.out cmp/lextest.cmp ; then \
		echo lextest OK ; else echo lextest FAILED ; fi

runfdtest_m : fdtest/fdtest_m $(MEMDBG)
	-@fdtest/fdtest_m 2>&1 | $(MEMDBG)

clean :
	-$(RM) -f *.o a.out core y.output
	-$(RM) -f fdtest/fdtest fdtest/fdtest.*out
	-$(RM) -f mtest/right mtest/wrong mtest/rsignal mtest/core
	-$(RM) -f lextest.out yacctest.out exectest.out shtest.out

wipe : clean
	-$(RM) $(TESTS) $(TESTS_M)

clobber: wipe

distclean: clobber
	-$(RM) -f lex.yy.c y.tab.c
	-$(RM) -f minish-lex.c minish-yacc.c minish-tab.h y.tab.h
