
%{

#include <iostream>

extern "C" int yylex(void);

void yyerror(const char *str) {
    std::cout << "Error: " << str << std::endl;
}

int yywrap() {
    return 1;
}   

%}

%token TOK_NUMBER TOK_IDENTIFIER TOK_BS TOK_BE TOK_PROJECT TOK_PROGRAM TOK_LIBRARY TOK_EOL;

%%

borcfile: 
    project
    |
    directive_list;

project:
    TOK_PROJECT TOK_IDENTIFIER TOK_EOL
    ;

directive_list:
    directive_list
    |
    program
    |
    library
    ;

program:
    TOK_PROGRAM TOK_IDENTIFIER TOK_EOL
    ;

library:
    TOK_LIBRARY TOK_IDENTIFIER TOK_EOL
    ;

%%
