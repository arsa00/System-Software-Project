%{
  #include <iostream>
  #include <string>
  extern int yylex (void);
  extern void yyerror(char* s);
%}


%union {
  char *symbol_name;
  char *literal_value;
  int gpr_index;
  int csr_index;
}

%token INSTRUCTION_HALT
%token INSTRUCTION_INT
%token INSTRUCTION_IRET
%token INSTRUCTION_CALL
%token INSTRUCTION_RET
%token INSTRUCTION_JMP
%token INSTRUCTION_BEQ
%token INSTRUCTION_BNE
%token INSTRUCTION_BGT
%token INSTRUCTION_PUSH
%token INSTRUCTION_POP
%token INSTRUCTION_XCHG
%token INSTRUCTION_ADD
%token INSTRUCTION_SUB
%token INSTRUCTION_MUL
%token INSTRUCTION_DIV
%token INSTRUCTION_NOT
%token INSTRUCTION_AND
%token INSTRUCTION_OR
%token INSTRUCTION_XOR
%token INSTRUCTION_SHL
%token INSTRUCTION_SHR
%token INSTRUCTION_LD
%token INSTRUCTION_ST
%token INSTRUCTION_CSRRD
%token INSTRUCTION_CSRWR


%token DIRECTIVE_GLOBAL
%token DIRECTIVE_EXTERN
%token DIRECTIVE_SECTION
%token DIRECTIVE_WORD
%token DIRECTIVE_SKIP
%token DIRECTIVE_ASCII
%token DIRECTIVE_EQU
%token DIRECTIVE_END

%token<symbol_name> LABEL
%token<symbol_name> SYMBOL
%token<literal_value> LITERAL_BIN
%token<literal_value> LITERAL_OCT
%token<literal_value> LITERAL_DEC
%token<literal_value> LITERAL_HEX
%token<literal_value> LITERAL_STRING

%token<gpr_index> GP_REG;
%token<csr_index> CS_REG;

%token COMMA
%token EOL
%token UNDEFIEND

%type<symbol_name> symbol_literal_list;


%%

code: 
| line code
;

line:
DIRECTIVE_WORD symbol_literal_list EOL { std::cout << ".word" << std::endl; }
| DIRECTIVE_GLOBAL symbol_list EOL { std::cout << ".global" << std::endl; }
| DIRECTIVE_EXTERN symbol_list EOL { std::cout << ".extern" << std::endl; }
| DIRECTIVE_SECTION single_symbol EOL { std::cout << ".section" << std::endl; }
| DIRECTIVE_SKIP all_num_literals EOL { std::cout << ".skip" << std::endl; }
| DIRECTIVE_END EOL { std::cout << ".end" << std::endl; }
| DIRECTIVE_ASCII string_literal EOL { std::cout << ".ascii" << std::endl; }
| LABEL { std::cout << "labela: " << $1 << std::endl; }
| INSTRUCTION_HALT EOL { std::cout << "halt" << std::endl; }
| INSTRUCTION_INT EOL { std::cout << "int" << std::endl; }
| INSTRUCTION_IRET EOL { std::cout << "iret" << std::endl; }
| INSTRUCTION_RET EOL { std::cout << "ret" << std::endl; }
| INSTRUCTION_CALL single_symbol EOL { std::cout << "call" << std::endl; }
| INSTRUCTION_CALL all_num_literals EOL { std::cout << "call" << std::endl; }
| INSTRUCTION_JMP single_symbol EOL { std::cout << "jmp" << std::endl; }
| INSTRUCTION_JMP all_num_literals EOL { std::cout << "jmp" << std::endl; }
| INSTRUCTION_BEQ single_gp_reg COMMA single_gp_reg COMMA single_symbol EOL { std::cout << "beq" << std::endl; }
| INSTRUCTION_BEQ single_gp_reg COMMA single_gp_reg COMMA all_num_literals EOL { std::cout << "beq" << std::endl; }
| INSTRUCTION_BNE single_gp_reg COMMA single_gp_reg COMMA single_symbol EOL { std::cout << "bne" << std::endl; }
| INSTRUCTION_BNE single_gp_reg COMMA single_gp_reg COMMA all_num_literals EOL { std::cout << "bne" << std::endl; }
| INSTRUCTION_BGT single_gp_reg COMMA single_gp_reg COMMA single_symbol EOL { std::cout << "bgt" << std::endl; }
| INSTRUCTION_BGT single_gp_reg COMMA single_gp_reg COMMA all_num_literals EOL { std::cout << "bgt" << std::endl; }
| INSTRUCTION_PUSH single_gp_reg EOL { std::cout << "push" << std::endl; }
| INSTRUCTION_POP single_gp_reg EOL { std::cout << "pop" << std::endl; }
| INSTRUCTION_XCHG single_gp_reg COMMA single_gp_reg EOL { std::cout << "xchg" << std::endl; }
| INSTRUCTION_ADD single_gp_reg COMMA single_gp_reg EOL { std::cout << "add" << std::endl; }
| INSTRUCTION_SUB single_gp_reg COMMA single_gp_reg EOL { std::cout << "sub" << std::endl; }
| INSTRUCTION_MUL single_gp_reg COMMA single_gp_reg EOL { std::cout << "mul" << std::endl; }
| INSTRUCTION_DIV single_gp_reg COMMA single_gp_reg EOL { std::cout << "div" << std::endl; }
| INSTRUCTION_NOT single_gp_reg EOL { std::cout << "not" << std::endl; }
| INSTRUCTION_AND single_gp_reg COMMA single_gp_reg EOL { std::cout << "and" << std::endl; }
| INSTRUCTION_OR single_gp_reg COMMA single_gp_reg EOL { std::cout << "or" << std::endl; }
| INSTRUCTION_XOR single_gp_reg COMMA single_gp_reg EOL { std::cout << "xor" << std::endl; }
| INSTRUCTION_SHL single_gp_reg COMMA single_gp_reg EOL { std::cout << "shl" << std::endl; }
| INSTRUCTION_SHR single_gp_reg COMMA single_gp_reg EOL { std::cout << "shr" << std::endl; }
| INSTRUCTION_CSRRD single_cs_reg COMMA single_gp_reg EOL { std::cout << "csrrd" << std::endl; }
| INSTRUCTION_CSRWR single_gp_reg COMMA single_cs_reg EOL { std::cout << "csrwr" << std::endl; }
| EOL
;

symbol_list:
symbol_list COMMA symbol_list
| single_symbol
;

symbol_literal_list:
symbol_literal_list COMMA symbol_literal_list
| single_symbol
| all_num_literals
;

single_symbol:
SYMBOL { std::cout << "simbol: " << $1 << std::endl; }
;

all_num_literals:
LITERAL_BIN { std::cout << "literal: " << std::stoi($1 + 2, 0, 2) << std::endl; }
| LITERAL_OCT { std::cout << "literal: " << std::stoi($1, 0, 8) << std::endl; }
| LITERAL_DEC { std::cout << "literal: " << std::stoi($1, 0, 10) << std::endl; }
| LITERAL_HEX { std::cout << "literal: " << std::stoi($1, 0, 16) << std::endl; }
;

string_literal:
LITERAL_STRING { std::cout << "literal: " << $1 << std::endl; }
;

single_gp_reg:
GP_REG { std::cout << "GP registar: " << $1 << std::endl; }
;

single_cs_reg:
CS_REG { std::cout << "CS registar: " << $1 << std::endl; }
;

%%