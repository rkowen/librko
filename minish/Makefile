CC	=gcc
LD	=gcc -o
LEX	=lex
LEXFLAGS=
YACC	=yacc
YYFLAGS	=-v -d -t
RM	=rm
SHELL	=sh
YYSED	=../etc/yy-sed
CP	=cp
#
CFLAGS=-g -DRKOERROR -I.. -DTEST

.c.o :
	$(CC) -c $(CFLAGS) $<

shtest : minish.c minish-yacc.o mshexec.o fdlist.o
	$(CC) $(CFLAGS) -o shtest -DSHTEST minish.c \
		minish-yacc.o mshexec.o fdlist.o \
		-ll -L.. -lrko

exectest : mshexec.c fdlist.o
	$(CC) $(CFLAGS) -o exectest -DEXECTEST mshexec.c fdlist.o \
		-ll -L.. -lrko

#yacctest : y.tab.c lex.yy.c fdlist.o
yacctest : minish-yacc.c minish-lex.c fdlist.o
	$(CC) $(CFLAGS) -o yacctest -DYACCTEST minish-yacc.c fdlist.o \
		-ll -L.. -lrko

minish-yacc.o : minish-yacc.c minish-lex.c
	$(CC) -c $(CFLAGS) -DFLEX_STRING $<

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
	$(CC) $(CFLAGS) -o lextest -DLEXTEST minish-lex.c -ll

lex.yy.c : minish.lex
	$(LEX) $(LEXFLAGS) minish.lex

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
	-$(RM) lextest yacctest exectest

clobber: wipe
	-$(RM) -f lex.yy.c y.tab.c

distclean: clobber
	-$(RM) -f minish-lex.c minish-yacc.c
