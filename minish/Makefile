CC	=gcc
LD	=gcc -o
LEX	=lex
YACC	=yacc
YYFLAGS	=-v -d -t
RM	=rm
SHELL	=sh
#
CFLAGS=-g -DRKOERROR -I..

.c.o :
	$(CC) -c $(CFLAGS) $<

exectest : mshexec.c fdlist.o
	$(CC) $(CFLAGS) -o exectest -DTEST mshexec.c fdlist.o \
		-ll -L.. -lrko

yacctest : y.tab.c lex.yy.c fdlist.o
	$(CC) $(CFLAGS) -o yacctest -DYACCTEST y.tab.c fdlist.o \
		-ll -L.. -lrko

y.tab.c y.tab.h : minish.yacc
	$(YACC) $(YYFLAGS) minish.yacc

lextest : lex.yy.c y.tab.h
	$(CC) $(CFLAGS) -o lextest -DLEXTEST lex.yy.c -ll

lex.yy.c : minish.lex
	$(LEX) minish.lex

fdtest/fdtest : fdlist.c
	$(CC) $(CFLAGS) -o fdtest/fdtest -DTEST fdlist.c -L.. -lrko

mtest : mtest/right mtest/wrong mtest/rsignal

mtest/right : mtest/right.c
	$(CC) $(CFLAGS) -o mtest/right $<

mtest/wrong : mtest/wrong.c
	$(CC) $(CFLAGS) -o mtest/wrong $<

mtest/rsignal : mtest/rsignal.c
	$(CC) $(CFLAGS) -o mtest/rsignal $<

runtest : runfdtest runlextest runyacctest runexectest

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
	-$(RM) -f lextest.out yacctest.out exectest.out

wipe : clean
	-$(RM) lextest yacctest

clobber: wipe
	-$(RM) -f lex.yy.c y.tab.c
