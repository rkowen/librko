/* minish.yacc - grammar for minish program
 */
/* declarations */
%token COMMAND NUMBER WORD EOC EOL
%token AMPER_AMPER BAR_BAR
%token GREATER_AMPER GREATER_GREATER GREATER_THAN LESS_AMPER LESS_THAN

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
char **ptr;

uvec statement;
%}

%start statements

%%
/* grammar rules */

statements
	: /* empty */
	| statement
	| statements statement
	;

statement : command options eoc
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
			if (uvec_ctor(&statement,10))
				rkoperror("minish-yacc:ctor:");
			if (uvec_add(&statement,yytext))
				rkoperror("minish-yacc:com:");
		}
	;

options :	/* empty options */
	| options redirection
	| options WORD
		{
#ifdef YACCTEST
			printf("options:%s\n", yytext);
			if (uvec_add(&statement,yytext))
				rkoperror("minish-yacc:opts:");
#endif
		}
	;

redirection
	: NUMBER redirect file
		{
			fdnum = $1;
#ifdef YACCTEST
			printf("redirect:%d:%s\n", fdnum, yytext);
#endif
		}
	| redirect file
		{
			fdnum = 1;
#ifdef YACCTEST
			printf("redirect:stdin/out:%s\n", yytext);
#endif
		}
	;

file	: WORD
		{
#ifdef YACCTEST
			printf("file:%s\n", yytext);
#endif
			if (uvec_add(&statement,yytext))
				rkoperror("minish-yacc:file:");
		}

redirect : GREATER_AMPER
		{
#ifdef YACCTEST
			printf("redirect:%s\n", yytext);
			if (uvec_add(&statement,yytext))
				rkoperror("minish-yacc:red:");
#endif
		}
	| LESS_AMPER
		{
#ifdef YACCTEST
			printf("redirect:%s\n", yytext);
			if (uvec_add(&statement,yytext))
				rkoperror("minish-yacc:red:");
#endif
		}
	| GREATER_THAN
		{
#ifdef YACCTEST
			printf("redirect:%s\n", yytext);
			if (uvec_add(&statement,yytext))
				rkoperror("minish-yacc:red:");
#endif
		}
	| GREATER_GREATER
		{
#ifdef YACCTEST
			printf("redirect:%s\n", yytext);
			if (uvec_add(&statement,yytext))
				rkoperror("minish-yacc:red:");
#endif
		}
	| LESS_THAN
		{
#ifdef YACCTEST
			printf("redirect:%s\n", yytext);
#endif
			if (uvec_add(&statement,yytext))
				rkoperror("minish-yacc:redchar:");
		}
	;

eoc	: eol
	| EOC
		{
#ifdef YACCTEST
			printf("eoc:%s\n", yytext);
#endif
			if (uvec_add(&statement,yytext))
				rkoperror("minish-yacc:eoc:");
			for (ptr = uvec_vector(&statement);
				*ptr != (char *) NULL; ++ptr) {
				printf("%s ", *ptr);
			}
			printf("\n");
			if (uvec_dtor(&statement))
				rkoperror("minish-yacc:dtor:");
		}
	| BAR_BAR
		{
#ifdef YACCTEST
			printf("eoc:%s\n", yytext);
#endif
			if (uvec_add(&statement,yytext))
				rkoperror("minish-yacc:eoc:");
			for (ptr = uvec_vector(&statement);
				*ptr != (char *) NULL; ++ptr) {
				printf("%s ", *ptr);
			}
			printf("\n");
			if (uvec_dtor(&statement))
				rkoperror("minish-yacc:dtor:");
		}
	| AMPER_AMPER
		{
#ifdef YACCTEST
			printf("eoc:%s\n", yytext);
#endif
			if (uvec_add(&statement,yytext))
				rkoperror("minish-yacc:eoc:");
			for (ptr = uvec_vector(&statement);
				*ptr != (char *) NULL; ++ptr) {
				printf("%s ", *ptr);
			}
			printf("\n");
			if (uvec_dtor(&statement))
				rkoperror("minish-yacc:dtor:");
		}
	;
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
				rkoperror("minish-yacc:dtor:");
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
