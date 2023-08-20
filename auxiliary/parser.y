%{
  #include <iostream>
  #include <string>
  #include "../../inc/all_instructions.hpp"
  #include "../../inc/all_directives.hpp"
  #include "../../inc/symbol.hpp"
  #include "../../inc/literal.hpp"
  #include "../../inc/command_builder.hpp"
   #include "../../inc/assembler.hpp"

  extern int yylex (void);
  extern void yyerror(char* s);

  void build_instruction(type::INSTRUCTION_TYPE ins_type)
  {
    Instruction *ins = CommandBuilder::get_instance().build_instruction(ins_type);
    if(ins) 
      Assembler::get_instance().add_command(ins);
  }

  void build_directive(type::DIRECTIVE_TYPE dir_type)
  {
    Directive *dir = CommandBuilder::get_instance().build_directive(dir_type);
    if(dir) 
      Assembler::get_instance().add_command(dir);
  }
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

%token PLUS
%token SQUARE_BRACKET_L
%token SQUARE_BRACKET_R
%token DOLLAR
%token COMMA
%token EOL
%token UNDEFIEND

%type<symbol_name> symbol_literal_list;


%%

code: 
| line code
;

line:
DIRECTIVE_WORD symbol_literal_list EOL { std::cout << ".word" << std::endl; build_directive(type::DIRECTIVE_TYPE::WORD); }
| DIRECTIVE_GLOBAL symbol_list EOL { std::cout << ".global" << std::endl; build_directive(type::DIRECTIVE_TYPE::GLOBAL); }
| DIRECTIVE_EXTERN symbol_list EOL { std::cout << ".extern" << std::endl; build_directive(type::DIRECTIVE_TYPE::EXTERN); }
| DIRECTIVE_SECTION single_symbol EOL { std::cout << ".section" << std::endl; build_directive(type::DIRECTIVE_TYPE::SECTION); }
| DIRECTIVE_SKIP all_num_literals EOL { std::cout << ".skip" << std::endl; build_directive(type::DIRECTIVE_TYPE::SKIP); }
| DIRECTIVE_END EOL { std::cout << ".end" << std::endl; build_directive(type::DIRECTIVE_TYPE::END); }
| DIRECTIVE_ASCII string_literal EOL { std::cout << ".ascii" << std::endl; build_directive(type::DIRECTIVE_TYPE::ASCII); }
| LABEL { std::cout << "labela: " << $1 << std::endl; Assembler::get_instance().add_symbol(new Symbol($1)); }
| INSTRUCTION_HALT EOL { std::cout << "halt" << std::endl; build_instruction(type::INSTRUCTION_TYPE::HALT); }
| INSTRUCTION_INT EOL { std::cout << "int" << std::endl; build_instruction(type::INSTRUCTION_TYPE::INT); }
| INSTRUCTION_IRET EOL { std::cout << "iret" << std::endl; build_instruction(type::INSTRUCTION_TYPE::IRET); }
| INSTRUCTION_RET EOL { std::cout << "ret" << std::endl; build_instruction(type::INSTRUCTION_TYPE::RET); }
| INSTRUCTION_CALL single_symbol EOL { std::cout << "call" << std::endl; build_instruction(type::INSTRUCTION_TYPE::CALL); }
| INSTRUCTION_CALL all_num_literals EOL { std::cout << "call" << std::endl; build_instruction(type::INSTRUCTION_TYPE::CALL); }
| INSTRUCTION_JMP single_symbol EOL { std::cout << "jmp" << std::endl; build_instruction(type::INSTRUCTION_TYPE::JMP); }
| INSTRUCTION_JMP all_num_literals EOL { std::cout << "jmp" << std::endl; build_instruction(type::INSTRUCTION_TYPE::JMP); }
| INSTRUCTION_BEQ single_gp_reg COMMA single_gp_reg COMMA single_symbol EOL { std::cout << "beq" << std::endl; build_instruction(type::INSTRUCTION_TYPE::BEQ); }
| INSTRUCTION_BEQ single_gp_reg COMMA single_gp_reg COMMA all_num_literals EOL { std::cout << "beq" << std::endl; build_instruction(type::INSTRUCTION_TYPE::BEQ); }
| INSTRUCTION_BNE single_gp_reg COMMA single_gp_reg COMMA single_symbol EOL { std::cout << "bne" << std::endl; build_instruction(type::INSTRUCTION_TYPE::BNE); }
| INSTRUCTION_BNE single_gp_reg COMMA single_gp_reg COMMA all_num_literals EOL { std::cout << "bne" << std::endl; build_instruction(type::INSTRUCTION_TYPE::BNE); }
| INSTRUCTION_BGT single_gp_reg COMMA single_gp_reg COMMA single_symbol EOL { std::cout << "bgt" << std::endl; build_instruction(type::INSTRUCTION_TYPE::BGT); }
| INSTRUCTION_BGT single_gp_reg COMMA single_gp_reg COMMA all_num_literals EOL { std::cout << "bgt" << std::endl; build_instruction(type::INSTRUCTION_TYPE::BGT); }
| INSTRUCTION_PUSH single_gp_reg EOL { std::cout << "push" << std::endl; build_instruction(type::INSTRUCTION_TYPE::PUSH); }
| INSTRUCTION_POP single_gp_reg EOL { std::cout << "pop" << std::endl; build_instruction(type::INSTRUCTION_TYPE::POP); }
| INSTRUCTION_XCHG single_gp_reg COMMA single_gp_reg EOL { std::cout << "xchg" << std::endl; build_instruction(type::INSTRUCTION_TYPE::XCHG); }
| INSTRUCTION_ADD single_gp_reg COMMA single_gp_reg EOL { std::cout << "add" << std::endl; build_instruction(type::INSTRUCTION_TYPE::ADD); }
| INSTRUCTION_SUB single_gp_reg COMMA single_gp_reg EOL { std::cout << "sub" << std::endl; build_instruction(type::INSTRUCTION_TYPE::SUB); }
| INSTRUCTION_MUL single_gp_reg COMMA single_gp_reg EOL { std::cout << "mul" << std::endl; build_instruction(type::INSTRUCTION_TYPE::MUL); }
| INSTRUCTION_DIV single_gp_reg COMMA single_gp_reg EOL { std::cout << "div" << std::endl; build_instruction(type::INSTRUCTION_TYPE::DIV); }
| INSTRUCTION_NOT single_gp_reg EOL { std::cout << "not" << std::endl; build_instruction(type::INSTRUCTION_TYPE::NOT); }
| INSTRUCTION_AND single_gp_reg COMMA single_gp_reg EOL { std::cout << "and" << std::endl; build_instruction(type::INSTRUCTION_TYPE::AND); }
| INSTRUCTION_OR single_gp_reg COMMA single_gp_reg EOL { std::cout << "or" << std::endl; build_instruction(type::INSTRUCTION_TYPE::OR); }
| INSTRUCTION_XOR single_gp_reg COMMA single_gp_reg EOL { std::cout << "xor" << std::endl; build_instruction(type::INSTRUCTION_TYPE::XOR); }
| INSTRUCTION_SHL single_gp_reg COMMA single_gp_reg EOL { std::cout << "shl" << std::endl; build_instruction(type::INSTRUCTION_TYPE::SHL); }
| INSTRUCTION_SHR single_gp_reg COMMA single_gp_reg EOL { std::cout << "shr" << std::endl; build_instruction(type::INSTRUCTION_TYPE::SHR); }
| INSTRUCTION_CSRRD single_cs_reg COMMA single_gp_reg EOL { std::cout << "csrrd" << std::endl; build_instruction(type::INSTRUCTION_TYPE::CSRRD); }
| INSTRUCTION_CSRWR single_gp_reg COMMA single_cs_reg EOL { std::cout << "csrwr" << std::endl; build_instruction(type::INSTRUCTION_TYPE::CSRWR); }
| INSTRUCTION_LD ld_st_operand COMMA single_gp_reg EOL { std::cout << "ld" << std::endl; build_instruction(type::INSTRUCTION_TYPE::LD); }
| INSTRUCTION_ST single_gp_reg COMMA ld_st_operand EOL { std::cout << "st" << std::endl; build_instruction(type::INSTRUCTION_TYPE::ST); }
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
SYMBOL { std::cout << "simbol: " << $1 << std::endl;  CommandBuilder::get_instance().enque_param(new Symbol($1)); }
;

all_num_literals:
LITERAL_BIN { std::cout << "literal: " << std::stoi($1 + 2, 0, 2) << std::endl; CommandBuilder::get_instance().enque_param(new Literal(std::stoi($1 + 2, 0, 2))); }
| LITERAL_OCT { std::cout << "literal: " << std::stoi($1, 0, 8) << std::endl;   CommandBuilder::get_instance().enque_param(new Literal(std::stoi($1, 0, 8))); }
| LITERAL_DEC { std::cout << "literal: " << std::stoi($1, 0, 10) << std::endl;  CommandBuilder::get_instance().enque_param(new Literal(std::stoi($1, 0, 10))); }
| LITERAL_HEX { std::cout << "literal: " << std::stoi($1, 0, 16) << std::endl;  CommandBuilder::get_instance().enque_param(new Literal(std::stoi($1, 0, 16))); }
;

string_literal:
LITERAL_STRING { std::cout << "literal: " << $1 << std::endl; CommandBuilder::get_instance().enque_param(new Literal($1)); }
;

single_gp_reg:
GP_REG { std::cout << "GP registar: " << $1 << std::endl; CommandBuilder::get_instance().set_next_gp_reg(static_cast<type::GP_REG>($1)); }
;

single_cs_reg:
CS_REG { std::cout << "CS registar: " << $1 << std::endl; CommandBuilder::get_instance().set_next_cs_reg(static_cast<type::CS_REG>($1)); }
;

ld_st_operand:
DOLLAR all_num_literals { std::cout << "immed" << std::endl; CommandBuilder::get_instance().set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES::IMMED); }
| DOLLAR single_symbol { std::cout << "immed" << std::endl; CommandBuilder::get_instance().set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES::IMMED); }
| all_num_literals { std::cout << "mem dir" << std::endl; CommandBuilder::get_instance().set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES::MEM_DIR); }
| single_symbol  { std::cout << "mem dir" << std::endl; CommandBuilder::get_instance().set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES::MEM_DIR); }
| single_cs_reg  { std::cout << "reg dir" << std::endl; CommandBuilder::get_instance().set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES::REG_DIR); }
| single_gp_reg  { std::cout << "reg dir" << std::endl; CommandBuilder::get_instance().set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES::REG_DIR); }
| SQUARE_BRACKET_L single_cs_reg SQUARE_BRACKET_R  { std::cout << "reg mem" << std::endl; CommandBuilder::get_instance().set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES::REG_IND); }
| SQUARE_BRACKET_L single_gp_reg SQUARE_BRACKET_R  { std::cout << "reg mem" << std::endl; CommandBuilder::get_instance().set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES::REG_IND); }
| SQUARE_BRACKET_L single_cs_reg PLUS all_num_literals SQUARE_BRACKET_R { std::cout << "reg mem with disp" << std::endl; CommandBuilder::get_instance().set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES::REG_IND_WITH_DISP); }
| SQUARE_BRACKET_L single_gp_reg PLUS all_num_literals SQUARE_BRACKET_R { std::cout << "reg mem with disp" << std::endl; CommandBuilder::get_instance().set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES::REG_IND_WITH_DISP); }
| SQUARE_BRACKET_L single_cs_reg PLUS single_symbol SQUARE_BRACKET_R { std::cout << "reg mem with disp" << std::endl; CommandBuilder::get_instance().set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES::REG_IND_WITH_DISP); }
| SQUARE_BRACKET_L single_gp_reg PLUS single_symbol SQUARE_BRACKET_R { std::cout << "reg mem with disp" << std::endl; CommandBuilder::get_instance().set_mem_addr_mode(type::MEMORY_ADDRESSING_MODES::REG_IND_WITH_DISP); }

%%