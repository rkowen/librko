CC	=gcc
LD	=gcc -o
LEX	=lex
YACC	=yacc
YYFLAGS	=-v -d -t
RM	=rm
#
CFLAGS=-g

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

clean :
	$(RM) -f *.o a.out core y.output

wipe : clean
	$(RM) lextest yacctest

clobber: wipe
	$(RM) -f lex.yy.c y.tab.c
