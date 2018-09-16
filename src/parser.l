%{

/* #include <stdio.h> */
#include "y.tab.h"

%}

%option noyywrap

%%

[E|e][H|h][L|l][O|o]		return CMD_EHLO; 
\[                              return TOK_BRACKET_O;
\]                              return TOK_BRACKET_C;
\{                              return TOK_PARAN_O;
\}                              return TOK_PARAN_C;
[A-Za-z]*                          return WORD;
;				return SEMICOLON;
\n                              /* ignore EOL */;
[ \t]+                          /* ignore whitespace */;
%%

