CC	=gcc
LD	=gcc -o
LEX	=lex
YACC	=yacc
YYFLAGS	=-v -d -t
RM	=rm
#
CFLAGS=-g -DRKOERROR

.c.o :
	$(CC) -c $(CFLAGS) $<

yacctest : y.tab.c lex.yy.c
	$(CC) $(CFLAGS) -I.. -o yacctest -DYACCTEST y.tab.c -ll -L.. -lrko

y.tab.c y.tab.h : minish.yacc
	$(YACC) $(YYFLAGS) minish.yacc

lextest : lex.yy.c y.tab.h
	$(CC) $(CFLAGS) -o lextest -DLEXTEST lex.yy.c -ll

lex.yy.c : minish.lex
	$(LEX) minish.lex

fdtest/fdtest : fdlist.c
	$(CC) $(CFLAGS) -I.. -o fdtest/fdtest -DTEST fdlist.c -L.. -lrko

runfdtest : fdtest/fdtest
	-(cd fdtest; ./fdtest.sh)

clean :
	-$(RM) -f *.o a.out core y.output
	-$(RM) -f fdtest/fdtest fdtest/fdtest.*out

wipe : clean
	-$(RM) lextest yacctest

clobber: wipe
	-$(RM) -f lex.yy.c y.tab.c
