/* usched.y grammar for the usched program
 */
/* declarations */
%token MONTHS DAYS NUMBER STRING
%token EOL				/* end-of-line */
%token MONTH YEAR WEEKEND HOLIDAY
%token NAME CONSTRAINTS MEETINGS VACATION PREFERENCE
%token TIME TIMES

%{
#ifdef YACCTEST
#  define _MAIN_
#else
int i;
#endif

#include <stdio.h>
#include <stdlib.h>
#include "usched.h"
%}

%left ','
%nonassoc '-'
%nonassoc '&'				/* highest precedent */

%start lines

%%
/* grammar rules */

lines	: /* empty lines */
	| lines line
	;

line	:
	  year EOL
	| month EOL
	| weekend EOL
	| holiday EOL
	| group EOL
	| error EOL
		{
			yyerrok;
			printf("Error in line %d, statement %d\n",
				linenum, statnum+1);
		}
	  line
		{	$$ = $4;	}
	;

year	: YEAR '=' NUMBER
	{
#ifdef YACCTEST
		printf("Year = %d\n", $3);
#else
		usyear = $3;
		if (usmonth) usinitcal(usmonth, usyear, &monthday1,
				&maxday, uscal);
#endif
	}
	;

month	: MONTH '=' NUMBER
	{
#ifdef YACCTEST
		printf("Month = %d\n", $3);
#else
		usmonth = $3;
		if (usyear) usinitcal(usmonth, usyear, &monthday1,
				&maxday, uscal);
#endif
	}
	| MONTH '=' MONTHS
	{
#ifdef YACCTEST
		printf("Month = %d\n", $3);
#else
		usmonth = $3;
		if (usyear) usinitcal(usmonth, usyear, &monthday1,
				&maxday, uscal);
#endif
	}
	;

weekend	: WEEKEND '=' dates
	{
#ifdef YACCTEST
		printf("Weekend =\n");
#else
		usyaccdates(_Weekend, uscal);
#endif
	}
	;

holiday	: HOLIDAY '=' dates
	{
#ifdef YACCTEST
		printf("holiday = %d\n", $3);
#else
		usyaccdates(_Holiday, uscal);
#endif
	}
	;

group	: /* empty */
	| group individual
	;

individual :
	  '{'
	{
#ifndef YACCTEST
		/* allocate space for new consultant */
		if (!(roster[maxroster] = (Consultant *) malloc(
			sizeof(Consultant)))) {
			yyerror("usched error: not enough memory"
				" to add consultant");
		}
		/* fill consultant with group calendar */
		for (i = 0; i < MAXDAY; ++i) 
			roster[maxroster]->calendar[i] = uscal[i];
#endif
	}
	  dataset '}'
	{
		maxroster++;
	}
	;

dataset	: /* empty */
	| dataset data
	;

data	: name EOL
	| constraints EOL
	| meetings EOL
	| vacation EOL
	| preference EOL
	| time EOL
	| EOL
	;

name	: NAME '=' STRING
	{
#ifdef YACCTEST
		printf("Name = %s\n", yytext);
#else
		/* strip off "'s and put into strbuff */
		strncpy(roster[maxroster]->name ,&(yytext[1]),
		(yyleng-1<MAXNAME?yyleng-2:MAXNAME));
#endif
	}
	;

constraints :
	  CONSTRAINTS '=' dates
	{
#ifdef YACCTEST
		printf(":Constraints=:\n");
#else
		usyaccdates(_Constraint, roster[maxroster]->calendar);
#endif
	}
	;

meetings :
	  MEETINGS '=' dates
	{
#ifdef YACCTEST
		printf(":Meetings=:\n");
#else
		usyaccdates(_Meeting, roster[maxroster]->calendar);
#endif
	}
	;

vacation :
	  VACATION '=' dates
	{
#ifdef YACCTEST
		printf(":Vacation=:\n");
#else
		usyaccdates(_Vacation, roster[maxroster]->calendar);
#endif
	}
	;

preference :
	  PREFERENCE '=' dates
	{
#ifdef YACCTEST
		printf(":Preference=:\n");
#else
		usyaccdates(_Prefer, roster[maxroster]->calendar);
#endif
	}
	;

time :
	  TIME '=' TIMES
	{
#ifdef YACCTEST
		printf(":time=%d:\n",$3);
#else
		roster[maxroster]->time = $3;
#endif
	}
	| TIMES
	{
#ifdef YACCTEST
		printf(":time=%d:\n",$1);
#else
		roster[maxroster]->time = $1;
#endif
	}
	;

dates	: /* empty */
	| dates ',' numbers
	| dates ',' days
	| numbers
	| days
	;

numbers	: NUMBER '-' NUMBER
	{
#ifdef YACCTEST
		printf(":%d:-:%d:\n",$1,$3);
#else
		for (i=$1; i <= $3; ++i) uspush(-i);
#endif
	}
	| NUMBER
	{
#ifdef YACCTEST
		printf(":%d:\n", $1);
#else
		uspush(-($1));
#endif
	}
	;

days	: DAYS '-' DAYS
	{
#ifdef YACCTEST
		printf(":%d:-:%d:\n",$1,$3);
#else
		for (i=$1; i <= $3; ++i) uspush(i);
#endif
	}
	| DAYS
	{
#ifdef YACCTEST
		printf(":%d:\n", $1);
#else
		uspush($1);
#endif
	}
	;

%% /* start of programs */

#undef input()		/* eliminates a useless warning */
#include "lex.yy.c"

int yyerror(const char *s) {
	fprintf(stderr,"usched.yacc: %s\n", s);
}

#ifndef YACCTEST
void usyaccdates(Calendar flag, Calendar *cal) {
	extern Weekdays monthday1;
	extern int maxday;
	int i;

		while((i=usnum()) > 0 && i != MAXDAY) {
			if ((i = uspop()) < 0) {
				if (cal[-i] < flag) cal[-i] = flag;
			} else {
				usdaycal(monthday1, maxday, i,
					flag, cal);
			}
		}
		usclear();
}
#endif

#ifdef YACCTEST
int main() {
#if 0
	extern int yydebug;
	yydebug = 1;
#endif
	return yyparse();
}
#endif /* YACCTEST */
