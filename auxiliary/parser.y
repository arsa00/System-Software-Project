%{
  #include <iostream>
  extern int yylex (void);
  extern void yyerror(char* s);
%}


%union {
  char *alias;
  char *symbol_name;
  int literal_value;
}

%token<alias> INSTRUCTION_ZERO_PARAM
%token<alias> INSTRUCTION_ONE_PARAM
%token<alias> INSTRUCTION_TWO_PARAM
%token<alias> INSTRUCTION_THREE_PARAM

%token DIRECTIVE_GLOBAL
%token DIRECTIVE_EXTERN
%token DIRECTIVE_SECTION
%token DIRECTIVE_WORD
%token DIRECTIVE_SKIP
%token DIRECTIVE_ASCII
%token DIRECTIVE_EQU
%token DIRECTIVE_END

%token<symbol_name> SYMBOL
%token<literal_value> LITERAL

%token WHITESPACE
%token COMMA
%token EOL
%token UNDEFIEND


%%

code: 
| line code
;

line:
EOL
| .word EOL { std::cout << ".word" << std::endl; }
| symbol_literal_list
;

.word:
DIRECTIVE_WORD
;

symbol_literal_list:
SYMBOL COMMA SYMBOL { std::cout << "simbol1: " << $1 << " simbol2: " << $3 << std::endl; }
| SYMBOL COMMA LITERAL { std::cout << "simbol1: " << $1 << " literal2: " << $3 << std::endl; }
| LITERAL COMMA SYMBOL { std::cout << "literal1: " << $1 << " simbol2: " << $3 << std::endl; }
| LITERAL COMMA LITERAL { std::cout << "literal1: " << $1 << " literal2: " << $3 << std::endl; }
| SYMBOL { std::cout << "simbol: " << $1 << std::endl; }
| LITERAL { std::cout << "literal: " << $1 << std::endl; }
;

%%