%{
/* mini-sh definitions */
#ifdef LEXTEST

#  define _MAIN_
#  define RETURN(a)	\
	printf(": " #a " :"); \
	puts(yytext);
int yylval;

#else
#  define RETURN(a)	\
	return(a);
extern int yylval;
#endif

#include "y.tab.h"
#include <stdlib.h>

/* WORD		= anything that should be passed as a single entity
 * NUMBER	= a filedescriptor 0=stdin, 1=stdout, 2=stderr, etc.
 * WS		= eat any unnecessary white space
 */
%}
NUMBER	[0-9]+
WORD	[^ 	;&<>|#'"\n]+
WS	[ \t]*

%s	START_COMMAND OPT START_COMMENT
%s	START_SQUOTE END_SQUOTE START_DQUOTE END_DQUOTE
%s	REDI REDI_NUM

%%
	/* mini-sh rules */

;		{
			BEGIN START_COMMAND;
			RETURN(EOC);
		}
"||"		{
			BEGIN START_COMMAND;
			RETURN(BAR_BAR);
		}
"&&"		{
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
			yylval = atoi(yytext);
			BEGIN REDI;
			RETURN(NUMBER);
		}
<OPT>{NUMBER}/"<"	{
			/* number is only a NUMBER if followed by < */
			/* must be a redirection */
			yylval = atoi(yytext);
			BEGIN REDI;
			RETURN(NUMBER);
		}
<REDI_NUM>{NUMBER}	{
			/* have redirection with trialing NUMBER */
			yylval = atoi(yytext);
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
<REDI>">&"	{
			/* this means to close off this redirction - an error */
			BEGIN OPT;
			RETURN(GREATER_AMPER);
		}
<OPT>">&"	{
			/* close off stdout */
			RETURN(GREATER_AMPER);
		}
<OPT>"<&"	{
			/* close off stdin */
			RETURN(LESS_AMPER);
		}
<OPT>'		{
			BEGIN START_SQUOTE;
		}
<START_SQUOTE>[^']*	{
			if (yytext[yyleng-1] == '\\') {
				yytext[yyleng-1]='\'';
				/* handle escaped ' */
				yyless(yyleng);		/* return string */
				yymore();		/* append next string */
			} else {
				BEGIN END_SQUOTE;
				RETURN(WORD);
			}
		}
<END_SQUOTE>'	{
			/* eat closing ' */
			BEGIN OPT;
		}
<OPT>\"		{
			BEGIN START_DQUOTE;
		}
<START_DQUOTE>[^\"]*	{
			if (yytext[yyleng-2] == '\\') {
				/* handle escaped " */
				yyless(yyleng-1);	/* return last " */
				yymore();		/* append next string */
			} else {
				BEGIN END_DQUOTE;
				RETURN(WORD);
			}
		}
<END_DQUOTE>\"	{
			/* eat closing " */
			BEGIN OPT;
		}
<OPT>"#"	{
			BEGIN START_COMMENT;
		}
<START_COMMENT>.*	{
			/* eat rest of comments */
		}
<OPT>{WORD}		{
			RETURN(WORD);
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

int yywrap(void) {
	fprintf(stdout, "mini-sh.lex: end of input\n");
	return(1);
}

int main(void) {
	FILE *testfile;
	if (!(testfile = freopen("lextest.dat","r",stdin))) {
		fprintf(stderr,"Can't open lextest.dat file for testing\n");
		return 1;
	}
	while(yylex());
	return 0;
}

#else

int yywrap(void) {
	return(1);
}

#endif
