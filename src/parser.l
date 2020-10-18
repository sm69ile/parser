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
                     syslog (LOG_DEBUG, "Opening input file %s\n", yytext);
                     fp = fopen(yytext, "r" );
		     if (! fp){ syslog(LOG_DEBUG,"Cannot open input file %s\n", yytext); }
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
	             syslog(LOG_DEBUG, "Closing input file\n");  
                     fclose(fp);
                  }

[ \t ]+                         { show_buffer(yytext); syslog(LOG_DEBUG, "Ignoring whitespace\n"); } /* ignore whitespace */
#.*$                            { show_buffer(yytext); syslog(LOG_DEBUG, "Comment found\n"); } /* ignore comment */

"line"|"lineto"|"rectangle"|"circle"|"ellipse"|"arc"|"moveto"|"floodfill"|"flowways"|"MAX"|"REM"|"state"|"break"  { show_buffer(yytext); yylval.cname = strdup(yytext); syslog(LOG_DEBUG, "Returning CNAME, buffer[%i]\n",nbuf++ ); return CNAME; }

"set" { show_buffer(yytext); yylval.set = strdup(yytext); syslog(LOG_DEBUG, "Returning SET, buffer[%i]\n", nbuf++); return SET; }

"v_background"|"v_foreground"|"v_line_width" { show_buffer(yytext); yylval.spara = strdup(yytext); syslog(LOG_DEBUG, "Returning SPARA, buffer[%i]\n", nbuf++); return SPARA; }

"exit"|"quit"|"list"|"init"|"clear"|"show"|"status"|"nobj"|"pobj"|"lobj"|"nstatus"|"pstatus"|"sleep"|"save"|"hide"|"display"|"object"|"command"|"redraw"|"help"   {  show_buffer(yytext);  yylval.ctrl = strdup(yytext);  syslog(LOG_DEBUG, "Control command received: %s, buffer[%i]\n",yytext, nbuf++);  return CTRL;  } 

"all"|"prev"|"next"|"last"|"act"|"count"                       {  show_buffer(yytext); yylval.cpara = strdup(yytext);  syslog(LOG_DEBUG, "Control parameter received: %s, buffer[%i]\n", yytext, nbuf++);  return CPARA; }


[A-Za-z][A-Za-z0-9]*                       { show_buffer(yytext); yylval.oname = strdup(yytext); syslog(LOG_DEBUG, "Returning ONAME, buffer[%i]\n", nbuf++); return ONAME; }
[0-9]+                          { show_buffer(yytext); yylval.integer=atoi(yytext); syslog(LOG_DEBUG, "Returning NUMBER, buffer[%i]\n", nbuf++); return NUMBER; }
[0-9]+\.?|[0-9]*\.[0-9]+        { show_buffer(yytext); sscanf(yytext,"%lf",&yylval.real); syslog(LOG_DEBUG, "Returning REAL, buffer[%i]\n", nbuf++); return REAL; }
['+''\-''*''/''=']              { show_buffer(yytext); syslog(LOG_DEBUG, "Returning OPERATOR, buffer[%i]\n", nbuf++); return yytext[0]; }
,                               { show_buffer(yytext); yylval.character=yytext[0]; syslog(LOG_DEBUG, "Returning COMMA, buffer[%i]\n", nbuf++); return COMMA;}
\(                              { show_buffer(yytext); yylval.character=yytext[0]; syslog(LOG_DEBUG, "Returning LPAREN, buffer[%i]\n", nbuf++); return LPAREN; }
\)                              { show_buffer(yytext); yylval.character=yytext[0]; syslog(LOG_DEBUG, "Returning RPAREN, buffer[%i]\n", nbuf++); return RPAREN; }
\{                              { show_buffer(yytext); yylval.character=yytext[0]; syslog(LOG_DEBUG, "Returning LBRACE, buffer[%i]\n", nbuf++); return LBRACE; }
\}                              { show_buffer(yytext); yylval.character=yytext[0]; syslog(LOG_DEBUG, "Returning RBRACE, buffer[%i]\n", nbuf++); return RBRACE; }
;                               { show_buffer(yytext); yylval.character=yytext[0]; syslog(LOG_DEBUG, "Returning SEMICOLON, buffer[%i]\n", nbuf++); return SEMICOLON; }
.                               { show_buffer(yytext); syslog(LOG_DEBUG, "Ignoring character: %s\n", yytext); }  /* ignore any other character */
\n                              {  if (INTERACTIV) { printf("[%i] > ",s_line); } s_line++; }
%%

void show_buffer(char* str)
{
   /*
    syslog(LOG_DEBUG,"[lex] new buffer");
        int j = strlen(str);

    for(int i=0; i<j; i++)
	{
	    if (str[i] != '\n')
		syslog(LOG_DEBUG,"[lex] yytext.length: %d, yytext[%d]: %i --> %c\n",j,i,str[i],str[i]);
	    else
		syslog(LOG_DEBUG,"[lex] Found LF\n");
	}
    
    syslog(LOG_DEBUG,"[lex] end buffer");
  */    
}
