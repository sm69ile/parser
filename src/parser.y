
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
  char* split(char*, char);
  void plist(int, ... );
  void cinit(); 
  void cnext();
  void clist();
  void cfree();
  void load_file(int, char **);
  void quit();
  
  typedef struct Command
  {
    char *name;
    int *para;
    int count_para;
    int key;
    int s_line;
    struct Command *next;
  }sCommand;

  sCommand *psComIni, *psCom;

  typedef struct Object
  {
    char *name;
    sCommand *sCommand;
    struct Object *next;
  }sObject;

  sObject *psObjIni, *psObj;
  %}

%union{
    double real;
    char* identifier;
    char* cname; /* line, lineto, rectangle, circle, ellipse, moveto, floodfill, bar, fillellipse, MAX, REM, STATE, COLOR */
    char* ctrl; /* exit, quit, list */
    int integer;
    char character;
}

%token <identifier> IDENTIFIER
%token <cname> CNAME
%token <ctrl> CTRL
%token <integer> NUMBER
%token <real> REAL
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
| 	list line { }
|       list ctrl { }
|       list object { }
|       list command { }
|       list plist { }
;

line:   '\n'
|       expr '=' { printf("\t%.2g\n",$1);}
;

expr:   expr '+' expr {$$ = $1 + $3;}
| 	expr '-' expr {$$ = $1 - $3;}
| 	expr '*' expr {$$ = $1 * $3;}
| 	expr '/' expr {$$ = $1 / $3;}
|       REAL {$$ = $1;}
;

object: IDENTIFIER LBRACE command RBRACE { printf("%s\n",$1); }
|       object IDENTIFIER LBRACE command RBRACE { printf("%s\n",$2); }
;

command: CNAME LPAREN plist RPAREN SEMICOLON { psCom->name=$1; psCom->s_line=s_line; cnext(); }
|        command CNAME LPAREN plist RPAREN SEMICOLON { psCom->name=$2; psCom->s_line=s_line; cnext(); }
;

plist: NUMBER { plist(1,$1); }
|      NUMBER COMMA NUMBER { plist(2,$1,$3); }
|      NUMBER COMMA NUMBER COMMA NUMBER { plist(3,$1,$3,$5); }
|      NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER { plist(4,$1,$3,$5,$7); }
|      NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER { plist(5,$1,$3,$5,$7,$9); }
;

ctrl: CTRL { fprintf(stderr, "Received control: %s\n",$1);
   if (! strncmp($1,"list",strlen("list"))) { clist(); }
   else if (! strncmp($1,"init",strlen("init"))) { cfree(); cinit(); }
   else if (! strncmp($1,"clear",strlen("clear"))) { yyclearin; }
   else if (( !strncmp($1,"quit",strlen("quit"))) || ( !strncmp($1,"exit",strlen("exit")))) { quit(); } }

%%

int main(int argc, char *argv[])
{
    openlog(PACKAGE_STRING, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
    syslog (LOG_NOTICE, "Program started by User %d", getuid ());
    cinit();
    printf("%s\tArguments: %i\n", PACKAGE_STRING, argc);      
    if (argc > 1) { load_file(argc, argv); }
    else
      {
	printf("[%i] > ",s_line++);
	yyin = stdin;
	do { 
	  yyparse();
	} while(!feof(yyin));
      }
    
    closelog();
    quit();
    
    return 0;
}

 void quit()
 {
       cfree();
       exit(EXIT_SUCCESS);
 }

 
void yyerror(const char* s) {
  fprintf(stderr, "[%s] Error: %s in line %d\n", PACKAGE_STRING, s, s_line-1);
}


void load_file(int argc, char *argv[])
{
  int i = 1;
  
  do{
    syslog (LOG_NOTICE, "Opening input file %s\n", argv[i]);
    yyin = fopen(argv[i], "r");
    if ( ! yyin ){ syslog(LOG_NOTICE,"Cannot open input file %s\n", argv[i]); }
    else
      {
	do { yyparse(); } while(!feof(yyin));

	syslog (LOG_NOTICE, "Successfully scanned %d lines\n",s_line);
	syslog (LOG_NOTICE, "Closing input file\n");  

	fclose(yyin);
      }
  } while (++i < argc);   
}


void cinit()
{
  syslog(LOG_NOTICE, "Initialising memory, starting node\n");

  if (!(psComIni=(sCommand*) malloc(sizeof(sCommand))))
    {
      syslog(LOG_NOTICE, "malloc failed: sCommand*\n");
      exit(EXIT_FAILURE);
    }

  psComIni->name=NULL;
  psComIni->para=NULL;
  psComIni->count_para=0;
  psComIni->key=0;
  psComIni->s_line=-1;
  psComIni->next=psComIni;
 
  cnext();
}


void cnext()
{
  psComIni->key++;
  syslog(LOG_NOTICE, "Initialising memory, node: %d\n", psComIni->key);
  
  if (!(psCom=(sCommand*) malloc(sizeof(sCommand))))
    {
      syslog(LOG_NOTICE, "malloc failed: sCommand*\n");
      exit(EXIT_FAILURE);
    }

  psCom->name=NULL;
  psCom->para=NULL;
  psCom->count_para=0;
  psCom->s_line=0;
  psCom->key=psComIni->key;
  
  psCom->next=psComIni->next;
  psComIni->next=psCom;
}

void clist()
{
  sCommand* last = psComIni->next;
  sCommand* first = psComIni;
  sCommand* act = last;

  do
    {
      sCommand* next = act->next;
      printf("mem: [source line: %d] [source key %d] %s (args=%d) ", act->s_line, act->key, act->name, act->count_para);
      for(int i=0; i<act->count_para;i++)
	printf("%d ", act->para[i]);
      printf("\n");
      act=next;
    }
  while(act != first);
}


void cfree()
{
  sCommand* last = psComIni->next;
  sCommand* first = psComIni;
  sCommand* act = last;

  do
    {
      sCommand* next = act->next;
      free(act->name);
      free(act->para);
      free(act);
      act=next;
    }
  while(act != first);
  free(act->para);
  free(act->name);
  free(act);
}

void plist(int i, ...)
{
  va_list args;
  sCommand* act = psCom;;
  act->count_para=i;
  
  if (!(act->para=(int*) malloc(i*sizeof(int))))
    {
      syslog(LOG_NOTICE, "malloc() failed: int*\n");
      exit(EXIT_FAILURE);
    }

  va_start(args,i);
  for(int j=0; j<i; j++)
    {
      act->para[j] = (int)va_arg(args, int);
    }
  va_end(args);
}
