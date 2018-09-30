%{

#include "y.tab.h"
void show(char*);

%}

%option noyywrap noinput nounput

%%

[ \t \n]+                       { show(yytext); }  /* ignore whitespace */
[0-9]+\.?|[0-9]*\.[0-9]+        { show(yytext); sscanf(yytext,"%lf",&yylval.real); return NUMBER; }
["+" "-" "*" "/"]               { show(yytext); return yytext[0]; }
;                               { show(yytext); return 0; }

%%

void show(char* str)
{
    int j = strlen(str);
    for(int i=0; i<j; i++)
	{
	    printf("[lex] yytext.length: %d, yytext[%d]: %i --> %c\n",j,i,str[i],str[i]);
	}
}
