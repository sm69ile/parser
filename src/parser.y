
%{
#include <stdio.h>
#include <stdlib.h> 
#include <syslog.h>
#include <unistd.h>
#include "../config.h"

    extern int yylex (void);
    extern int yyparse();
    extern FILE* yyin;

    
    void yyerror(const char *s);

    FILE* fp_open(char* fn); 

    %}

%union{
    double real;
    char* identifier;
    char* command; /* line, lineto, rectangle, circle, ellipse, moveto, floodfill, bar, fillellipse, MAX, REM, STATE, COLOR */
    int integer;
    char character;
 }


%token <identifier> IDENTIFIER
%token <command> COMMAND
%token <integer> NUMBER
%token <character> COMMA
%token <character> LPAREN
%token <character> RPAREN

%type <real> expr

%left '+' '-'
%left '*' '/'

%%

list: /* 	nothing */
| 	list line
|       list identifier
|       list command
|       list comma
|       list lparen
|       list rparen
;

line:   '\n'
|       expr '=' { printf("\t%.2g\n",$1);}
;

expr:   NUMBER  {$$ = $1;}
|       expr '+' expr {$$ = $1 + $3;}
| 	expr '-' expr {$$ = $1 - $3;}
| 	expr '*' expr {$$ = $1 * $3;}
| 	expr '/' expr {$$ = $1 / $3;}
;

identifier: IDENTIFIER { printf("Identifier: %s\n",$1); }
;

command: COMMAND  { printf("Command: %s\n",$1); }
;

comma: COMMA { printf("Comma: %c\n",$1); }
;

lparen: LPAREN { printf("LParen: %c\n",$1); }
;

rparen: RPAREN { printf("RParen: %c\n",$1); }
;


%%

int main(int argc, char *argv[])
{
    int i = 1;

    printf("%s\tArguments: %i\n", PACKAGE_STRING, argc);

    openlog(PACKAGE_STRING, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
    syslog (LOG_NOTICE, "Program started by User %d", getuid ());
    
    if (argc == 1)
	{
	    yyin = stdin;
	    do { 
		yyparse();
	    } while(!feof(yyin));
	}
    else
	{
	    do{
	      syslog (LOG_NOTICE, "Open input file %s\n", argv[i]);
	      yyin = fopen(argv[i++], "r");
		do { 
		    yyparse();
		} while(!feof(yyin));
                syslog (LOG_NOTICE, "Closing input file\n");  
                fclose(yyin);
	    } while (i < argc);
	} 

    closelog();
    return 0;
}

  
void yyerror(const char* s) {
  fprintf(stderr, "[%s] Error: %s\n", PACKAGE_STRING,s);
}


FILE* fp_open(char* fn){
  
  FILE* fp = fopen(fn, "r");     // read mode
 
    if (fp == NULL)
	{
	    perror("Error while opening the file.\n");
	    exit(EXIT_FAILURE);
	}
 
    printf("Parsing contents of file: %s\n", fn);

    return fp;
}
