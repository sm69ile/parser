#if !defined _parser_h
#define _parser_h

#include <stdio.h>
#include <stdlib.h> 
#include <syslog.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <stdbool.h>
#include <signal.h>
#include "../config.h"

#define MAX_STATE 10

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

sObject *psObjIni, *psObj;
pid_t c_pid;
int iState;



int yy_scan_string(char*);
int yylex_destroy();
void yyerror(const char *s);

void vinit(sObject *);
void vnext(sObject *);
void vlist(sObject *);
sVset* get_vset(sObject*, char*);
bool set_vset(sObject*, char*, unsigned long);
void vfree();

void oinit();
void onext();
void olist();
void ofree();

void cinit(sObject *); 
void cnext(sObject *);
void clist(sObject *);
void cfree(sObject *);
void plist(int, ... );

void v_show();
void v_close();

void xshow(int, char **);
void load_file(int, char **);
void ctrl(char*, char*);
void quit();


#endif
