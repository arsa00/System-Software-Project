%{
  #include <iostream>
  #include <string>
  extern int yylex (void);
  extern void yyerror(char* s);
%}


%union {
  char *alias;
  char *symbol_name;
  char *literal_value;
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
%token<literal_value> LITERAL_BIN
%token<literal_value> LITERAL_OCT
%token<literal_value> LITERAL_DEC
%token<literal_value> LITERAL_HEX

%token WHITESPACE
%token COMMA
%token EOL
%token UNDEFIEND

%type<symbol_name> symbol_literal_list;

%%

code: 
| line code
;

line:
.word symbol_literal_list EOL { std::cout << ".word" << std::endl; }
;

.word:
DIRECTIVE_WORD
;

symbol_literal_list:
symbol_literal_list COMMA symbol_literal_list
| SYMBOL { std::cout << "simbol: " << $1 << std::endl; }
| LITERAL_BIN { std::cout << "literal: " << std::stoi($1 + 2, 0, 2) << std::endl; }
| LITERAL_OCT { std::cout << "literal: " << std::stoi($1, 0, 8) << std::endl; }
| LITERAL_DEC { std::cout << "literal: " << std::stoi($1, 0, 10) << std::endl; }
| LITERAL_HEX { std::cout << "literal: " << std::stoi($1, 0, 16) << std::endl; }
;

%%