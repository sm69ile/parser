%{

#include "y.tab.h"
#include "syslog.h"
void show(char*);

int nbuf = 0;
int s_line = 0;

%}

%option noyywrap noinput nounput

%%
#.*$                         { show(yytext); syslog(LOG_NOTICE, "Comment found\n"); } /* ignore comment */
[ \t ]+                       { show(yytext); syslog(LOG_NOTICE, "Ignoring whitespace\n"); } /* ignore whitespace */
"line"|"circle"|"arc"           { show(yytext); yylval.cname = yytext; syslog(LOG_NOTICE, "Returning CNAME, buffer[%i]\n", nbuf++); return CNAME; }
[A-Za-z]+                       { show(yytext); yylval.identifier = yytext; syslog(LOG_NOTICE, "Returning IDENTIFIER, buffer[%i]\n", nbuf++); return IDENTIFIER; }
[0-9]+                          { show(yytext); yylval.integer=atoi(yytext); syslog(LOG_NOTICE, "Returning NUMBER, buffer[%i]\n", nbuf++); return NUMBER; } 
['+''\-''*''/''=']              { show(yytext); syslog(LOG_NOTICE, "Returning OPERATOR, buffer[%i]\n", nbuf++); return yytext[0]; }
,                               { show(yytext); yylval.character=yytext[0]; syslog(LOG_NOTICE, "Returning COMMA, buffer[%i]\n", nbuf++); return COMMA;}
\(                              { show(yytext); yylval.character=yytext[0]; syslog(LOG_NOTICE, "Returning LPAREN, buffer[%i]\n", nbuf++); return LPAREN; }
\)                              { show(yytext); yylval.character=yytext[0]; syslog(LOG_NOTICE, "Returning RPAREN, buffer[%i]\n", nbuf++); return RPAREN; }
\{                              { show(yytext); yylval.character=yytext[0]; syslog(LOG_NOTICE, "Returning LBRACE, buffer[%i]\n", nbuf++); return LBRACE; }
\}                              { show(yytext); yylval.character=yytext[0]; syslog(LOG_NOTICE, "Returning RBRACE, buffer[%i]\n", nbuf++); return RBRACE; }
;                               { show(yytext); yylval.character=yytext[0]; syslog(LOG_NOTICE, "Returning SEMICOLON, buffer[%i]\n", nbuf++); return SEMICOLON; }
.                               { show(yytext); syslog(LOG_NOTICE, "Ignoring character: %s\n", yytext); }  /* ignore any other character */
\n                              { s_line++; }
%%

void show(char* str)
{
    syslog(LOG_NOTICE,"[lex] new buffer");
    int j = strlen(str);

    for(int i=0; i<j; i++)
	{
	    if (str[i] != '\n')
		syslog(LOG_NOTICE,"[lex] yytext.length: %d, yytext[%d]: %i --> %c\n",j,i,str[i],str[i]);
	    else
		syslog(LOG_NOTICE,"[lex] Found LF\n");
	}
    syslog(LOG_NOTICE,"[lex] end buffer");
}
