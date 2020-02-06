%{

#include "parser.h"
  
  extern int yylex (void);
  extern int yyparse();
  extern FILE* yyin;
  extern int s_line;
  
  //  sObject *psObjIni, *psObj;

  const char* CURR = "CURR";
  const char* INI = "INI";
  
  %}

%union{
    double real;
    char* oname;
    char* cname; /* line, lineto, rectangle, circle, ellipse, moveto, floodfill, bar, fillellipse, MAX, REM, STATE, COLOR */
    char* ctrl; /* exit, quit, list */
    int integer;
    char character;
}

%token <oname> ONAME
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

object: ONAME LBRACE command RBRACE { psObj->name=$1; onext(); }
|       object ONAME LBRACE command RBRACE { psObj->name=$2; onext(); }
;

command: CNAME LPAREN plist RPAREN SEMICOLON { psObj->psCom->name=$1; psObj->psCom->s_line=s_line-1; cnext(psObj); }
|        command CNAME LPAREN plist RPAREN SEMICOLON { psObj->psCom->name=$2; psObj->psCom->s_line=s_line-1; cnext(psObj); }
;

plist: NUMBER { plist(1,$1); }
|      NUMBER COMMA NUMBER { plist(2,$1,$3); }
|      NUMBER COMMA NUMBER COMMA NUMBER { plist(3,$1,$3,$5); }
|      NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER { plist(4,$1,$3,$5,$7); }

;

ctrl: CTRL { fprintf(stderr, "Received control: %s\n",$1);
   if (! strncmp($1,"list",strlen("list"))) { olist(); }
   else if (! strncmp($1,"init",strlen("init"))) { ofree(); oinit(); onext(); }
   else if (! strncmp($1,"clear",strlen("clear"))) { yyclearin; }
   else if (! strncmp($1,"show",strlen("show"))) { if (fork() == 0) xshow(0, NULL); }
   else if (( !strncmp($1,"quit",strlen("quit"))) || ( !strncmp($1,"exit",strlen("exit")))) { quit(); } }

%%

int main(int argc, char *argv[])
{
    openlog(PACKAGE_STRING, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
    syslog (LOG_NOTICE, "Program started by User %d", getuid ());

    oinit();
    onext();

    printf("%s\tArguments: %i\n", PACKAGE_STRING, argc);      
    if (argc > 1) { load_file(argc, argv); }

    printf("[%i] > ",s_line++);
    yyin = stdin;
    do { 
      yyparse();
    } while(!feof(yyin));
        
    closelog();
    quit();
    
    return 0;
}

 void quit()
 {
       ofree();
       exit(EXIT_SUCCESS);
 }

 
void yyerror(const char* s) {
  fprintf(stderr, "[%s] Error: %s in line %d\n", PACKAGE_STRING, s, s_line-1);
}


void load_file(int argc, char *argv[])
{
  int j,i = 1;
  char * l_argv;
  const char* LOAD = ". ";
  
  do{
    
    j=strlen(LOAD)+strlen(argv[i])+1;
    syslog (LOG_NOTICE, "File: %s - Command length: %i\n",argv[i],j);
    
    if (!(l_argv=(char*) malloc(j*sizeof(char))))
      {
	syslog(LOG_NOTICE, "malloc() failed: char\n");
	exit(EXIT_FAILURE);
      }
    
    bzero(l_argv,j);
    strcat(l_argv,LOAD);
    strcat(l_argv,argv[i]);
    syslog (LOG_NOTICE, "Command line: %s\n",l_argv);
    yy_scan_string(l_argv);
    yyparse();
    yylex_destroy();
    free(l_argv);

  } while (++i < argc);   
}

void oinit()
{
  syslog(LOG_NOTICE, "Initialising memory for object, starting node\n");

  if (!(psObjIni=(sObject*) malloc(sizeof(sObject))))
    {
      syslog(LOG_NOTICE, "malloc failed: sObject*\n");
      exit(EXIT_FAILURE);
    }

  psObjIni->key=0;
  psObjIni->name=strdup(INI);

  psObjIni->psComIni=NULL;
  psObjIni->psCom=NULL;

  psObjIni->next=psObjIni;
}

void onext()
{
  psObjIni->key++;
  syslog(LOG_NOTICE, "Initialising memory, object key: %d\n", psObjIni->key);
  
  if (!(psObj=(sObject*) malloc(sizeof(sObject))))
    {
      syslog(LOG_NOTICE, "malloc failed: sObject*\n");
      exit(EXIT_FAILURE);
    }

  psObj->key=psObjIni->key;
  psObj->name=strdup(CURR);

  psObj->psComIni=NULL;
  psObj->psCom=NULL;

  cinit(psObj);
  cnext(psObj);
  
  psObj->next=psObjIni->next;
  psObjIni->next=psObj;
}


void olist()
{
  sObject* last = psObjIni->next;
  sObject* first = psObjIni;
  sObject* act = last;

  do
    {
      sObject* next = act->next;
      printf("obj: [node %d] %s\n", act->key, act->name);
      clist(act);
      act=next;
    }
  while(act != first);
}


void ofree()
{
  sObject* last = psObjIni->next;
  sObject* first = psObjIni;
  sObject* act = last;

  do
    {
      sObject* next = act->next;
      free(act->name);
      cfree(act);
      free(act);
      act=next;
    }
  while(act != first);
  free(act->name);
  free(act);
}


void cinit(sObject *psObj)
{
  syslog(LOG_NOTICE, "Initialising memory for command, starting node\n");

  if (!(psObj->psComIni=(sCommand*) malloc(sizeof(sCommand))))
    {
      syslog(LOG_NOTICE, "malloc failed: sCommand*\n");
      exit(EXIT_FAILURE);
    }

  psObj->psComIni->key=0;
  psObj->psComIni->name=strdup(INI);
  psObj->psComIni->s_line=-1;  
  psObj->psComIni->count_para=0;
  psObj->psComIni->para=NULL;
  
  psObj->psComIni->next=psObj->psComIni;
}


void cnext(sObject *psObj)
{
  psObj->psComIni->key++;
  syslog(LOG_NOTICE, "Initialising memory, command node: %d\n", psObj->psComIni->key);
  
  if (!(psObj->psCom=(sCommand*) malloc(sizeof(sCommand))))
    {
      syslog(LOG_NOTICE, "malloc failed: sCommand*\n");
      exit(EXIT_FAILURE);
    }

  psObj->psCom->key=psObj->psComIni->key;
  psObj->psCom->name=strdup(CURR);
  psObj->psCom->s_line=0;
  psObj->psCom->count_para=0;
  psObj->psCom->para=NULL;
  
  psObj->psCom->next=psObj->psComIni->next;
  psObj->psComIni->next=psObj->psCom;
}


void clist(sObject *psObj)
{
  sCommand* last = psObj->psComIni->next;
  sCommand* first = psObj->psComIni;
  sCommand* act = last;

  do
    {
      sCommand* next = act->next;

      printf("com: [source line: %d] [node %d] %s (args=%d) ", act->s_line, act->key, act->name, act->count_para);
      for(int i=0; i<act->count_para;i++)
	printf("%d ", act->para[i]);
      printf("\n");

      act=next;
    }
  while(act != first);
}


void cfree(sObject *psObj)
{
  sCommand* last = psObj->psComIni->next;
  sCommand* first = psObj->psComIni;
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
  sCommand* act = psObj->psCom;
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
