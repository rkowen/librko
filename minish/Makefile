CC	=gcc
LD	=gcc -o
LEX	=lex
YACC	=yacc
RM	=rm
#
CFLAGS=-g

.c.o :
	$(CC) -c $(CFLAGS) $<

yacctest : y.tab.c lex.yy.c
	$(CC) $(CFLAGS) -o yacctest -DYACCTEST y.tab.c -ll -lrko

y.tab.c : minish.yacc
	$(YACC) -t minish.yacc

lextest : lex.yy.c
	$(CC) $(CFLAGS) -o lextest -DLEXTEST lex.yy.c -ll

lex.yy.c : minish.lex
	$(LEX) minish.lex

clobber:
	$(RM) -f *.o core a.out lex.yy.c y.tab.c
