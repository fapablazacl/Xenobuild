
%{
#include <iostream>
#include "parser.hpp"
#define YY_DECL extern "C" int yylex()

int linenum = 1;

void display_token(const char *msg) {
    std::cout << msg << yytext << " at line " << linenum << std::endl;
}
%}

%%
\n                      { ++linenum; }
[ \t\n]                 ;
"project"               { return TOK_PROJECT; }
"program"               { return TOK_PROGRAM; }
"library"               { return TOK_LIBRARY; }
"path"                  { return TOK_PATH; }
"import"                { return TOK_IMPORT; }
"include"               { return TOK_INCLUDE; }
"sources"               { return TOK_SOURCES; }
";"                     { return TOK_EOL; }
"{"                     { return TOK_BS; }
"}"                     { return TOK_BE; }
"="                     { return TOK_EQUAL; }
","                     { return TOK_COMMA; }
"."                     { return TOK_DOT; }
"/"                     { return TOK_SLASH; }
[0-9]+                  { yylval.number = std::atoi(yytext); return TOK_NUMBER; }
[a-zA-Z][a-zA-Z0-9]*    { yylval.string = new std::string(yytext); return TOK_IDENTIFIER; }
[a-zA-Z\.][a-zA-Z0-9\.]*  { yylval.string = new std::string(yytext); return TOK_PATHPART; }
%%
