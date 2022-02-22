%{

#include <readline/readline.h>
#include <readline/history.h>
#include "parser.h"
#define CRED "\001\33[1m\33[31m\002"
#define CWHITE "\001\33[0m\002"
  
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
%token <ctrl> CONTROL
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

plist:
|      NUMBER { plist(1,$1); }
|      NUMBER COMMA NUMBER { plist(2,$1,$3); }
|      NUMBER COMMA NUMBER COMMA NUMBER { plist(3,$1,$3,$5); }
|      NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER { plist(4,$1,$3,$5,$7); }
|      NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER COMMA NUMBER { plist(5,$1,$3,$5,$7,$9); }
;

set:  SET SPARA NUMBER SEMICOLON { if (! set_vset(psObj, $2, $3)) { psObj->psVset->para=$2; psObj->psVset->value=$3; vnext(psObj); } }
|     set SET SPARA NUMBER SEMICOLON { if (! set_vset(psObj, $3, $4)) { psObj->psVset->para=$3; psObj->psVset->value=$4; vnext(psObj); } }
;
ctrl: CONTROL SEMICOLON { ctrl($1,NULL); }
|
CONTROL CPARA SEMICOLON { ctrl($1,$2);}

%%
 void yyerror(const char* s) {
  printf("[%s] Error: %s near line %d\n", PACKAGE_STRING, s, s_line);
}


int main(int argc, char *argv[])
{
  openlog(PACKAGE_STRING, LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1);
  syslog (LOG_DEBUG, "Program started by User %d", getuid ());
  printf("%s\tArguments: %i\n", PACKAGE_STRING, argc);      
  
  /* FILE* fp = fopen(P_OUT, "r" ); */
  /* if (! fp) */
  /*   { */
  /*     syslog(LOG_DEBUG,"Output file %s not found, trying to create\n", P_OUT); */
  /*     if(!(fp = fopen(P_OUT, "a+"))) */
  /* 	{ */
  /* 	  syslog(LOG_DEBUG,"Cannot open output file for writing: %s\n", P_OUT); */
  /* 	  exit(EXIT_FAILURE); */
  /* 	} */
  /*     else */
  /* 	{ */
  /* 	  syslog(LOG_DEBUG,"New output file %s created\n", P_OUT); */
  /* 	  fprintf(fp,FIRST_LINE); */
  /* 	  fclose(fp); */
  /* 	} */
  /*   } */
  
  iThr = 0;
  
  oinit();
  onext();

  if (argc > 1)
    { load_file(argc, argv); }

  char prompt[LINE_BUF];

  do { 

      bzero(prompt, LINE_BUF);
      snprintf(prompt, LINE_BUF, "%s [%i] > %s", CRED, s_line, CWHITE); 

      char *f = readline(prompt);
      YY_BUFFER_STATE b = yy_scan_string(f);
      yyparse();
      yy_delete_buffer(b);

      add_history(f); s_line++;
      free(f);
  }while(1);


  printf("Uups, unreachable code detected, exiting\n");
  quit();
   
  return 0;
}

  void oinit()
  {
    syslog(LOG_DEBUG, "Initialising memory for object, starting node\n");

    iState_idx = 0;
    iObj_idx = 1;

    if (!(psObjIni=(sObject*) malloc(sizeof(sObject))))
      {
	syslog(LOG_DEBUG, "malloc failed: sObject*\n");
	exit(EXIT_FAILURE);
      }

    psObjIni->key=0;
    psObjIni->s_line=-1;  
    psObjIni->name=strdup(INI);
    psObjIni->angle_deg=0;
    psObjIni->angle_rad=0;

    psObjIni->psVsetIni=NULL;
    psObjIni->psVset=NULL;
    psObjIni->psComIni=NULL;
    psObjIni->psCom=NULL;
    
    psObjIni->next=psObjIni;
  }


  void onext()
  {
    psObjIni->key++;
    syslog(LOG_DEBUG, "Initialising memory, object key: %d\n", psObjIni->key);
      
    if (!(psObj=(sObject*) malloc(sizeof(sObject))))
      {
	syslog(LOG_DEBUG, "malloc failed: sObject*\n");
	exit(EXIT_FAILURE);
      }

    psObj->key=psObjIni->key;
    psObj->s_line=0;
    psObj->name=strdup(CURR);
    psObj->angle_deg=0;
    psObj->angle_rad=0;

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
    sObject* act = get_object_by_key(iObj_idx);
    
    printf("obj: [line: %d] [node %d] %s\n", act->s_line, act->key, act->name);
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

	printf("obj: [line: %d] [node %d] %s\n", act->s_line, act->key, act->name);
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

  int olist_count_command()
  {
    sObject* last = psObjIni->next;
    sObject* first = psObjIni;
    sObject* act = last;
    int i=0;
  
    do
      {
	sObject* next = act->next;
	i+=act->psComIni->key;
	act=next;
      }
    while(act != first);
    return i;
  }

  sObject* get_object_by_key(int key)
  {

    sObject* last = psObjIni->next;
    sObject* first = psObjIni;
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
    syslog(LOG_DEBUG, "Initialising memory for drawing parameters, starting node\n");

    if (!(psObj->psVsetIni=(sVset*) malloc(sizeof(sVset))))
      {
	syslog(LOG_DEBUG, "malloc failed: sVset*\n");
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
    syslog(LOG_DEBUG, "Initialising memory, drawing parameter, key: %d\n", psObj->psVsetIni->key);
  
    if (!(psObj->psVset=(sVset*) malloc(sizeof(sVset))))
      {
	syslog(LOG_DEBUG, "malloc failed: sVset*\n");
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
	    //	  fprintf(stderr,"Found [source line: %d] [node %d] %s: %lu\n",act->d_line, act->key, act-> para, act->value);
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
    syslog(LOG_DEBUG, "Initialising memory for command, starting node\n");

    if (!(psObj->psComIni=(sCommand*) malloc(sizeof(sCommand))))
      {
	syslog(LOG_DEBUG, "malloc failed: sCommand*\n");
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
    syslog(LOG_DEBUG, "Initialising memory, command node: %d\n", psObj->psComIni->key);
  
    if (!(psObj->psCom=(sCommand*) malloc(sizeof(sCommand))))
      {
	syslog(LOG_DEBUG, "malloc failed: sCommand*\n");
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
	syslog(LOG_DEBUG, "malloc failed: int*\n");
	exit(EXIT_FAILURE);
      }

    va_start(args,i);
    for(int j=0; j<i; j++)
      {
	act->para[j] = (int)va_arg(args, int);
      }
    va_end(args);
  }

  void v_display()
  {
    if (iThr < MAX_THREADS)
      {
	fprintf(stderr, "Starting thread %i of %i\n",iThr,MAX_THREADS);

	if ( pthread_create(&v_thr[iThr], NULL, x_show, (void*) NULL) != 0)
	  { fprintf (stderr, "Could not start thread, max %i, curr %i\n", MAX_THREADS, iThr); }
	else
	  iThr++;
      }
    else
      { fprintf(stdout, "Max threads running: %i of %i\n", iThr,MAX_THREADS);  }
  }

  void *x_show() { xshow(0, NULL); return NULL; }

void help()
{
  FILE *fp;
  char *s;

  syslog(LOG_DEBUG,"Using Help file: %s\n", HELP_FILE);
  
  if ((fp = fopen(HELP_FILE, "r")))
    {
      if((s = (char *) malloc(LINE_BUF * sizeof(char))))
	{
	  while(!feof(fp))
	    {
	      bzero(s,LINE_BUF);
	      if(fgets(s,LINE_BUF,fp))
		printf("%s",s);
	    }
	  free(s);
	}
      else
	{ syslog(LOG_DEBUG, "malloc failed: char*\n"); }
      fclose(fp);
    }
  else
    { syslog(LOG_DEBUG, "Cannot open help file: %s\n",HELP_FILE); }
}

  
  void load_file(int argc, char *argv[])
  {
    int j,i = 1;
    char * l_argv;
    const char* LOAD = ". ";
  
    do{
    
      j=strlen(LOAD)+strlen(argv[i])+1;
      syslog (LOG_DEBUG, "File: %s - Command length: %i\n",argv[i],j);
    
      if (!(l_argv=(char*) malloc(j*sizeof(char))))
	{
	  syslog(LOG_DEBUG, "malloc failed: char*\n");
	  exit(EXIT_FAILURE);
	}
    
      bzero(l_argv,j);
      strcat(l_argv,LOAD);
      strcat(l_argv,argv[i]);
      syslog (LOG_DEBUG, "Command line: %s\n",l_argv);
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
	else if (! strncmp(cmd,"save",strlen("save")))
	  { save(); }
	else if (! strncmp(cmd,"flush",strlen("flush")))
	  { yyclearin; }

	else if (! strncmp(cmd,"nobj",strlen("nobj")))
	  { ctrl("object","next"); }
	else if (! strncmp(cmd,"pobj",strlen("pobj")))
	  { ctrl("object","prev"); }
	else if (! strncmp(cmd,"lobj",strlen("lobj")))
	  { ctrl("object","last"); }
	    
	else if (! strncmp(cmd,"nstatus",strlen("nstatus")))
	  { ctrl("status","next"); }
	else if (! strncmp(cmd,"pstatus",strlen("pstatus")))
	  { ctrl("status","prev"); }
	    
	else if (! strncmp(cmd,"display",strlen("display")))
	  {
	    if (iCstate == v_close)
	      v_display();
	    else
	      printf("Client already running\n");
	  }
	else if (! strncmp(cmd,"sleep",strlen("sleep")))
	  { sleep(3); }
	else if (! strncmp(cmd,"hide",strlen("hide")))
	  { iCtask = v_hide; }
	else if (! strncmp(cmd,"redraw",strlen("redraw")))
	  { iCtask = v_redraw; }

	else if (! strncmp(cmd,"show",strlen("show")))
	  { iCtask = v_show; }

	else if (! strncmp(cmd,"help",strlen("help")))
	  { help(); }
	    
	else if (( !strncmp(cmd,"quit",strlen("quit"))) || ( !strncmp(cmd,"exit",strlen("exit"))))
	  { quit(); }
	else
	  { printf("Command %s not found.\n", cmd ); }
      }
    else
      {
	if (!strncmp(cmd,"status",strlen("status")) &&  !strncmp(para,"next",strlen("next")) )
	  { if (iState_idx < MAX_STATE)
	      { iState_idx++; iCtask = v_redraw; }
	    else
	      { iState_idx = 0; iCtask = v_redraw; } 
	  }
	else
	  if (!strncmp(cmd,"status",strlen("status")) &&  !strncmp(para,"prev",strlen("prev")) )
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
		    if (!strncmp(cmd,"object",strlen("object")) &&  !strncmp(para,"last",strlen("last")) )
		      { iObj_idx = (psObjIni->next->key); iCtask = v_redraw; }
		    else
		      if (!strncmp(cmd,"object",strlen("object")) &&  !strncmp(para,"count",strlen("count")) )
			{ printf("%s %s (incl. reserved):\t%i\n", cmd, para, psObjIni->next->key);}
		      else
			if (!strncmp(cmd,"command",strlen("command")) &&  !strncmp(para,"count",strlen("count")) )
			  { printf("%s %s (incl. reserved):\t%i\n", cmd, para, olist_count_command()); }

			else
			  { printf("Command sequence %s %s not found.\n", cmd, para); }
      }
  }



void save()
  {
    static char * commands[]=
      {
       "line",       //32513
       "lineto",     //32514
       "rectangle",  //32515
       "circle",     //32516
       "ellipse",    //32117
       "moveto",     //32518
       "REM",        //32519
       "flowways",   //32520
       "arc",        //32521
       "state",      //32522
       "break",      //32523
       "clear"       //32524
      };
    
    
    int clength = sizeof(commands)/sizeof(commands[0]);  
    char * buf;
    int count_o = 0;
    int count_c = 0;
    int count_p = 0;
    int buf_length;

    FILE* fp = fopen(P_OUT, "w+" );
    
    if (! fp){ syslog(LOG_DEBUG,"Cannot open output file %s\n", P_OUT); }
    else
      {
	sObject* olast = psObjIni->next;
	sObject* ofirst = psObjIni;
	sObject* oact = olast;
	do
	  {
	    sObject* onext = oact->next;

	    fprintf(fp,"%s%s",oact->name,"~");
	    count_o++;

	    buf_length = strlen("XXXXX^X");

	    if ((buf = (char*)malloc((buf_length)*sizeof(char))))
	      {
		for(int j=0; j<oact->psComIni->key; j++)
		  {
		    sCommand* cact = get_command_by_key(oact->psComIni, oact->psComIni->next, j);
		    
		    // check if the object has any comman parameters

		    if(cact != NULL)
		      {
			for(int i=0; i<clength; i++)
			  {
			    if(! strncmp(cact->name, commands[i], strlen(cact->name)))
			    {
				bzero(buf,buf_length);
				sprintf(buf,"%d^", i+DCB);
				fprintf(fp,"%s",buf);
				count_c++;
				
				for(int j=0;j<cact->count_para;j++)
				  {
				    bzero(buf,buf_length);
				    sprintf(buf,"%d^", cact->para[j]);
				    fprintf(fp,"%s",buf);
				    count_p++;
				  }
				break;
			      }
			  }
		      }
		  }
		fprintf(fp,"%s","\n");

		free(buf);
	      }
	    else
	      { syslog(LOG_DEBUG, "malloc failed: char*\n"); }
	    oact=onext;
	  } 
	while(oact != ofirst);
      } 
    fclose(fp);
    printf("Successfully wrote:\n%i Objects\n%i Commands\n%i Parameter\nto file: %s\n", count_o, count_c, count_p, P_OUT);
  }


  void quit()
  {
    if (iCstate == v_open)
      iCtask = v_exit;

    ofree();
    closelog();
    exit(EXIT_SUCCESS);
  }



