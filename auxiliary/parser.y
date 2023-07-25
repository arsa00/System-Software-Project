%{
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
INSTRUCTION EOL { printf("INSTRUKCIJA: %s\n", $1); }
| DIRECTIVE EOL { printf("DIREKTIVA: %s\n", $1); }
;

%%