%x load

%{

#include "y.tab.h"
#include "syslog.h"
#include <stdbool.h>

void show_buffer(char*);
int nbuf = 0;
int s_line = 0;
FILE *fp;
bool INTERACTIV = true;

%}

%option noyywrap noinput nounput

%%
\.    BEGIN(load);
<load>[ \t]*      /* eat the whitespace */
<load>[^ \t\n]+   {
                     syslog (LOG_NOTICE, "Opening input file %s\n", yytext);
                     fp = fopen(yytext, "r" );
		     if (! fp){ syslog(LOG_NOTICE,"Cannot open input file %s\n", yytext); }
		     else
		     {
		       yyin=fp;
		       INTERACTIV = false;
		       yypush_buffer_state(yy_create_buffer( yyin, YY_BUF_SIZE ));
		     }

		     BEGIN(INITIAL);
                  }
<<EOF>>           {
                     if (! INTERACTIV)
		        { yypop_buffer_state(); }
		     else
		        { yyterminate(); }

		     INTERACTIV=true;
	             syslog(LOG_NOTICE, "Closing input file\n");  
                     fclose(fp);
                  }

[ \t ]+                         { show_buffer(yytext); syslog(LOG_NOTICE, "Ignoring whitespace\n"); } /* ignore whitespace */
#.*$                            { show_buffer(yytext); syslog(LOG_NOTICE, "Comment found\n"); } /* ignore comment */
"line"|"lineto"|"rectangle"|"circle"|"ellipse"|"moveto"|"floodfill"|"MAX" { show_buffer(yytext); yylval.cname = strdup(yytext); syslog(LOG_NOTICE, "Returning CNAME, buffer[%i]\n",nbuf++); return CNAME; }

"set"                           { show_buffer(yytext); yylval.set = strdup(yytext); syslog(LOG_NOTICE, "Returning SET, buffer[%i]\n", nbuf++); return SET; }
"v_background"|"v_foreground"|"v_line_width"            { show_buffer(yytext); yylval.spara = strdup(yytext); syslog(LOG_NOTICE, "Returning SPARA, buffer[%i]\n", nbuf++); return SPARA; }

"exit"|"quit"|"list"|"init"|"clear"|"show"|"close"|"state"                  { show_buffer(yytext); yylval.ctrl = strdup(yytext); syslog(LOG_NOTICE, "Control command received\n"); return CTRL; } 
"colors"|"next"|"prev"                        { show_buffer(yytext); yylval.cpara = strdup(yytext); syslog(LOG_NOTICE, "Returning CPARA, buffer[%i]\n", nbuf++); return CPARA; }
[A-Za-z]+                       { show_buffer(yytext); yylval.oname = strdup(yytext); syslog(LOG_NOTICE, "Returning ONAME, buffer[%i]\n", nbuf++); return ONAME; }
[0-9]+                          { show_buffer(yytext); yylval.integer=atoi(yytext); syslog(LOG_NOTICE, "Returning NUMBER, buffer[%i]\n", nbuf++); return NUMBER; }
[0-9]+\.?|[0-9]*\.[0-9]+        { show_buffer(yytext); sscanf(yytext,"%lf",&yylval.real); syslog(LOG_NOTICE, "Returning REAL, buffer[%i]\n", nbuf++); return REAL; }
['+''\-''*''/''=']              { show_buffer(yytext); syslog(LOG_NOTICE, "Returning OPERATOR, buffer[%i]\n", nbuf++); return yytext[0]; }
,                               { show_buffer(yytext); yylval.character=yytext[0]; syslog(LOG_NOTICE, "Returning COMMA, buffer[%i]\n", nbuf++); return COMMA;}
\(                              { show_buffer(yytext); yylval.character=yytext[0]; syslog(LOG_NOTICE, "Returning LPAREN, buffer[%i]\n", nbuf++); return LPAREN; }
\)                              { show_buffer(yytext); yylval.character=yytext[0]; syslog(LOG_NOTICE, "Returning RPAREN, buffer[%i]\n", nbuf++); return RPAREN; }
\{                              { show_buffer(yytext); yylval.character=yytext[0]; syslog(LOG_NOTICE, "Returning LBRACE, buffer[%i]\n", nbuf++); return LBRACE; }
\}                              { show_buffer(yytext); yylval.character=yytext[0]; syslog(LOG_NOTICE, "Returning RBRACE, buffer[%i]\n", nbuf++); return RBRACE; }
;                               { show_buffer(yytext); yylval.character=yytext[0]; syslog(LOG_NOTICE, "Returning SEMICOLON, buffer[%i]\n", nbuf++); return SEMICOLON; }
.                               { show_buffer(yytext); syslog(LOG_NOTICE, "Ignoring character: %s\n", yytext); }  /* ignore any other character */
\n                              {  if (INTERACTIV) { printf("[%i] > ",s_line); s_line++;} }
%%

void show_buffer(char* str)
{
  /*
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
  */    
}
