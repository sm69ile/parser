#if !defined _parser_h
#define _parser_h

#include <stdio.h>
#include <stdlib.h> 
#include <syslog.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include "../config.h"

typedef struct Command
{
  int key;
  char *name;
  int s_line;
  int count_para;
  int *para;
  
  struct Command *next;
}sCommand;

typedef struct Object
{
  int key;
  char *name;
  
  sCommand *psComIni, *psCom;
  
  struct Object *next;
}sObject;

sObject *psObjIni, *psObj;


void yyerror(const char *s);
void plist(int, ... );
void cinit(sObject *); 
void oinit();
void cnext(sObject *);
void onext(); 
void clist(sObject *);
void olist();
void cfree(sObject *);
void ofree();
void load_file(int, char **);
void quit();
int yy_scan_string(char*);
int yylex_destroy();

// void xshow(int, char **, sObject *);
void xshow(int, char **);

#endif
