%{
/* mini-sh definitions */
#ifdef LEXTEST

#  define _MAIN_
#  define RETURN(a)	\
	printf(": " #a " :"); \
	puts(minish_yytext);
int minish_yylval;

#else
#  define RETURN(a)	\
	return(a);
extern int minish_yylval;
#  ifdef FLEX_STRING
#    undef YY_INPUT
#    define YY_INPUT(b, r, ms)	(r = minish_string_yyinput(b, ms))
#  endif
#endif

#include "minish-tab.h"
#include <stdlib.h>
#ifdef MEMDEBUG
#  include "memdebug.h"
#endif

/* WORD		= anything that should be passed as a single entity
 * NUMBER	= a filedescriptor 0=stdin, 1=stdout, 2=stderr, etc.
 * WS		= eat any unnecessary white space
 */
%}
NUMBER	[0-9]+
WORD	[^ 	;&<>|#'"\n]+
WS	[ \t]*

%s	START_COMMAND OPT START_COMMENT START_COMMENT_ONLY
%s	START_SQUOTE END_SQUOTE START_DQUOTE END_DQUOTE
%s	REDI REDI_NUM

%%
	/* mini-sh rules */

<INITIAL,OPT>;	{
			BEGIN START_COMMAND;
			RETURN(EOC);
		}
<OPT>"||"	{
			BEGIN START_COMMAND;
			RETURN(BAR_BAR);
		}
<OPT>"&&"	{
			BEGIN START_COMMAND;
			RETURN(AMPER_AMPER);
		}
<INITIAL,START_COMMAND>{WORD}	{
			BEGIN OPT;
			RETURN(COMMAND);
		}
<OPT>{NUMBER}/">"	{
			/* number is only a NUMBER if followed by > */
			/* must be a redirection */
			minish_yylval = atoi(minish_yytext);
			BEGIN REDI;
			RETURN(NUMBER);
		}
<OPT>{NUMBER}/"<"	{
			/* number is only a NUMBER if followed by < */
			/* must be a redirection */
			minish_yylval = atoi(minish_yytext);
			BEGIN REDI;
			RETURN(NUMBER);
		}
<REDI_NUM>{NUMBER}	{
			/* have redirection with trialing NUMBER */
			minish_yylval = atoi(minish_yytext);
			BEGIN OPT;
			RETURN(NUMBER);
		}
<REDI>">&"/{NUMBER}	{
			/* have full redirection ... get NUMBER next */
			BEGIN REDI_NUM;
			RETURN(GREATER_AMPER);
		}
<OPT,REDI>">"	{
			BEGIN OPT;
			RETURN(GREATER_THAN);
		}
<OPT,REDI>">>"	{
			BEGIN OPT;
			RETURN(GREATER_GREATER);
		}
<OPT,REDI>"<"	{
			BEGIN OPT;
			RETURN(LESS_THAN);
		}
<OPT,REDI>"<>"	{
			BEGIN OPT;
			RETURN(LESS_GREATER);
		}
<REDI>">&"	{
			/* this means to close off this redirction - an error */
			BEGIN OPT;
			RETURN(GREATER_AMPER_CLOSE);
		}
<OPT>">&"	{
			/* close off stdout */
			RETURN(GREATER_AMPER_CLOSE);
		}
<OPT>"<&"	{
			/* close off stdin */
			RETURN(LESS_AMPER);
		}
<OPT>'		{
			BEGIN START_SQUOTE;
		}
<START_SQUOTE>[^']*	{
			if (minish_yytext[minish_yyleng-1] == '\\') {
				minish_yytext[minish_yyleng-1]='\'';
				/* handle escaped ' */
				yyless(minish_yyleng);		/* return string */
				yymore();		/* append next string */
			} else {
				BEGIN END_SQUOTE;
				RETURN(WORD);
			}
		}
<END_SQUOTE>'	{
			/* eat closing ' */
			/* minish_yyleng--;
			minish_yytext[minish_yyleng]='\0'; */
			BEGIN OPT;
		}
<OPT>\"		{
			BEGIN START_DQUOTE;
		}
<START_DQUOTE>[^\"]*	{
			if (minish_yytext[minish_yyleng-2] == '\\') {
				/* handle escaped " */
				yyless(minish_yyleng-1);	/* return last " */
				yymore();		/* append next string */
			} else {
				BEGIN END_DQUOTE;
				RETURN(WORD);
			}
		}
<END_DQUOTE>\"	{
			/* eat closing " */
			/* minish_yyleng--;
			minish_yytext[minish_yyleng]='\0'; */
			BEGIN OPT;
		}
<INITIAL>"#"	{
			BEGIN START_COMMENT_ONLY;
		}
<OPT>"#"	{
			BEGIN START_COMMENT;
		}
<START_COMMENT,START_COMMENT_ONLY>.*	{
			/* eat rest of comments */
		}
<OPT>{WORD}		{
			RETURN(WORD);
		}
^{WS}\n		{	/* eat blank lines */
			BEGIN 0;
		}
<START_COMMENT_ONLY>\n	{	/* eat comment only lines */
			BEGIN 0;
		}
\n		{
			BEGIN 0;
			RETURN(EOL);
		}
{WS}		{	/* eat white space */	
		}

%%
/* mini-sh routines */

#ifdef LEXTEST

int minish_yywrap(void) {
	fprintf(stdout, "mini-sh.lex: end of input\n");
	return(1);
}

int main(void) {
	FILE *testfile;
	if (!(testfile = freopen("lextest.dat","r",stdin))) {
		fprintf(stderr,"Can't open lextest.dat file for testing\n");
		return 1;
	}
	while(minish_yylex());
	return 0;
}

#else

int minish_yywrap(void) {
	return(1);
}

#endif
