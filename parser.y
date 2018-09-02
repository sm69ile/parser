%{
#include <stdio.h>
#include <stdlib.h>
  
extern int yylex (void);
extern int yyparse();
extern FILE* yyin;

void yyerror(const char *s);

FILE* fp_open(char* fn); 

%}

%token  CMD_EHLO TOK_BRACKET_O TOK_BRACKET_C TOK_PARAN_O TOK_PARAN_C WORD SEMICOLON

%%

commands: /* empty */
        |
        commands command;
        |
	line commands;

command:
        cmd_ehlo;
        |
	bracket_open;
        |
        bracket_close;
        |
	word;
        |
	semicolon;
        |
	paran_open;
        |
        paran_close;

       
cmd_ehlo:
        CMD_EHLO
        {
	  printf("Found command: ehlo\n");
        };

bracket_open:
        TOK_BRACKET_O
	{
	    printf("Found token: bracket open\n");
        };

bracket_close:
        TOK_BRACKET_C
	{
	    printf("Found token: bracket close\n");
        };

paran_open:
        TOK_PARAN_O
	{
	    printf("Found token: paran open\n");
        };

paran_close:
        TOK_PARAN_C
	{
	    printf("Found token: paran close\n");
        };

word:
        WORD
	{
	    printf("Found word\n");
        };

semicolon:
        SEMICOLON
	{
	    printf("Found semicolon\n");
        };

line:
       CMD_EHLO TOK_BRACKET_O WORD TOK_BRACKET_C
        {
	  printf("Found line: ehlo[word]\n");
        };


%%

int main(int argc, char *argv[])
{
  int i = 1;

  printf("argc: %i\n", argc);
  
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
	  yyin = fp_open(argv[i++]);
	  do { 
	    yyparse();
	  } while(!feof(yyin));
      } while (i < argc);
     } 
     return 0;
}

  
void yyerror(const char* s) {
  fprintf(stderr, "Parse error: %s\n", s);
  exit(1);
}


FILE* fp_open(char* fn){
  
  FILE* fp = fopen(fn, "r"); // read mode
 
  if (fp == NULL)
    {
      perror("Error while opening the file.\n");
      exit(EXIT_FAILURE);
    }
 
  printf("Parsing contents of file: %s\n", fn);

  return fp;
}
