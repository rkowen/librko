/* minish.yacc - grammar for minish program
 */
/* declarations */
%token COMMAND EOC EOL
%token AMPER_AMPER BAR_BAR
%token GREATER_AMPER_CLOSE GREATER_AMPER GREATER_GREATER GREATER_THAN
%token LESS_AMPER LESS_THAN LESS_GREATER
%token NUMBER WORD

%left EOC BAR_BAR AMPER_AMPER

%{
#ifdef YACCTEST
#  define _MAIN_
#else
static int i;
#endif

#include <stdio.h>
#include <unistd.h>
#include "librko.h"
#include "minish.h"

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

#ifdef YACCTEST
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

%}

%start statements

%%
/* grammar rules */

statements
	: EOL
	| statement
	| statements statement
	;

statement
	: command options eoc
		{
#ifdef YACCTEST
			printf("statement: %d\n", ++statnum);
#endif
		}
	;
command : COMMAND
		{
			fdnum = -1;
#ifdef YACCTEST
			printf("command:%s\n", minish_yytext);
#endif
			if (!(fdlist = minish_fdlist_ctor()))
				rkoperror("minish : yacc : fd : ctor");
			if (!(minish_argv = uvec_ctor(10)))
				rkoperror("minish : yacc : arg : ctor");
			if (uvec_add(minish_argv,minish_yytext))
				rkoperror("minish : yacc : com");
			if (! uvec_exists(statement)) {
				if (!(statement = uvec_ctor(10)))
					rkoperror("minish : yacc : stm : ctor");
			}
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : com");
		}
	;

options :	/* empty options */
	| options redirection
	| options word
		{
#ifdef YACCTEST
			printf("option:%s\n", minish_yytext);
#endif
			if (uvec_add(minish_argv,minish_yytext))
				rkoperror("minish : yacc : option ");
		}
	;

redirection
	: redir_file
	| redir_fd
	| redir_close
	;

redir_file
	: redirect file
		{
#ifdef YACCTEST
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
	| number redirect file
		{
			fdnum = $1;
#ifdef YACCTEST
			printf("redirect:%d:%s\n", fdnum, minish_yytext);
#endif
			if (minish_fdlist_add(fdlist, action,
			fdnum, filename))
				rkoperror("minish : yacc : redir : fd");
		}
	;

redirect: GREATER_THAN
		{
			fdnum = STDOUT_FILENO;
			action = MINISH_FD_WRITE;
#ifdef YACCTEST
			printf("redirect:%s\n", minish_yytext);
#endif
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : red : stdout");
		}
	| GREATER_GREATER
		{
			fdnum = STDOUT_FILENO;
			action = MINISH_FD_APPEND;
#ifdef YACCTEST
			printf("redirect:%s\n", minish_yytext);
#endif
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : app");
		}
	| LESS_THAN
		{
			fdnum = STDIN_FILENO;
			action = MINISH_FD_READ;
#ifdef YACCTEST
			printf("redirect:%s\n", minish_yytext);
#endif
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : red");
		}
	| LESS_GREATER
		{
			fdnum = STDIN_FILENO;
			action = MINISH_FD_READWRITE;
#ifdef YACCTEST
			printf("redirect:%s\n", minish_yytext);
#endif
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : rw");
		}
	;

redir_fd: number redir_ga number
		{
			fdnum = $1;
			fdnum2 = $3;
#ifdef YACCTEST
			printf("redirect:%d to %d\n", fdnum, fdnum2);
#endif
			if (minish_fdlist_add(fdlist, MINISH_FD_REDIRECT,
			fdnum, fdnum2))
				rkoperror("minish : yacc : redir : fd");
		}

redir_ga
	: GREATER_AMPER
		{
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : fd-red");
		}

redir_close
	: LESS_AMPER
		{
			fdnum = STDIN_FILENO;
#ifdef YACCTEST
			printf("close:%d:\n", fdnum);
#endif
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : close : stdin : stm");
			if (minish_fdlist_add(fdlist, MINISH_FD_CLOSE, fdnum))
				rkoperror("minish : yacc : close : stdin : fd");
		}
	| GREATER_AMPER_CLOSE
		{
			fdnum = STDOUT_FILENO;
#ifdef YACCTEST
			printf("close:%d:\n", fdnum);
#endif
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : close : stdout : stm");
			if (minish_fdlist_add(fdlist, MINISH_FD_CLOSE, fdnum))
				rkoperror("minish : yacc : close : stdout : fd");
		}
	| number GREATER_AMPER_CLOSE
		{
			fdnum = $1;
#ifdef YACCTEST
			printf("close:%d:\n", fdnum);
#endif
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : close : n : stm");
			if (minish_fdlist_add(fdlist, MINISH_FD_CLOSE, fdnum))
				rkoperror("minish : yacc : close : n : fd");
		}
	;

file	: word
		{
#ifdef YACCTEST
			printf("file:%s\n", minish_yytext);
#endif
			(void) strcpy(filename, minish_yytext);
		}

eoc	: eol
	| EOC
		{
#ifdef YACCTEST
			printf("eoc:%s\n", minish_yytext);
			dump_uvec("<argv>", minish_argv);
			if (minish_fdlist_dump(fdlist, stdout))
				rkoperror("minish : yacc : eoc : fd : dump");
#else
			if (minish_fdlist_process(fdlist))
				rkoperror("minish : yacc : eoc : fd : process");
#endif
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : eoc : stm");

			if (uvec_dtor(&minish_argv))
				rkoperror("minish : yacc : eoc : arg : dtor");
			if (minish_fdlist_dtor(&fdlist))
				rkoperror("minish : yacc : eoc : fd : dtor");
		}
	| BAR_BAR
		{
#ifdef YACCTEST
			printf("eoc:%s\n", minish_yytext);
			dump_uvec("<argv>", minish_argv);
			if (minish_fdlist_dump(fdlist, stdout))
				rkoperror("minish : yacc : || : fd : dump");
#else
			if (minish_fdlist_process(fdlist))
				rkoperror("minish : yacc : || : fd : process");
#endif
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : || : stm");

			if (uvec_dtor(&minish_argv))
				rkoperror("minish : yacc : || : arg : dtor");
			if (minish_fdlist_dtor(&fdlist))
				rkoperror("minish : yacc : || : fd : dtor");
		}
	| AMPER_AMPER
		{
#ifdef YACCTEST
			printf("eoc:%s\n", minish_yytext);
			dump_uvec("<argv>", minish_argv);
			if (minish_fdlist_dump(fdlist, stdout))
				rkoperror("minish : yacc : && : fd : dump");
#else
			if (minish_fdlist_process(fdlist))
				rkoperror("minish : yacc : && : fd : process");
#endif
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : && : stm");

			if (uvec_dtor(&minish_argv))
				rkoperror("minish : yacc : && : arg : dtor");
			if (minish_fdlist_dtor(&fdlist))
				rkoperror("minish : yacc : && : fd : dtor");
		}
	;

number	: NUMBER
		{
			$$ = $1;
#ifdef YACCTEST
			printf("number:%s\n", minish_yytext);
#endif
			strcpy(buffer, "(");
			strcat(buffer, minish_yytext);
			strcat(buffer, ")");
			if (uvec_add(statement,buffer))
				rkoperror("minish : yacc : number");
		}

word	: WORD
		{
#ifdef YACCTEST
			printf("word:%s\n", minish_yytext);
#endif
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : word");
		}

eol	: EOL
		{
#ifdef YACCTEST
			dump_uvec("<argv>", minish_argv);
			printf("eol: linenum = %d\n", ++linenum);
			dump_uvec("<line>", statement);
			if (minish_fdlist_dump(fdlist, stdout))
				rkoperror("minish : yacc : eol : fd : dump");
#else
			if (minish_fdlist_process(fdlist))
				rkoperror("minish : yacc : && : fd : process");
#endif

			if (uvec_dtor(&statement))
				rkoperror("minish : yacc : eol : stm : dtor");
			if (uvec_dtor(&minish_argv))
				rkoperror("minish : yacc : eol : arg : dtor");
			if (minish_fdlist_dtor(&fdlist))
				rkoperror("minish : yacc : eol : fd : dtor");
		}
	;

%% /* start of programs */

#undef input()		/* eliminates a useless warning */
#include "minish-lex.c"

int minish_yyerror(const char *s) {
	fprintf(stderr,"minish.yacc: %s\n", s);
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
	return 0;
}
#endif /* YACCTEST */
