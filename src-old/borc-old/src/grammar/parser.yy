
%{
#include <string>
#include <iostream>

extern int linenum;

void display(const char *msg) {
    std::cout << msg << " at line " << linenum << std::endl;
}

extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

extern int linenum;

void yyerror(const char *str) {
    std::cout << str << " at line " << linenum << std::endl;
}
%}

%union {
    int number;
    std::string *string;
    int token;
}

%token <token> TOK_PROJECT TOK_PROGRAM TOK_LIBRARY TOK_IMPORT TOK_SOURCES TOK_INCLUDE TOK_PATH TOK_EOL;
%token <token> TOK_BS TOK_BE TOK_EQUAL TOK_COMMA TOK_DOT TOK_SLASH;
%token <string> TOK_IDENTIFIER TOK_PATHPART
%token <number> TOK_NUMBER

%%
borcfile:
    project buildable_list { display("Done with the borcfile"); }
     
project:
    TOK_PROJECT TOK_IDENTIFIER TOK_EOL  { display("Matched a Project"); }

buildable_list:
    buildable_list buildable    
    | buildable                         { display("Matched a Buildable List"); }

buildable: 
    TOK_PROGRAM TOK_IDENTIFIER TOK_BS buildable_content TOK_BE 
    | TOK_LIBRARY TOK_IDENTIFIER TOK_BS buildable_content TOK_BE { display("Matched a Buildable"); }

buildable_content:
    buildable_content buildable_directive
    | buildable_directive               { display("Matched a Buildable Content"); }
    
buildable_directive:
    import_directive | include_directive | sources_directive | path_directive { display("Matched a Buildable Directive"); }
    
import_directive:
    TOK_IMPORT TOK_IDENTIFIER TOK_EOL { display("Matched a Import Directive"); }

include_directive:
    TOK_INCLUDE TOK_EQUAL paths { display("Matched a Include Directive"); }

sources_directive:
    TOK_SOURCES TOK_EQUAL paths { display("Matched a Sources Directive"); }

path_directive:
    TOK_PATH TOK_EQUAL path TOK_EOL { display("Matched a Path Directive"); }

paths:
    TOK_BS paths TOK_COMMA path TOK_BE
    | path TOK_EOL { display("Matched a Path(s)"); }

path:
    path_parts { display("Matched a Path"); }

path_parts:
    path_part TOK_SLASH path_parts
    | path_part { display("Matched a Path Parts"); }

path_part:
    TOK_PATHPART { display("Matched a Path Part"); }
%%

#include <cstdio>

int main(int argc, char **argv) {
    FILE *file = std::fopen(argv[1], "r");

    if (!file) {
        std::cout << "No se pudo abrir el archivo " << argv[1] << std::endl;
        return 1;
    }

    yyin = file;

    do {
        yyparse();

    } while( !std::feof(yyin) );

    std::fclose(file);

    return 0;
}
