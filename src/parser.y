
%{
#include <stdio.h>
#include <stdlib.h> 
#include <syslog.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include "../config.h"
  
  extern int yylex (void);
  extern int yyparse();
  extern FILE* yyin;
  extern int s_line;
  
  void yyerror(const char *s);
  FILE* fp_open(char* fn); 
  char* split(char*, char);
  void plist(int, ... );
  void cinit(); 
  void cnext();
  void clist();
  void cfree();
  
  typedef struct Command
  {
    char *name;
    int *args;
    int count_args;
    int key;
    int s_line;
    struct Command *next;
  }sCommand;

  sCommand *psComIni, *psCom;

%}

%union{
    double real;
    char* identifier;
    char* cname; /* line, lineto, rectangle, circle, ellipse, moveto, floodfill, bar, fillellipse, MAX, REM, STATE, COLOR */
    int integer;
    char character;
}


%token <identifier> IDENTIFIER
%token <cname> CNAME
%token <integer> NUMBER
%token <character> COMMA
%token <character> LPAREN
%token <character> RPAREN
%token <character> LBRACE
%token <character> RBRACE
%token <character> SEMICOLON

%type <real> expr

%left '+' '-'
%left '*' '/'

%%

list:   /* nothing */
| 	list line
|       list object
|       list command 
|       list plist
;

line:   '\n'
|       expr '=' { printf("\t%.2g\n",$1);}
;

expr:   expr '+' expr {$$ = $1 + $3;}
| 	expr '-' expr {$$ = $1 - $3;}
| 	expr '*' expr {$$ = $1 * $3;}
| 	expr '/' expr {$$ = $1 / $3;}
|       NUMBER {$$ = $1;}
;

object: IDENTIFIER LBRACE command RBRACE { char* p=split($1,'{'); printf("object:\t\t%s\n",p); free(p); }
;

command: CNAME LPAREN plist RPAREN SEMICOLON { psCom->name=split($1,'('); cnext(); }
|        command CNAME LPAREN plist RPAREN SEMICOLON { psCom->name=split($2,'('); cnext(); }
;

plist: NUMBER COMMA NUMBER { plist(2,$1,$3); }
|      NUMBER COMMA NUMBER COMMA NUMBER { plist(3,$1,$3,$5); }
|      NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER { plist(4,$1,$3,$5,$7); }
|      NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER { plist(5,$1,$3,$5,$7,$9); }
;

%%

int main(int argc, char *argv[])
{
    int i = 1;

    printf("%s\tArguments: %i\n", PACKAGE_STRING, argc);

    openlog(PACKAGE_STRING, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
    syslog (LOG_NOTICE, "Program started by User %d", getuid ());

    cinit();

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
	      s_line=0;
	      yyin = fopen(argv[i++], "r");
		do { 
		    yyparse();
		} while(!feof(yyin));
                syslog (LOG_NOTICE, "Closing input file\n");  
                syslog (LOG_NOTICE, "Successfully scanned %d lines\n",s_line);
                fclose(yyin);
	    } while (i < argc);

	} 

    clist();
    cfree();
    closelog();

    return 0;
}

  
void yyerror(const char* s) {
  fprintf(stderr, "[%s] Error: %s in line %d\n", PACKAGE_STRING, s, s_line);
  cfree();
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


void cinit()
{
  syslog(LOG_NOTICE, "Initialising memory, starting node\n");

  if (!(psComIni=(sCommand*) malloc(sizeof(sCommand))))
    {
      syslog(LOG_NOTICE, "malloc failed: sCommand*\n");
      exit(1);
    }

  psComIni->name=NULL;
  psComIni->args=NULL;
  psComIni->count_args=0;
  psComIni->key=0;
  psComIni->s_line=0;
  psComIni->next=NULL;
  cnext();
}


void cnext()
{
  psComIni->key++;
  syslog(LOG_NOTICE, "Initialising memory, node: %d\n", psComIni->key);

  
  if (!(psCom=(sCommand*) malloc(sizeof(sCommand))))
    {
      syslog(LOG_NOTICE, "malloc failed: sCommand*\n");
      exit(1);
    }

  psCom->name=NULL;
  psCom->args=NULL;
  psCom->count_args=0;
  psCom->s_line=s_line;
  psCom->key=psComIni->key;
  
  psCom->next=psComIni->next;
  psComIni->next=psCom;
}


void clist()
{
  sCommand* last = psComIni->next;
  sCommand* act = last;

  do
    {
      sCommand* next = act->next;
      printf("mem: [%d] [%d] %s (args=%d) ", act->s_line, act->key, act->name, act->count_args);
      for(int i=0; i<act->count_args;i++)
	printf("%d ",*act->args++);
      printf("\n");
      act=next;
    }
  while(act!=NULL);
}


void cfree()
{
  sCommand* last = psComIni->next;
  sCommand* act = last;

  do
    {
      sCommand* next = act->next;
      //  Warum geht das nicht ?    printf("free: %s - %d - %d - %d\n",act->name, act->count_args, act->key, psComIni->key);
      free(act->name);
      free(act);
      act=next;
    }
  while(act->key >1);
  free(act->name);
  free(act);
}
  

void plist(int i, ...)
{
  va_list args;
  int arg;

  psCom->count_args=i;
  
  if (!(psCom->args=(int*) malloc(psCom->count_args*sizeof(int))))
    {
      syslog(LOG_NOTICE, "malloc() failed: int*\n");
      exit(1);
    }
  if(i)
    {
      va_start(args,i);
      for(int j=0; j<i; j++)
	{
	  arg = (int)va_arg(args, int);
	  psCom->args[j]=arg;
	}
      va_end(args);
    }
}


char* split(char* s, char c)
{
  char *p,*l;
  int i = strlen(s);

  syslog(LOG_DEBUG, "Buflen: %d\n", i);

  l=NULL;
  
  if (!(p = (char*) malloc(i*sizeof(char))))
    {
      syslog(LOG_NOTICE, "malloc() failed: char*\n");
      exit(1);
    }

  bzero(p,i);
  for(i=0;s[i]!=c;i++)
    p[i]=s[i];
  
  i=strlen(p);
  syslog(LOG_DEBUG, "Buflen: %d\n", i);
  
  if ((l = (char*) malloc(i*sizeof(char))))
    {
      bzero(l,i);
      for(i=0;p[i]!='\0';i++)
	l[i]=p[i];
    }
  free(p);

  return l;
}
