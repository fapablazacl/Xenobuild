
%{
extern "C" int yylex(void);

#include "BorcFile.yy.hpp"
%}

%%
[0-9]+                  return TOK_NUMBER;
[a-zA-Z][a-zA-Z0-9]*    return TOK_IDENTIFIER;
{                       return TOK_BS;
}                       return TOK_BE;
project                 return TOK_PROJECT;
program                 return TOK_PROGRAM;
library                 return TOK_LIBRARY;
;                       return TOK_EOL;
%%
