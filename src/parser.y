%{

#include "parser.h"
  
  extern int yylex (void);
  extern int yyparse();
  extern FILE* yyin;
  extern int s_line;
  
  const char* CURR = "Empty";
  const char* INI = "INI";
  
  %}

%union{
    double real;
    char* oname;
    char* cname; /* line, lineto, rectangle, circle, ellipse, moveto, floodfill, bar, fillellipse, MAX, REM, STATE, COLOR */
    char* set; /* set */
    char* spara;
    char* ctrl; /* exit, quit, list ... */
    char* cpara; /*all, next, prev, ... */
    int integer;
    char character;
}

%token <oname> ONAME
%token <cname> CNAME
%token <set> SET
%token <spara> SPARA
%token <ctrl> CTRL
%token <cpara> CPARA
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
|       list object { }
|       list command { }
|       list plist { }
|       list set { }
|       list ctrl { }
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

object: ONAME LBRACE command RBRACE { psObj->name=$1; psObj->s_line=s_line-1;  onext(); iObj_idx=psObjIni->next->key-1; }
|       object ONAME LBRACE command RBRACE { psObj->name=$2; psObj->s_line=s_line-1;onext(); iObj_idx=psObjIni->next->key-1; }
;

command: CNAME LPAREN plist RPAREN SEMICOLON { psObj->psCom->name=$1; psObj->psCom->s_line=s_line-1; cnext(psObj); }
|        command CNAME LPAREN plist RPAREN SEMICOLON { psObj->psCom->name=$2; psObj->psCom->s_line=s_line-1; cnext(psObj); }
;

plist: NUMBER { plist(1,$1); }
|      NUMBER COMMA NUMBER { plist(2,$1,$3); }
|      NUMBER COMMA NUMBER COMMA NUMBER { plist(3,$1,$3,$5); }
|      NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER { plist(4,$1,$3,$5,$7); }
|      NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER { plist(5,$1,$3,$5,$7,$9); }
;

set:  SET SPARA NUMBER SEMICOLON { if (! set_vset(psObj, $2, $3)) { psObj->psVset->para=$2; psObj->psVset->value=$3; vnext(psObj); } }
|     set SET SPARA NUMBER SEMICOLON { if (! set_vset(psObj, $3, $4)) { psObj->psVset->para=$3; psObj->psVset->value=$4; vnext(psObj); } }
;
ctrl: CTRL SEMICOLON { ctrl($1,NULL); }
|
CTRL CPARA SEMICOLON { ctrl($1,$2);}

%%
 void yyerror(const char* s) {
  printf("[%s] Error: %s in line %d\n", PACKAGE_STRING, s, s_line-1);
}


int main(int argc, char *argv[])
{
  openlog(PACKAGE_STRING, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
  syslog (LOG_NOTICE, "Program started by User %d", getuid ());
  
  iState_idx = 0;
  iObj_idx = 1;
  iThr = 0;
  
  oinit();
  onext();
  
  printf("%s\tArguments: %i\n", PACKAGE_STRING, argc);      
  
   if (argc > 1)
     { load_file(argc, argv); }
   
   printf("[%i] > ",s_line++);
   yyin = stdin;
   do { 
     yyparse();
   } while(!feof(yyin));
   

   fprintf(stderr,"Uups, \"feof(yyin);\" found, exiting\n");
   quit();
   
   return 0;
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
  psObjIni->s_line=-1;  
  psObjIni->name=strdup(INI);

  psObjIni->psVsetIni=NULL;
  psObjIni->psVset=NULL;
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
  psObj->s_line=0;
  psObj->name=strdup(CURR);

  psObjIni->psVsetIni=NULL;
  psObjIni->psVset=NULL;
  psObj->psComIni=NULL;
  psObj->psCom=NULL;

  vinit(psObj);
  vnext(psObj);
  
  cinit(psObj);
  cnext(psObj);
  
  psObj->next=psObjIni->next;
  psObjIni->next=psObj;
}


void olist_act()
{
  sObject* act = get_object_by_key(psObjIni, psObjIni->next, iObj_idx);
  
  printf("obj: [source line: %d] [node %d] %s\n", act->s_line, act->key, act->name);
  vlist(act);
  clist(act);
}


void olist()
{
  sObject* last = psObjIni->next;
  sObject* first = psObjIni;
  sObject* act = last;

  do
    {
      sObject* next = act->next;

      printf("obj: [source line: %d] [node %d] %s\n", act->s_line, act->key, act->name);
      vlist(act);
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


sObject* get_object_by_key(sObject* first, sObject* last, int key)
{
  sObject* act = last;
  do
    {
      sObject* next = act->next;
      if(act->key == key)
	return act;
      act=next;
    }
  while(act != first);

  return NULL;
}

/* vset block */
 
void vinit(sObject* psObj)
{
  syslog(LOG_NOTICE, "Initialising memory for drawing parameters, starting node\n");

  if (!(psObj->psVsetIni=(sVset*) malloc(sizeof(sVset))))
    {
      syslog(LOG_NOTICE, "malloc failed: sVset*\n");
      exit(EXIT_FAILURE);
    }

  psObj->psVsetIni->key=0;
  psObj->psVsetIni->d_line=-1;  
  psObj->psVsetIni->para=strdup(INI);
  psObj->psVsetIni->value=0;

  psObj->psVsetIni->next=psObj->psVsetIni;
}


void vnext(sObject* psObj)
{
  psObj->psVsetIni->key++;
  syslog(LOG_NOTICE, "Initialising memory, drawing parameter, key: %d\n", psObj->psVsetIni->key);
  
  if (!(psObj->psVset=(sVset*) malloc(sizeof(sVset))))
    {
      syslog(LOG_NOTICE, "malloc failed: sVset*\n");
      exit(EXIT_FAILURE);
    }

  psObj->psVset->key=psObj->psVsetIni->key;
  psObj->psVset->d_line=0;
  psObj->psVset->para=strdup(CURR);
  psObj->psVset->value=0;

  psObj->psVset->next=psObj->psVsetIni->next;
  psObj->psVsetIni->next=psObj->psVset;
}

void vlist(sObject* psObj)
{
  sVset* last = psObj->psVsetIni->next;
  sVset* first = psObj->psVsetIni;
  sVset* act = last;

  do
    {
      sVset* next = act->next;
      printf("dpara: [source line: %d] [node %d] %s %lu\n", act->d_line, act->key, act->para, act->value);
      act=next;
    }
  while(act != first);
}


sVset* get_vset(sObject* psObj, char *para)
{
  sVset* last = psObj->psVsetIni->next;
  sVset* first = psObj->psVsetIni;
  sVset* act = last;
  do
    {
      sVset* next = act->next;

      if (!strcmp(act->para, para))
	{
	  fprintf(stderr,"Found [source line: %d] [node %d] %s: %lu\n",act->d_line, act->key, act-> para, act->value);
	  return act;
	}
      act=next;
    }
  while(act != first);
  return NULL;

}

bool set_vset(sObject* psObj, char* para, unsigned long value)
{
  sVset *act = get_vset(psObj, para);
  if (act)
    { act->value=value; return true; }
  else
    return false;
}
  

void vfree(sObject* psObj)
{
  sVset* last = psObj->psVsetIni->next;
  sVset* first = psObj->psVsetIni;
  sVset* act = last;

  do
    {
      sVset* next = act->next;
      free(act->para);
      free(act);
      act=next;
    }
  while(act != first);
  free(act->para);
  free(act);
}

sVset* get_setting_by_name(sVset* first, sVset* last, const char* para)
{
  sVset* act = last;
  do
    {
      sVset* next = act->next;
      if (!strcmp(act->para, para))
	  return act;
      act=next;
    }
  while(act != first);

  return NULL;
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
  psObj->psComIni->s_line=-1;  
  psObj->psComIni->name=strdup(INI);
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
  psObj->psCom->s_line=0;
  psObj->psCom->name=strdup(CURR);
  
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

sCommand* get_command_by_key(sCommand* first, sCommand* last, int key)
{
  sCommand* act = last;
  do
    {
      sCommand* next = act->next;
      if(act->key == key)
	return act;
      act=next;
    }
  while(act != first);

  return NULL;
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


void v_create()
{
  if (iThr < MAX_THREADS)
  {
    fprintf(stderr, "Starting thread %i of %i\n",iThr,MAX_THREADS);

      if ( pthread_create(&v_thr[iThr], NULL, v_show, (void*) NULL) != 0)
	{ fprintf (stderr, "Could not start thread, max %i, curr %i\n", MAX_THREADS, iThr); }
      else
      	iThr++;
  }
  else
    { fprintf(stdout, "Max threads running: %i of %i\n", iThr,MAX_THREADS);  }
}

void *v_show() { xshow(0, NULL); return NULL; }


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


void ctrl(char* cmd, char* para)
{

  if(! para)
    {
      if (! strncmp(cmd,"init",strlen("init")))
	{ ofree(); oinit(); onext(); }
      else if (! strncmp(cmd,"list",strlen("list")))
	{ olist(); }
      else if (! strncmp(cmd,"clear",strlen("clear")))
	{ yyclearin; }
      else if (! strncmp(cmd,"nobj",strlen("nobj")))
	{ ctrl("object","next"); }
      else if (! strncmp(cmd,"nstate",strlen("nstate")))
	{ ctrl("state","next"); }
      else if (! strncmp(cmd,"pstate",strlen("pstate")))
	{ ctrl("state","prev"); }
      else if (! strncmp(cmd,"pobj",strlen("pobj")))
	{ ctrl("object","prev"); }
      else if (! strncmp(cmd,"show",strlen("show")))
	{
	    if (iCstate == v_close)
		v_create();
	    else
		fprintf(stderr, "Client already running\n");
	}
      else if (! strncmp(cmd,"sleep",strlen("sleep")))
	{ sleep(3); }
      else if (( !strncmp(cmd,"quit",strlen("quit"))) || ( !strncmp(cmd,"exit",strlen("exit"))))
	{ quit(); }
      else
	{ fprintf(stderr, "Command %s not found.\n", cmd ); }
    }
  else
    {
      if (!strncmp(cmd,"state",strlen("state")) &&  !strncmp(para,"next",strlen("next")) )
	{ if (iState_idx < MAX_STATE)
	    { iState_idx++; iCtask = v_redraw; }
	  else
	    { iState_idx = 0; iCtask = v_redraw; } 
	}
      else
	if (!strncmp(cmd,"state",strlen("state")) &&  !strncmp(para,"prev",strlen("prev")) )
	  { if (iState_idx > 0)
	      { iState_idx--; iCtask = v_redraw; }
	    else { iState_idx = MAX_STATE; iCtask = v_redraw; }
	  }
	else
	  if (!strncmp(cmd,"object",strlen("object")) &&  !strncmp(para,"next",strlen("next")) )
	    { if (iObj_idx < psObjIni->next->key)
		{ iObj_idx++; iCtask = v_redraw; }
	      else
		{ iObj_idx = 1; iCtask = v_redraw; } 
	    }
	  else
	    if (!strncmp(cmd,"object",strlen("object")) &&  !strncmp(para,"prev",strlen("prev")) )
	      { if (iObj_idx > 1)
		  { iObj_idx--; iCtask = v_redraw; }
		else
		  { iObj_idx = psObjIni->next->key; iCtask = v_redraw; }
	      }
	    else
	      if (!strncmp(cmd,"list", strlen("list")) &&  !strncmp(para,"all",strlen("all")) )
		{ olist(); }
	      else
		if (!strncmp(cmd,"list",strlen("list")) &&  !strncmp(para,"act",strlen("act")) )
		  { olist_act();}
	      else
		if (!strncmp(cmd,"show",strlen("show")) &&  !strncmp(para,"last",strlen("last")) )
		  { iObj_idx = (psObjIni->next->key-1); v_create() ;}

		else
		  { fprintf(stderr, "Command sequence %s %s not found.\n", cmd, para); }
    }
}


void quit()
{
    if (iCstate == v_open)
	iCtask = v_exit;

    ofree();
    closelog();
    exit(EXIT_SUCCESS);
}
