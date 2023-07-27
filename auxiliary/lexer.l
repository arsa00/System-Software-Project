%{
  #include "../inc/parser.hpp"
  #include <string.h>
  #include <iostream>
  #include <string>
%}

%option noyywrap

%%

".global" { return DIRECTIVE_GLOBAL; }
".extern" { return DIRECTIVE_EXTERN; }
".section" { return DIRECTIVE_SECTION; }
".word" { return DIRECTIVE_WORD; }
".skip" { return DIRECTIVE_SKIP; }
".ascii" { return DIRECTIVE_ASCII; }
".end" { return DIRECTIVE_END; }

"halt" { return INSTRUCTION_HALT; }
"int" { return INSTRUCTION_INT; }
"iret" { return INSTRUCTION_IRET; }
"call" { return INSTRUCTION_CALL; }
"ret" { return INSTRUCTION_RET; }
"jmp" { return INSTRUCTION_JMP; }
"beq" { return INSTRUCTION_BEQ; }
"bne" { return INSTRUCTION_BNE; }
"bgt" { return INSTRUCTION_BGT; }
"push" { return INSTRUCTION_PUSH; }
"pop" { return INSTRUCTION_POP; }
"xchg" { return INSTRUCTION_XCHG; }
"add" { return INSTRUCTION_ADD; }
"sub" { return INSTRUCTION_SUB; }
"mul" { return INSTRUCTION_MUL; }
"div" { return INSTRUCTION_DIV; }
"not" { return INSTRUCTION_NOT; }
"and" { return INSTRUCTION_AND; }
"or" { return INSTRUCTION_OR; }
"xor" { return INSTRUCTION_XOR; }
"shl" { return INSTRUCTION_SHL; }
"shr" { return INSTRUCTION_SHR; }
"ld" { return INSTRUCTION_LD; }
"st" { return INSTRUCTION_ST; }
"csrrd" { return INSTRUCTION_CSRRD; }
"csrwr" { return INSTRUCTION_CSRWR; }

[._A-Za-z][._0-9A-Za-z]* { yylval.symbol_name = yytext; return SYMBOL; }
[._A-Za-z][._0-9A-Za-z]*: { yytext[strlen(yytext)-1] = '\0'; yylval.symbol_name = yytext; return LABEL; }

0b[0-1]+ { yylval.literal_value = yytext; return LITERAL_BIN; }
0[0-7]* { yylval.literal_value = yytext; return LITERAL_OCT; }
[1-9][0-9]* { yylval.literal_value = yytext; return LITERAL_DEC; }
0x[0-9a-fA-F]+ { yylval.literal_value = yytext; return LITERAL_HEX; }
\"([^"\\\n]|\\(.|\n))*\" { yytext[strlen(yytext)-1] = '\0'; yylval.literal_value = yytext + 1; return LITERAL_STRING; }

(%r1[0-5]|%r[0-9]) { yylval.gpr_index = std::stoi(yytext+2, 0, 10); return GP_REG; }
"%status" { yylval.gpr_index = 0; return CS_REG; }
"%handler" { yylval.gpr_index = 1; return CS_REG; }
"%cause" { yylval.gpr_index = 2; return CS_REG; }

"," { return COMMA; }
\n { return EOL; }
#.*\n { return EOL; }
[ \t]+ { }
. { return UNDEFIEND; }

%%