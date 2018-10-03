%{

#include "y.tab.h"
#include "syslog.h"
void show(char*);

int nbuf = 0;

%}

%option noyywrap noinput nounput

%%

[ \t \n]+                       { show(yytext); syslog(LOG_NOTICE, "Ignoring whitespace\n"); }  /* ignore whitespace */
[0-9]+\.?|[0-9]*\.[0-9]+        { show(yytext); sscanf(yytext,"%lf",&yylval.real); syslog(LOG_NOTICE, "Returning NUMBER, buffer[%i]\n", nbuf++); return NUMBER; }
["+" "-" "*" "/"]               { show(yytext); syslog(LOG_NOTICE, "Returning OPERATOR, buffer[%i]\n", nbuf++); return yytext[0]; }
;                               { show(yytext); syslog(LOG_NOTICE, "Returning SEMICOLON, buffer[%i]\n", nbuf++); }
.                               { show(yytext); syslog(LOG_NOTICE, "Ignoring character: %s\n", yytext); }  /* ignore any other character */

%%

void show(char* str)
{
    int j = strlen(str);
    for(int i=0; i<j; i++)
	{
	    if (str[i] != '\n')
		syslog(LOG_NOTICE,"[lex] yytext.length: %d, yytext[%d]: %i --> %c\n",j,i,str[i],str[i]);
	    else
		syslog(LOG_NOTICE,"[lex] Found LF\n");
	}
}
