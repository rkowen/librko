#ifndef lint
static char yysccsid[] = "@(#)yaccpar	1.8 (Berkeley) 01/20/90";
#endif
#define YYBYACC 1
#line 13 "minish.yacc"
#ifdef YACCTEST
#  define _MAIN_
#endif

#include <stdio.h>
#include <unistd.h>
#include "librko.h"
#include "minish.h"

#ifdef _CRAY
extern char *minish_yytext;
#endif

static int linenum = 0;
static int statnum = 0;
static int fdnum = 0;
static int fdnum2 = 0;
static minish_fd_action action;
static char **ptr;
static char buffer[20];
static char filename[FILENAME_MAX];

static uvec *statement;
static uvec *minish_argv;
static minish_fdlist *fdlist;

typedef enum {NEXT_NONE, NEXT_ANY, NEXT_IF_GOOD, NEXT_IF_BAD} minish_next;
static minish_next next_command = NEXT_ANY;
static int retval = 0;
int minish_signal = 0;
int minish_retval = 0;

#if defined(YACCTEST) || defined(DEBUG)
void dbgstop(void) {
	static int a=1;
}

void dump_uvec(char *string, uvec *m_uvec) {
	char **ptr;

	printf("%s: ", string);
	for (ptr = uvec_vector(m_uvec);
		*ptr != (char *) NULL; ++ptr) {
		printf("%s ", *ptr);
	}
	printf("\n");
}
#endif /* YACCTEST */

#line 56 "y.tab.c"
#define COMMAND 257
#define LPAREN 258
#define RPAREN 259
#define EOC 260
#define EOL 261
#define AMPER_AMPER 262
#define BAR_BAR 263
#define BAR 264
#define BAR_AMPER 265
#define GREATER_AMPER_CLOSE 266
#define GREATER_AMPER 267
#define GREATER_GREATER 268
#define GREATER_THAN 269
#define LESS_AMPER 270
#define LESS_THAN 271
#define LESS_GREATER 272
#define NUMBER 273
#define WORD 274
#define YYERRCODE 256
short yylhs[] = {                                        -1,
    0,    0,    0,    1,    2,    3,    3,    3,    5,    5,
    5,    7,    7,   10,   10,   10,   10,    8,   13,    9,
    9,    9,   11,    4,    4,    4,    4,   12,    6,   14,
};
short yylen[] = {                                         2,
    1,    1,    2,    3,    1,    0,    2,    2,    1,    1,
    1,    2,    3,    1,    1,    1,    1,    3,    1,    1,
    1,    2,    1,    1,    1,    1,    1,    1,    1,    1,
};
short yydefred[] = {                                      0,
    5,    1,    0,    2,    6,    3,    0,   25,   30,   27,
   26,   21,   15,   14,   20,   16,   17,   28,   29,    4,
    7,    8,    9,   10,   11,    0,    0,   24,   23,   12,
   22,   19,    0,    0,   13,   18,
};
short yydgoto[] = {                                       3,
    4,    5,    7,   20,   21,   29,   23,   24,   25,   26,
   30,   27,   34,   28,
};
short yysindex[] = {                                   -238,
    0,    0, -253,    0,    0,    0, -260,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -269, -251,    0,    0,    0,
    0,    0, -269, -266,    0,    0,
};
short yyrindex[] = {                                      0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,
};
short yygindex[] = {                                      0,
   19,    0,    0,    0,    0,   17,    0,    0,    0,   -2,
   -7,   -6,    0,    0,
};
#define YYTABLESIZE 28
short yytable[] = {                                       8,
    9,   10,   11,    1,   19,   12,   18,   13,   14,   15,
   16,   17,   18,   19,   31,   32,   13,   14,    1,   16,
   17,    6,    2,   22,   33,   35,    0,   36,
};
short yycheck[] = {                                     260,
  261,  262,  263,  257,  274,  266,  273,  268,  269,  270,
  271,  272,  273,  274,  266,  267,  268,  269,  257,  271,
  272,    3,  261,    7,   27,   33,   -1,   34,
};
#define YYFINAL 3
#ifndef YYDEBUG
#define YYDEBUG 1
#endif
#define YYMAXTOKEN 274
#if YYDEBUG
char *yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"COMMAND","LPAREN","RPAREN","EOC",
"EOL","AMPER_AMPER","BAR_BAR","BAR","BAR_AMPER","GREATER_AMPER_CLOSE",
"GREATER_AMPER","GREATER_GREATER","GREATER_THAN","LESS_AMPER","LESS_THAN",
"LESS_GREATER","NUMBER","WORD",
};
char *yyrule[] = {
"$accept : statements",
"statements : EOL",
"statements : statement",
"statements : statements statement",
"statement : command options eoc",
"command : COMMAND",
"options :",
"options : options redirection",
"options : options word",
"redirection : redir_file",
"redirection : redir_fd",
"redirection : redir_close",
"redir_file : redirect file",
"redir_file : number redirect file",
"redirect : GREATER_THAN",
"redirect : GREATER_GREATER",
"redirect : LESS_THAN",
"redirect : LESS_GREATER",
"redir_fd : number redir_ga number",
"redir_ga : GREATER_AMPER",
"redir_close : LESS_AMPER",
"redir_close : GREATER_AMPER_CLOSE",
"redir_close : number GREATER_AMPER_CLOSE",
"file : word",
"eoc : eol",
"eoc : EOC",
"eoc : BAR_BAR",
"eoc : AMPER_AMPER",
"number : NUMBER",
"word : WORD",
"eol : EOL",
};
#endif
#ifndef YYSTYPE
typedef int YYSTYPE;
#endif
#define yyclearin (minish_yychar=(-1))
#define yyerrok (minish_yyerrflag=0)
#ifdef YYSTACKSIZE
#ifndef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#endif
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 500
#define YYMAXDEPTH 500
#endif
#endif
int minish_yydebug;
int minish_yynerrs;
int minish_yyerrflag;
int minish_yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE minish_yyval;
YYSTYPE minish_yylval;
short yyss[YYSTACKSIZE];
YYSTYPE yyvs[YYSTACKSIZE];
#define yystacksize YYSTACKSIZE
#line 387 "minish.yacc"
 /* start of programs */

#undef input()		/* eliminates a useless warning */
#include "minish-lex.c"

int minish_yyerror(const char *s) {
	fprintf(stdout,"minish.yacc: %s at '%s'\n", s, minish_yytext);
}

#ifdef YACCTEST
/* mini-sh routines */

int main(void) {
	FILE *testfile;
#if 0
	extern int minish_yydebug;
	minish_yydebug = 1;
#endif
	if (!(testfile = freopen("lextest.dat","r",stdin))) {
		fprintf(stderr,"Can't open lextest.dat file for testing\n");
		return 1;
	}
	return minish_yyparse();
}
#endif /* YACCTEST */
#line 229 "y.tab.c"
#define YYABORT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
minish_yyparse()
{
    register int yym, yyn, minish_yystate;
#if YYDEBUG
    register char *minish_yys;
    extern char *getenv();

    if (minish_yys = getenv("YYDEBUG"))
    {
        yyn = *minish_yys;
        if (yyn >= '0' && yyn <= '9')
            minish_yydebug = yyn - '0';
    }
#endif

    minish_yynerrs = 0;
    minish_yyerrflag = 0;
    minish_yychar = (-1);

    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = minish_yystate = 0;

yyloop:
    if (yyn = yydefred[minish_yystate]) goto yyreduce;
    if (minish_yychar < 0)
    {
        if ((minish_yychar = minish_yylex()) < 0) minish_yychar = 0;
#if YYDEBUG
        if (minish_yydebug)
        {
            minish_yys = 0;
            if (minish_yychar <= YYMAXTOKEN) minish_yys = yyname[minish_yychar];
            if (!minish_yys) minish_yys = "illegal-symbol";
            printf("minish_yydebug: state %d, reading %d (%s)\n", minish_yystate,
                    minish_yychar, minish_yys);
        }
#endif
    }
    if ((yyn = yysindex[minish_yystate]) && (yyn += minish_yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == minish_yychar)
    {
#if YYDEBUG
        if (minish_yydebug)
            printf("minish_yydebug: state %d, shifting to state %d\n",
                    minish_yystate, yytable[yyn]);
#endif
        if (yyssp >= yyss + yystacksize - 1)
        {
            goto yyoverflow;
        }
        *++yyssp = minish_yystate = yytable[yyn];
        *++yyvsp = minish_yylval;
        minish_yychar = (-1);
        if (minish_yyerrflag > 0)  --minish_yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[minish_yystate]) && (yyn += minish_yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == minish_yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (minish_yyerrflag) goto yyinrecovery;
#ifdef lint
    goto yynewerror;
#endif
yynewerror:
    minish_yyerror("syntax error");
#ifdef lint
    goto yyerrlab;
#endif
yyerrlab:
    ++minish_yynerrs;
yyinrecovery:
    if (minish_yyerrflag < 3)
    {
        minish_yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (minish_yydebug)
                    printf("minish_yydebug: state %d, error recovery shifting\
 to state %d\n", *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yyss + yystacksize - 1)
                {
                    goto yyoverflow;
                }
                *++yyssp = minish_yystate = yytable[yyn];
                *++yyvsp = minish_yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (minish_yydebug)
                    printf("minish_yydebug: error recovery discarding state %d\n",
                            *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (minish_yychar == 0) goto yyabort;
#if YYDEBUG
        if (minish_yydebug)
        {
            minish_yys = 0;
            if (minish_yychar <= YYMAXTOKEN) minish_yys = yyname[minish_yychar];
            if (!minish_yys) minish_yys = "illegal-symbol";
            printf("minish_yydebug: state %d, error recovery discards token %d (%s)\n",
                    minish_yystate, minish_yychar, minish_yys);
        }
#endif
        minish_yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (minish_yydebug)
        printf("minish_yydebug: state %d, reducing by rule %d (%s)\n",
                minish_yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    minish_yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 4:
#line 77 "minish.yacc"
{
#if defined(YACCTEST) || defined(DEBUG)
			printf("statement: %d\n", ++statnum);
#endif
		}
break;
case 5:
#line 84 "minish.yacc"
{
			fdnum = -1;
#if defined(YACCTEST) || defined(DEBUG)
			printf("command:%s\n", minish_yytext);
#endif
			if (!(fdlist = minish_fdlist_ctor()))
				rkoperror("minish : yacc : fd : ctor");
			if (!(minish_argv = uvec_ctor(10)))
				rkoperror("minish : yacc : arg : ctor");
			if (uvec_add(minish_argv,minish_yytext))
				rkoperror("minish : yacc : com");
			if (!uvec_exists(statement)) {
				if (!(statement = uvec_ctor(10)))
					rkoperror("minish : yacc : stm : ctor");
			}
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : com");
		}
break;
case 8:
#line 107 "minish.yacc"
{
#if defined(YACCTEST) || defined(DEBUG)
			printf("option:%s\n", minish_yytext);
#endif
			if (uvec_add(minish_argv,minish_yytext))
				rkoperror("minish : yacc : option ");
		}
break;
case 12:
#line 124 "minish.yacc"
{
#if defined(YACCTEST) || defined(DEBUG)
			printf("redirect:stdin/out:%s\n", minish_yytext);
#endif
			if (minish_fdlist_add(fdlist, action,
			fdnum, filename))
				rkoperror("minish : yacc : redir : stdio");
			/* special case of <> */
			if (action == MINISH_FD_READWRITE)
				if (minish_fdlist_add(fdlist,
				MINISH_FD_REDIRECT, STDOUT_FILENO,
				STDOUT_FILENO)) rkoperror(
					"minish : yacc : redir : rw");
		}
break;
case 13:
#line 139 "minish.yacc"
{
			fdnum = yyvsp[-2];
#if defined(YACCTEST) || defined(DEBUG)
			printf("redirect:%d:%s\n", fdnum, minish_yytext);
#endif
			if (minish_fdlist_add(fdlist, action,
			fdnum, filename))
				rkoperror("minish : yacc : redir : fd");
		}
break;
case 14:
#line 151 "minish.yacc"
{
			fdnum = STDOUT_FILENO;
			action = MINISH_FD_WRITE;
#if defined(YACCTEST) || defined(DEBUG)
			printf("redirect:%s\n", minish_yytext);
#endif
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : red : stdout");
		}
break;
case 15:
#line 161 "minish.yacc"
{
			fdnum = STDOUT_FILENO;
			action = MINISH_FD_APPEND;
#if defined(YACCTEST) || defined(DEBUG)
			printf("redirect:%s\n", minish_yytext);
#endif
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : app");
		}
break;
case 16:
#line 171 "minish.yacc"
{
			fdnum = STDIN_FILENO;
			action = MINISH_FD_READ;
#if defined(YACCTEST) || defined(DEBUG)
			printf("redirect:%s\n", minish_yytext);
#endif
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : red");
		}
break;
case 17:
#line 181 "minish.yacc"
{
			fdnum = STDIN_FILENO;
			action = MINISH_FD_READWRITE;
#if defined(YACCTEST) || defined(DEBUG)
			printf("redirect:%s\n", minish_yytext);
#endif
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : rw");
		}
break;
case 18:
#line 193 "minish.yacc"
{
			fdnum = yyvsp[-2];
			fdnum2 = yyvsp[0];
#if defined(YACCTEST) || defined(DEBUG)
			printf("redirect:%d to %d\n", fdnum, fdnum2);
#endif
			if (minish_fdlist_add(fdlist, MINISH_FD_REDIRECT,
			fdnum, fdnum2))
				rkoperror("minish : yacc : redir : fd");
		}
break;
case 19:
#line 206 "minish.yacc"
{
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : fd-red");
		}
break;
case 20:
#line 213 "minish.yacc"
{
			fdnum = STDIN_FILENO;
#if defined(YACCTEST) || defined(DEBUG)
			printf("close:%d:\n", fdnum);
#endif
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : close : stdin : stm");
			if (minish_fdlist_add(fdlist, MINISH_FD_CLOSE, fdnum))
				rkoperror("minish : yacc : close : stdin : fd");
		}
break;
case 21:
#line 224 "minish.yacc"
{
			fdnum = STDOUT_FILENO;
#if defined(YACCTEST) || defined(DEBUG)
			printf("close:%d:\n", fdnum);
#endif
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : close : stdout : stm");
			if (minish_fdlist_add(fdlist, MINISH_FD_CLOSE, fdnum))
				rkoperror("minish : yacc : close : stdout : fd");
		}
break;
case 22:
#line 235 "minish.yacc"
{
			fdnum = yyvsp[-1];
#if defined(YACCTEST) || defined(DEBUG)
			printf("close:%d:\n", fdnum);
#endif
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : close : n : stm");
			if (minish_fdlist_add(fdlist, MINISH_FD_CLOSE, fdnum))
				rkoperror("minish : yacc : close : n : fd");
		}
break;
case 23:
#line 248 "minish.yacc"
{
#if defined(YACCTEST) || defined(DEBUG)
			printf("file:%s\n", minish_yytext);
#endif
			(void) strcpy(filename, minish_yytext);
		}
break;
case 25:
#line 257 "minish.yacc"
{
#if defined(YACCTEST) || defined(DEBUG)
			printf("eoc:%s\n", minish_yytext);
			dump_uvec("<argv>", minish_argv);
			if (minish_fdlist_dump(fdlist, stdout))
				rkoperror("minish : yacc : eoc : fd : dump");
#endif
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : eoc : stm");

#ifndef YACCTEST
			if (next_command==NEXT_ANY ||
			(retval && next_command==NEXT_IF_BAD) ||
			(!retval && next_command==NEXT_IF_GOOD)) {
				retval = minish_exec(minish_argv, fdlist,
					&minish_signal);
			}
			next_command = NEXT_ANY;
#endif

			if (uvec_dtor(&minish_argv))
				rkoperror("minish : yacc : eoc : arg : dtor");
			if (minish_fdlist_dtor(&fdlist))
				rkoperror("minish : yacc : eoc : fd : dtor");
		}
break;
case 26:
#line 283 "minish.yacc"
{
#if defined(YACCTEST) || defined(DEBUG)
			printf("eoc:%s\n", minish_yytext);
			dump_uvec("<argv>", minish_argv);
			if (minish_fdlist_dump(fdlist, stdout))
				rkoperror("minish : yacc : || : fd : dump");
#endif
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : || : stm");

#ifndef YACCTEST
			if (next_command==NEXT_ANY ||
			(retval && next_command==NEXT_IF_BAD) ||
			(!retval && next_command==NEXT_IF_GOOD)) {
				retval = minish_exec(minish_argv, fdlist,
					&minish_signal);
			}
			next_command = NEXT_IF_BAD;
#endif

			if (uvec_dtor(&minish_argv))
				rkoperror("minish : yacc : || : arg : dtor");
			if (minish_fdlist_dtor(&fdlist))
				rkoperror("minish : yacc : || : fd : dtor");
		}
break;
case 27:
#line 309 "minish.yacc"
{
#if defined(YACCTEST) || defined(DEBUG)
			printf("eoc:%s\n", minish_yytext);
			dump_uvec("<argv>", minish_argv);
			if (minish_fdlist_dump(fdlist, stdout))
				rkoperror("minish : yacc : && : fd : dump");
#endif
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : && : stm");

#ifndef YACCTEST
			if (next_command==NEXT_ANY ||
			(retval && next_command==NEXT_IF_BAD) ||
			(!retval && next_command==NEXT_IF_GOOD)) {
				retval = minish_exec(minish_argv, fdlist,
					&minish_signal);
			}
			next_command = NEXT_IF_GOOD;
#endif

			if (uvec_dtor(&minish_argv))
				rkoperror("minish : yacc : && : arg : dtor");
			if (minish_fdlist_dtor(&fdlist))
				rkoperror("minish : yacc : && : fd : dtor");
		}
break;
case 28:
#line 337 "minish.yacc"
{
			minish_yyval = yyvsp[0];
#if defined(YACCTEST) || defined(DEBUG)
			printf("number:%s\n", minish_yytext);
#endif
			strcpy(buffer, "(");
			strcat(buffer, minish_yytext);
			strcat(buffer, ")");
			if (uvec_add(statement,buffer))
				rkoperror("minish : yacc : number");
		}
break;
case 29:
#line 350 "minish.yacc"
{
#if defined(YACCTEST) || defined(DEBUG)
			printf("word:%s\n", minish_yytext);
#endif
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : word");
		}
break;
case 30:
#line 359 "minish.yacc"
{
#if defined(YACCTEST) || defined(DEBUG)
			dump_uvec("<argv>", minish_argv);
			printf("eol: linenum = %d\n", ++linenum);
			dump_uvec("<line>", statement);
			if (minish_fdlist_dump(fdlist, stdout))
				rkoperror("minish : yacc : eol : fd : dump");
#endif
#ifndef YACCTEST
			if (next_command==NEXT_ANY ||
			(retval && next_command==NEXT_IF_BAD) ||
			(!retval && next_command==NEXT_IF_GOOD)) {
				retval = minish_exec(minish_argv, fdlist,
					&minish_signal);
			}
			minish_retval = retval;
			next_command = NEXT_ANY;
#endif

			if (uvec_dtor(&statement))
				rkoperror("minish : yacc : eol : stm : dtor");
			if (uvec_dtor(&minish_argv))
				rkoperror("minish : yacc : eol : arg : dtor");
			if (minish_fdlist_dtor(&fdlist))
				rkoperror("minish : yacc : eol : fd : dtor");
		}
break;
#line 690 "y.tab.c"
    }
    yyssp -= yym;
    minish_yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (minish_yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (minish_yydebug)
            printf("minish_yydebug: after reduction, shifting from state 0 to\
 state %d\n", YYFINAL);
#endif
        minish_yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = minish_yyval;
        if (minish_yychar < 0)
        {
            if ((minish_yychar = minish_yylex()) < 0) minish_yychar = 0;
#if YYDEBUG
            if (minish_yydebug)
            {
                minish_yys = 0;
                if (minish_yychar <= YYMAXTOKEN) minish_yys = yyname[minish_yychar];
                if (!minish_yys) minish_yys = "illegal-symbol";
                printf("minish_yydebug: state %d, reading %d (%s)\n",
                        YYFINAL, minish_yychar, minish_yys);
            }
#endif
        }
        if (minish_yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += minish_yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == minish_yystate)
        minish_yystate = yytable[yyn];
    else
        minish_yystate = yydgoto[yym];
#if YYDEBUG
    if (minish_yydebug)
        printf("minish_yydebug: after reduction, shifting from state %d \
to state %d\n", *yyssp, minish_yystate);
#endif
    if (yyssp >= yyss + yystacksize - 1)
    {
        goto yyoverflow;
    }
    *++yyssp = minish_yystate;
    *++yyvsp = minish_yyval;
    goto yyloop;
yyoverflow:
    minish_yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
