/* minish.yacc - grammar for minish program
 */
/* declarations */
%token COMMAND EOC EOL
%token AMPER_AMPER BAR_BAR
%token GREATER_AMPER_CLOSE GREATER_AMPER GREATER_GREATER GREATER_THAN
%token LESS_AMPER LESS_THAN
%token NUMBER WORD

%left EOC BAR_BAR AMPER_AMPER

%{
#ifdef YACCTEST
#  define _MAIN_
#else
int i;
#endif

#include <stdio.h>
#include "librko.h"

int linenum = 0;
int statnum = 0;
int fdnum = 0;
int fdnum2 = 0;
char **ptr;
char buffer[20];

uvec statement;
uvec options;

void dbgstop(void) {
	static int a=1;
}

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
			printf("command:%s\n", yytext);
#endif
			if (uvec_ctor(&options,10))
				rkoperror("minish-yacc:o:ctor:");
			if (! uvec_exists(&statement)) {
				if (uvec_ctor(&statement,10))
					rkoperror("minish-yacc:s:ctor:");
			}
			if (uvec_add(&statement,yytext))
				rkoperror("minish-yacc:com:");
		}
	;

options :	/* empty options */
	| options redirection
	| options word
		{
#ifdef YACCTEST
			printf("option:%s\n", yytext);
#endif
			if (uvec_add(&options,yytext))
				rkoperror("minish-yacc:option:");
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
			printf("redirect:stdin/out:%s\n", yytext);
#endif
		}
	| number redirect file
		{
			fdnum = $1;
#ifdef YACCTEST
			printf("redirect:%d:%s\n", fdnum, yytext);
#endif
		}
	;

redirect: GREATER_THAN
		{
			fdnum = 1;	/* stdout */
#ifdef YACCTEST
			printf("redirect:%s\n", yytext);
#endif
			if (uvec_add(&statement,yytext))
				rkoperror("minish-yacc:red:stdout");
		}
	| GREATER_GREATER
		{
			fdnum = 1;	/* stdout */
#ifdef YACCTEST
			printf("redirect:%s\n", yytext);
#endif
			if (uvec_add(&statement,yytext))
				rkoperror("minish-yacc:app:");
		}
	| LESS_THAN
		{
			fdnum = 0;	/* stdin */
#ifdef YACCTEST
			printf("redirect:%s\n", yytext);
#endif
			if (uvec_add(&statement,yytext))
				rkoperror("minish-yacc:red:");
		}
	;

redir_fd: number redir_ga number
		{
			fdnum = $1;
			fdnum2 = $3;
#ifdef YACCTEST
			printf("redirect:%d to %d\n", fdnum, fdnum2);
#endif
		}

redir_ga
	: GREATER_AMPER
		{
			if (uvec_add(&statement,yytext))
				rkoperror("minish-yacc:fd-red:");
		}

redir_close
	: LESS_AMPER
		{
			fdnum = 0;
#ifdef YACCTEST
			printf("close:%d:\n", fdnum);
#endif
			if (uvec_add(&statement,yytext))
				rkoperror("minish-yacc:close:stdin");
		}
	| GREATER_AMPER_CLOSE
		{
			fdnum = 1;
#ifdef YACCTEST
			printf("close:%d:\n", fdnum);
#endif
			if (uvec_add(&statement,yytext))
				rkoperror("minish-yacc:close:stdout");
		}
	| number GREATER_AMPER_CLOSE
		{
			fdnum = $1;
#ifdef YACCTEST
			printf("close:%d:\n", fdnum);
#endif
			if (uvec_add(&statement,yytext))
				rkoperror("minish-yacc:close:fd");
		}
	;

file	: word
		{
#ifdef YACCTEST
			printf("file:%s\n", yytext);
#endif
		}


eoc	: eol
	| EOC
		{
#ifdef YACCTEST
			printf("eoc:%s\n", yytext);
#endif
			if (uvec_add(&statement,yytext))
				rkoperror("minish-yacc:eoc:");

			printf("options: ");
			for (ptr = uvec_vector(&options);
				*ptr != (char *) NULL; ++ptr) {
				printf("%s ", *ptr);
			}
			printf("\n");
			if (uvec_dtor(&options))
				rkoperror("minish-yacc:o:dtor:");
		}
	| BAR_BAR
		{
#ifdef YACCTEST
			printf("eoc:%s\n", yytext);
#endif
			if (uvec_add(&statement,yytext))
				rkoperror("minish-yacc:eoc:");

			printf("options: ");
			for (ptr = uvec_vector(&options);
				*ptr != (char *) NULL; ++ptr) {
				printf("%s ", *ptr);
			}
			printf("\n");
			if (uvec_dtor(&options))
				rkoperror("minish-yacc:o:dtor:");
		}
	| AMPER_AMPER
		{
#ifdef YACCTEST
			printf("eoc:%s\n", yytext);
#endif
			if (uvec_add(&statement,yytext))
				rkoperror("minish-yacc:eoc:");

			printf("options: ");
			for (ptr = uvec_vector(&options);
				*ptr != (char *) NULL; ++ptr) {
				printf("%s ", *ptr);
			}
			printf("\n");
			if (uvec_dtor(&options))
				rkoperror("minish-yacc:o:dtor:");
		}
	;

number	: NUMBER
		{
			$$ = $1;
#ifdef YACCTEST
			printf("number:%s\n", yytext);
			strcpy(buffer, "<");
			strcat(buffer, yytext);
			strcat(buffer, ">");
			if (uvec_add(&statement,buffer))
				rkoperror("minish-yacc:number:");
#endif
		}

word	: WORD
		{
#ifdef YACCTEST
			printf("word:%s\n", yytext);
			if (uvec_add(&statement,yytext))
				rkoperror("minish-yacc:word:");
#endif
		}

eol	: EOL
		{
#ifdef YACCTEST
			printf("eol: linenum = %d\n", ++linenum);
#endif
			if (uvec_add(&statement,yytext))
				rkoperror("minish-yacc:eol:");
			for (ptr = uvec_vector(&statement);
				*ptr != (char *) NULL; ++ptr) {
				printf("%s ", *ptr);
			}
			printf("\n");

			if (uvec_dtor(&statement))
				rkoperror("minish-yacc:s:dtor:");
			if (uvec_dtor(&options))
				rkoperror("minish-yacc:o:dtor:");
		}
	;

%% /* start of programs */

#undef input()		/* eliminates a useless warning */
#include "lex.yy.c"

int yyerror(const char *s) {
	fprintf(stderr,"minish.yacc: %s\n", s);
}

#ifdef YACCTEST
/* mini-sh routines */

int main(void) {
	FILE *testfile;
#if 0
	extern int yydebug;
	yydebug = 1;
#endif
	if (!(testfile = freopen("lextest.dat","r",stdin))) {
		fprintf(stderr,"Can't open lextest.dat file for testing\n");
		return 1;
	}
	return yyparse();
	return 0;
}
#endif /* YACCTEST */
