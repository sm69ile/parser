#if !defined _parser_h
#define _parser_h

#include <stdio.h>
#include <stdlib.h> 
#include <syslog.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>

#include <pthread.h>

#include "../config.h"

#define MAX_STATE 10
#define MAX_THREADS 1

#define FIRST_LINE "Graphikfile Viewer Ver. 0.1\n"
#define P_OUT "parser.agf"
#define HELP_FILE ETCDIR "/help.txt"
#define COLOR_FILE ETCDIR "/colors.txt"
#define DCB 32513
#define LINE_BUF 80


typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(const char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern int yylex_destroy();

enum eVtask {
    v_idle = 0,
    v_redraw = 1,
    v_exit = 2,
    v_hide = 3,
    v_show = 4
};

enum eVState {
    v_close= 0,
    v_open= 1
};

typedef struct Vset
{
  int key;
  int d_line;
  char *para;
  unsigned long value;

  struct Vset *next;
}sVset;

typedef struct Command
{
  int key;
  int s_line;
  char *name;
  int count_para;
  int *para;
  
  struct Command *next;
}sCommand;

typedef struct Object
{
  int key;
  int s_line;
  char *name;
  unsigned int angle_deg;
  //  double angle_rad;

  sVset *psVsetIni, *psVset;  
  sCommand *psComIni, *psCom;
  
  struct Object *next;
}sObject;



/* int iThr; */
/* pthread_t v_thr[MAX_THREADS]; */
/* sObject *psObjIni, *psObj; */

/* int iState_idx; */
/* int iObj_idx; */
/* int iCtask; */
/* int iCstate; */

//int yy_scan_string(char*);
//int yylex_destroy();
void yyerror(const char *s);

void vinit(sObject *);
void vnext(sObject *);
void vlist(sObject *);
sVset* get_vset(sObject*, char*);
bool set_vset(sObject*, char*, unsigned long);
void vfree();
sVset *get_setting_by_name(sVset*, sVset*, const char*);

void oinit();
void onext();
void olist_act();
void olist();
void ofree();
int olist_count_command();
sObject *get_object_by_key(int);

void cinit(sObject *); 
void cnext(sObject *);
void clist(sObject *);
void cfree(sObject *);
sCommand *get_command_by_key(sCommand*, sCommand*, int);

void plist(int, ... );

void v_create();
void *x_show();

void xshow(int, char **);
void load_file(int, char **);
void ctrl(char*, char*);
void help();
void save();
void quit();


#endif
