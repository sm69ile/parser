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
#include <signal.h> // fork


#include "../config.h"

#define MAX_STATE 10
#define MAX_THREADS 10

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

  sVset *psVsetIni, *psVset;  
  sCommand *psComIni, *psCom;
  
  struct Object *next;
}sObject;


pthread_t v_thr[MAX_THREADS];
// static int v_thr_ret[MAX_THREADS];
int iThr;

sObject *psObjIni, *psObj;

int iState_idx;
int iObj_idx;

int yy_scan_string(char*);
int yylex_destroy();
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
sObject *get_object_by_key(sObject*, sObject*, int);

void cinit(sObject *); 
void cnext(sObject *);
void clist(sObject *);
void cfree(sObject *);
sCommand *get_command_by_key(sCommand*, sCommand*, int);

void plist(int, ... );

void v_create();
void *v_show();

void xshow(int, char **);
void load_file(int, char **);
void ctrl(char*, char*);
void v_destroy();
void quit();


#endif
