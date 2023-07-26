%{
  #include <iostream>
  extern int yylex (void);
  extern void yyerror(char* s);
%}

%union {
  char *instruction_alias;
}

%token<instruction_alias> INSTRUCTION
%token<instruction_alias> DIRECTIVE
%token EOL

%%

code: 
| code_line code
;

code_line: 
INSTRUCTION EOL { std::cout << "INSTRUKCIJA: " << $1 << std::endl; }
| DIRECTIVE EOL { std::cout << "DIREKTIVA: " << $1 << std::endl; }
;

%%