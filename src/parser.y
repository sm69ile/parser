%{
#include <stdio.h>
#include <stdlib.h>

extern int yylex (void);
extern int yyparse();
extern FILE* yyin;

void yyerror(const char *s);

FILE* fp_open(char* fn); 

%}

%union{
    double real;
    char* command;
}

%token <real> NUMBER
%type  <real> expr
%left '+' '-'
%left '*' '/'

%%

list: /* 	nothing */
	| 	list '\n'
	| 	expr {printf(" =%.10g", $1);}

		; 	     
		
expr:		expr '+' NUMBER {$$ = $1 + $3;}
	| 	expr '-' NUMBER {$$ = $1 - $3;}
	| 	expr '*' NUMBER {$$ = $1 * $3;}
	| 	expr '/' NUMBER {$$ = $1 / $3;}
	|	NUMBER {$$ = $1;}
		;
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
