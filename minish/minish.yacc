/* minish.yacc - grammar for minish program
 */
/* declarations */
%token COMMAND LPAREN RPAREN EOC EOL
%token AMPER_AMPER BAR_BAR BAR BAR_AMPER
%token GREATER_AMPER_CLOSE GREATER_AMPER GREATER_GREATER GREATER_THAN
%token LESS_AMPER LESS_THAN LESS_GREATER
%token NUMBER WORD

%left EOC BAR_BAR AMPER_AMPER

%{
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
#if defined(YACCTEST) || defined(DEBUG)
			printf("statement: %d\n", ++statnum);
#endif
		}
	;
command : COMMAND
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
	;

options :	/* empty options */
	| options redirection
	| options word
		{
#if defined(YACCTEST) || defined(DEBUG)
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
	| number redirect file
		{
			fdnum = $1;
#if defined(YACCTEST) || defined(DEBUG)
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
#if defined(YACCTEST) || defined(DEBUG)
			printf("redirect:%s\n", minish_yytext);
#endif
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : red : stdout");
		}
	| GREATER_GREATER
		{
			fdnum = STDOUT_FILENO;
			action = MINISH_FD_APPEND;
#if defined(YACCTEST) || defined(DEBUG)
			printf("redirect:%s\n", minish_yytext);
#endif
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : app");
		}
	| LESS_THAN
		{
			fdnum = STDIN_FILENO;
			action = MINISH_FD_READ;
#if defined(YACCTEST) || defined(DEBUG)
			printf("redirect:%s\n", minish_yytext);
#endif
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : red");
		}
	| LESS_GREATER
		{
			fdnum = STDIN_FILENO;
			action = MINISH_FD_READWRITE;
#if defined(YACCTEST) || defined(DEBUG)
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
#if defined(YACCTEST) || defined(DEBUG)
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
#if defined(YACCTEST) || defined(DEBUG)
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
#if defined(YACCTEST) || defined(DEBUG)
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
#if defined(YACCTEST) || defined(DEBUG)
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
#if defined(YACCTEST) || defined(DEBUG)
			printf("file:%s\n", minish_yytext);
#endif
			(void) strcpy(filename, minish_yytext);
		}

eoc	: eol
	| EOC
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
	| BAR_BAR
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
	| AMPER_AMPER
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
	;

number	: NUMBER
		{
			$$ = $1;
#if defined(YACCTEST) || defined(DEBUG)
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
#if defined(YACCTEST) || defined(DEBUG)
			printf("word:%s\n", minish_yytext);
#endif
			if (uvec_add(statement,minish_yytext))
				rkoperror("minish : yacc : word");
		}

eol	: EOL
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
	;

%% /* start of programs */

#undef input()		/* eliminates a useless warning */
#include "minish-lex.c"

int minish_yyerror(const char *s) {
	fprintf(stdout,"minish.yacc: %s at '%s'\n", s, yytext);
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
